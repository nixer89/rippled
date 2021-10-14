//------------------------------------------------------------------------------
/*
    This file is part of xrpld: https://github.com/xrplf/xrpld
    Copyright (c) 2021 XRP Ledger Foundation

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <xrpl/app/ledger/LedgerMaster.h>
#include <xrpl/app/main/Application.h>
#include <xrpl/app/misc/HashRouter.h>
#include <xrpl/app/misc/Transaction.h>
#include <xrpl/app/rdb/RelationalDBInterface_postgres.h>
#include <xrpl/app/rdb/backend/RelationalDBInterfacePostgres.h>
#include <xrpl/app/rdb/backend/RelationalDBInterfaceSqlite.h>
#include <xrpl/app/tx/apply.h>
#include <xrpl/basics/Log.h>
#include <xrpl/basics/safe_cast.h>
#include <xrpl/core/DatabaseCon.h>
#include <xrpl/core/Pg.h>
#include <xrpl/json/json_reader.h>
#include <xrpl/protocol/ErrorCodes.h>
#include <xrpl/protocol/Feature.h>
#include <xrpl/protocol/jss.h>

namespace ripple {

Transaction::Transaction(
    std::shared_ptr<STTx const> const& stx,
    std::string& reason,
    Application& app) noexcept
    : mTransaction(stx), mApp(app), j_(app.journal("Ledger"))
{
    try
    {
        mTransactionID = mTransaction->getTransactionID();
    }
    catch (std::exception& e)
    {
        reason = e.what();
        return;
    }

    mStatus = NEW;
}

//
// Misc.
//

void
Transaction::setStatus(TransStatus ts, std::uint32_t lseq)
{
    mStatus = ts;
    mInLedger = lseq;
}

TransStatus
Transaction::sqlTransactionStatus(boost::optional<std::string> const& status)
{
    char const c = (status) ? (*status)[0] : safe_cast<char>(txnSqlUnknown);

    switch (c)
    {
        case txnSqlNew:
            return NEW;
        case txnSqlConflict:
            return CONFLICTED;
        case txnSqlHeld:
            return HELD;
        case txnSqlValidated:
            return COMMITTED;
        case txnSqlIncluded:
            return INCLUDED;
    }

    assert(c == txnSqlUnknown);
    return INVALID;
}

Transaction::pointer
Transaction::transactionFromSQL(
    boost::optional<std::uint64_t> const& ledgerSeq,
    boost::optional<std::string> const& status,
    Blob const& rawTxn,
    Application& app)
{
    std::uint32_t const inLedger =
        rangeCheckedCast<std::uint32_t>(ledgerSeq.value_or(0));

    SerialIter it(makeSlice(rawTxn));
    auto txn = std::make_shared<STTx const>(it);
    std::string reason;
    auto tr = std::make_shared<Transaction>(txn, reason, app);

    tr->setStatus(sqlTransactionStatus(status));
    tr->setLedger(inLedger);
    return tr;
}

std::variant<
    std::pair<std::shared_ptr<Transaction>, std::shared_ptr<TxMeta>>,
    TxSearched>
Transaction::load(uint256 const& id, Application& app, error_code_i& ec)
{
    return load(id, app, std::nullopt, ec);
}

std::variant<
    std::pair<std::shared_ptr<Transaction>, std::shared_ptr<TxMeta>>,
    TxSearched>
Transaction::load(
    uint256 const& id,
    Application& app,
    ClosedInterval<uint32_t> const& range,
    error_code_i& ec)
{
    using op = std::optional<ClosedInterval<uint32_t>>;

    return load(id, app, op{range}, ec);
}

Transaction::Locator
Transaction::locate(uint256 const& id, Application& app)
{
    return dynamic_cast<RelationalDBInterfacePostgres*>(
               &app.getRelationalDBInterface())
        ->locateTransaction(id);
}

std::variant<
    std::pair<std::shared_ptr<Transaction>, std::shared_ptr<TxMeta>>,
    TxSearched>
Transaction::load(
    uint256 const& id,
    Application& app,
    std::optional<ClosedInterval<uint32_t>> const& range,
    error_code_i& ec)
{
    return dynamic_cast<RelationalDBInterfaceSqlite*>(
               &app.getRelationalDBInterface())
        ->getTransaction(id, range, ec);
}

// options 1 to include the date of the transaction
Json::Value
Transaction::getJson(JsonOptions options, bool binary) const
{
    Json::Value ret(mTransaction->getJson(JsonOptions::none, binary));

    if (mInLedger)
    {
        ret[jss::inLedger] = mInLedger;  // Deprecated.
        ret[jss::ledger_index] = mInLedger;

        if (options == JsonOptions::include_date)
        {
            auto ct = mApp.getLedgerMaster().getCloseTimeBySeq(mInLedger);
            if (ct)
                ret[jss::date] = ct->time_since_epoch().count();
        }
    }

    return ret;
}

}  // namespace ripple
