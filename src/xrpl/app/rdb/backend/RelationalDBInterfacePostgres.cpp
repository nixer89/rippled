//------------------------------------------------------------------------------
/*
    This file is part of xrpld: https://github.com/xrplf/xrpld
    Copyright (c) 2020 XRP Ledger Foundation

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

#include <xrpl/app/ledger/AcceptedLedger.h>
#include <xrpl/app/ledger/LedgerMaster.h>
#include <xrpl/app/ledger/LedgerToJson.h>
#include <xrpl/app/ledger/TransactionMaster.h>
#include <xrpl/app/main/Application.h>
#include <xrpl/app/misc/Manifest.h>
#include <xrpl/app/misc/impl/AccountTxPaging.h>
#include <xrpl/app/rdb/RelationalDBInterface_nodes.h>
#include <xrpl/app/rdb/RelationalDBInterface_postgres.h>
#include <xrpl/app/rdb/backend/RelationalDBInterfacePostgres.h>
#include <xrpl/basics/BasicConfig.h>
#include <xrpl/basics/StringUtilities.h>
#include <xrpl/core/DatabaseCon.h>
#include <xrpl/core/Pg.h>
#include <xrpl/core/SociDB.h>
#include <xrpl/json/to_string.h>
#include <xrpl/nodestore/DatabaseShard.h>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <soci/sqlite3/soci-sqlite3.h>

namespace xrpl {

class RelationalDBInterfacePostgresImp : public RelationalDBInterfacePostgres
{
public:
    RelationalDBInterfacePostgresImp(
        Application& app,
        Config const& config,
        JobQueue& jobQueue)
        : app_(app)
        , j_(app_.journal("PgPool"))
        , pgPool_(
#ifdef XRPLD_REPORTING
              make_PgPool(config.section("ledger_tx_tables"), j_)
#endif
          )
    {
        assert(config.reporting());
#ifdef XRPLD_REPORTING
        if (config.reporting() && !config.reportingReadOnly())  // use pg
        {
            initSchema(pgPool_);
        }
#endif
    }

    void
    stop() override
    {
#ifdef XRPLD_REPORTING
        pgPool_->stop();
#endif
    }

    void
    sweep() override;

    std::optional<LedgerIndex>
    getMinLedgerSeq() override;

    std::optional<LedgerIndex>
    getMaxLedgerSeq() override;

    std::string
    getCompleteLedgers() override;

    std::chrono::seconds
    getValidatedLedgerAge() override;

    bool
    writeLedgerAndTransactions(
        LedgerInfo const& info,
        std::vector<AccountTransactionsData> const& accountTxData) override;

    std::optional<LedgerInfo>
    getLedgerInfoByIndex(LedgerIndex ledgerSeq) override;

    std::optional<LedgerInfo>
    getNewestLedgerInfo() override;

    std::optional<LedgerInfo>
    getLedgerInfoByHash(uint256 const& ledgerHash) override;

    uint256
    getHashByIndex(LedgerIndex ledgerIndex) override;

    std::optional<LedgerHashPair>
    getHashesByIndex(LedgerIndex ledgerIndex) override;

    std::map<LedgerIndex, LedgerHashPair>
    getHashesByIndex(LedgerIndex minSeq, LedgerIndex maxSeq) override;

    std::vector<uint256>
    getTxHashes(LedgerIndex seq) override;

    std::vector<std::shared_ptr<Transaction>>
    getTxHistory(LedgerIndex startIndex) override;

    std::pair<AccountTxResult, RPC::Status>
    getAccountTx(AccountTxArgs const& args) override;

    Transaction::Locator
    locateTransaction(uint256 const& id) override;

    bool
    ledgerDbHasSpace(Config const& config) override;

    bool
    transactionDbHasSpace(Config const& config) override;

    bool
    isCaughtUp(std::string& reason) override;

private:
    Application& app_;
    beast::Journal j_;
    std::shared_ptr<PgPool> pgPool_;

    bool
    dbHasSpace(Config const& config);
};

void
RelationalDBInterfacePostgresImp::sweep()
{
#ifdef XRPLD_REPORTING
    pgPool_->idleSweeper();
#endif
}

std::optional<LedgerIndex>
RelationalDBInterfacePostgresImp::getMinLedgerSeq()
{
    return xrpl::getMinLedgerSeq(pgPool_, j_);
}

std::optional<LedgerIndex>
RelationalDBInterfacePostgresImp::getMaxLedgerSeq()
{
    return xrpl::getMaxLedgerSeq(pgPool_);
}

std::string
RelationalDBInterfacePostgresImp::getCompleteLedgers()
{
    return xrpl::getCompleteLedgers(pgPool_);
}

std::chrono::seconds
RelationalDBInterfacePostgresImp::getValidatedLedgerAge()
{
    return xrpl::getValidatedLedgerAge(pgPool_, j_);
}

bool
RelationalDBInterfacePostgresImp::writeLedgerAndTransactions(
    LedgerInfo const& info,
    std::vector<AccountTransactionsData> const& accountTxData)
{
    return xrpl::writeLedgerAndTransactions(pgPool_, info, accountTxData, j_);
}

std::optional<LedgerInfo>
RelationalDBInterfacePostgresImp::getLedgerInfoByIndex(LedgerIndex ledgerSeq)
{
    return xrpl::getLedgerInfoByIndex(pgPool_, ledgerSeq, app_);
}

std::optional<LedgerInfo>
RelationalDBInterfacePostgresImp::getNewestLedgerInfo()
{
    return xrpl::getNewestLedgerInfo(pgPool_, app_);
}

std::optional<LedgerInfo>
RelationalDBInterfacePostgresImp::getLedgerInfoByHash(uint256 const& ledgerHash)
{
    return xrpl::getLedgerInfoByHash(pgPool_, ledgerHash, app_);
}

uint256
RelationalDBInterfacePostgresImp::getHashByIndex(LedgerIndex ledgerIndex)
{
    return xrpl::getHashByIndex(pgPool_, ledgerIndex, app_);
}

std::optional<LedgerHashPair>
RelationalDBInterfacePostgresImp::getHashesByIndex(LedgerIndex ledgerIndex)
{
    LedgerHashPair p;
    if (!xrpl::getHashesByIndex(
            pgPool_, ledgerIndex, p.ledgerHash, p.parentHash, app_))
        return {};
    return p;
}

std::map<LedgerIndex, LedgerHashPair>
RelationalDBInterfacePostgresImp::getHashesByIndex(
    LedgerIndex minSeq,
    LedgerIndex maxSeq)
{
    return xrpl::getHashesByIndex(pgPool_, minSeq, maxSeq, app_);
}

std::vector<uint256>
RelationalDBInterfacePostgresImp::getTxHashes(LedgerIndex seq)
{
    return xrpl::getTxHashes(pgPool_, seq, app_);
}

std::vector<std::shared_ptr<Transaction>>
RelationalDBInterfacePostgresImp::getTxHistory(LedgerIndex startIndex)
{
    return xrpl::getTxHistory(pgPool_, startIndex, app_, j_);
}

std::pair<AccountTxResult, RPC::Status>
RelationalDBInterfacePostgresImp::getAccountTx(AccountTxArgs const& args)
{
    return xrpl::getAccountTx(pgPool_, args, app_, j_);
}

Transaction::Locator
RelationalDBInterfacePostgresImp::locateTransaction(uint256 const& id)
{
    return xrpl::locateTransaction(pgPool_, id, app_);
}

bool
RelationalDBInterfacePostgresImp::dbHasSpace(Config const& config)
{
    /* Postgres server could be running on a different machine. */

    return true;
}

bool
RelationalDBInterfacePostgresImp::ledgerDbHasSpace(Config const& config)
{
    return dbHasSpace(config);
}

bool
RelationalDBInterfacePostgresImp::transactionDbHasSpace(Config const& config)
{
    return dbHasSpace(config);
}

std::unique_ptr<RelationalDBInterface>
getRelationalDBInterfacePostgres(
    Application& app,
    Config const& config,
    JobQueue& jobQueue)
{
    return std::make_unique<RelationalDBInterfacePostgresImp>(
        app, config, jobQueue);
}
bool
RelationalDBInterfacePostgresImp::isCaughtUp(std::string& reason)
{
#ifdef XRPLD_REPORTING
    using namespace std::chrono_literals;
    auto age = PgQuery(pgPool_)("SELECT age()");
    if (!age || age.isNull())
    {
        reason = "No ledgers in database";
        return false;
    }
    if (std::chrono::seconds{age.asInt()} > 3min)
    {
        reason = "No recently-published ledger";
        return false;
    }
#endif
    return true;
}

}  // namespace xrpl
