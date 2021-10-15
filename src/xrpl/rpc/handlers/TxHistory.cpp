//------------------------------------------------------------------------------
/*
    This file is part of xrpld: https://github.com/xrplf/xrpld
    Copyright (c) 2012-2014 XRP Ledger Foundation

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
#include <xrpl/app/misc/Transaction.h>
#include <xrpl/app/rdb/RelationalDBInterface.h>
#include <xrpl/core/DatabaseCon.h>
#include <xrpl/core/Pg.h>
#include <xrpl/core/SociDB.h>
#include <xrpl/net/RPCErr.h>
#include <xrpl/protocol/ErrorCodes.h>
#include <xrpl/protocol/jss.h>
#include <xrpl/resource/Fees.h>
#include <xrpl/rpc/Context.h>
#include <xrpl/rpc/Role.h>
#include <xrpl/rpc/Status.h>
#include <boost/format.hpp>

namespace xrpl {

// {
//   start: <index>
// }
Json::Value
doTxHistory(RPC::JsonContext& context)
{
    if (!context.app.config().useTxTables())
        return rpcError(rpcNOT_ENABLED);

    context.loadType = Resource::feeMediumBurdenRPC;

    if (!context.params.isMember(jss::start))
        return rpcError(rpcINVALID_PARAMS);

    unsigned int startIndex = context.params[jss::start].asUInt();

    if ((startIndex > 10000) && (!isUnlimited(context.role)))
        return rpcError(rpcNO_PERMISSION);

    auto trans =
        context.app.getRelationalDBInterface().getTxHistory(startIndex);

    Json::Value obj;
    Json::Value& txs = obj[jss::txs];
    obj[jss::index] = startIndex;
    if (context.app.config().reporting())
        obj["used_postgres"] = true;

    for (auto const& t : trans)
        txs.append(t->getJson(JsonOptions::none));

    return obj;
}

}  // namespace xrpl
