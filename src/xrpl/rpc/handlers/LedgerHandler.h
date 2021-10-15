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

#ifndef XRPL_RPC_HANDLERS_LEDGER_H_INCLUDED
#define XRPL_RPC_HANDLERS_LEDGER_H_INCLUDED

#include <xrpl/app/ledger/LedgerMaster.h>
#include <xrpl/app/ledger/LedgerToJson.h>
#include <xrpl/app/main/Application.h>
#include <xrpl/json/Object.h>
#include <xrpl/ledger/ReadView.h>
#include <xrpl/protocol/jss.h>
#include <xrpl/rpc/Context.h>
#include <xrpl/rpc/Role.h>
#include <xrpl/rpc/Status.h>
#include <xrpl/rpc/impl/Handler.h>

namespace Json {
class Object;
}

namespace xrpl {
namespace RPC {

struct JsonContext;

// ledger [id|index|current|closed] [full]
// {
//    ledger: 'current' | 'closed' | <uint256> | <number>,  // optional
//    full: true | false    // optional, defaults to false.
// }

class LedgerHandler
{
public:
    explicit LedgerHandler(JsonContext&);

    Status
    check();

    template <class Object>
    void
    writeResult(Object&);

    static char const*
    name()
    {
        return "ledger";
    }

    static Role
    role()
    {
        return Role::USER;
    }

    static Condition
    condition()
    {
        return NO_CONDITION;
    }

private:
    JsonContext& context_;
    std::shared_ptr<ReadView const> ledger_;
    std::vector<TxQ::TxDetails> queueTxs_;
    Json::Value result_;
    int options_ = 0;
    LedgerEntryType type_;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Implementation.

template <class Object>
void
LedgerHandler::writeResult(Object& value)
{
    if (ledger_)
    {
        Json::copyFrom(value, result_);
        addJson(value, {*ledger_, &context_, options_, queueTxs_, type_});
    }
    else
    {
        auto& master = context_.app.getLedgerMaster();
        {
            auto&& closed = Json::addObject(value, jss::closed);
            addJson(closed, {*master.getClosedLedger(), &context_, 0});
        }
        {
            auto&& open = Json::addObject(value, jss::open);
            addJson(open, {*master.getCurrentLedger(), &context_, 0});
        }
    }
}

}  // namespace RPC
}  // namespace xrpl

#endif
