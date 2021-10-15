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

#include <xrpl/app/main/Application.h>
#include <xrpl/ledger/ReadView.h>
#include <xrpl/protocol/jss.h>
#include <xrpl/rpc/Context.h>
#include <xrpl/rpc/impl/RPCHelpers.h>

namespace xrpl {

// {
//   ledger_hash : <ledger>,
//   ledger_index : <ledger_index>
// }
//
// XXX In this case, not specify either ledger does not mean ledger current. It
// means any ledger.
Json::Value
doTransactionEntry(RPC::JsonContext& context)
{
    std::shared_ptr<ReadView const> lpLedger;
    Json::Value jvResult = RPC::lookupLedger(lpLedger, context);

    if (!lpLedger)
        return jvResult;

    if (!context.params.isMember(jss::tx_hash))
    {
        jvResult[jss::error] = "fieldNotFoundTransaction";
    }
    else if (jvResult.get(jss::ledger_hash, Json::nullValue).isNull())
    {
        // We don't work on ledger current.

        // XXX We don't support any transaction yet.
        jvResult[jss::error] = "notYetImplemented";
    }
    else
    {
        uint256 uTransID;
        // XXX Relying on trusted WSS client. Would be better to have a strict
        // routine, returning success or failure.
        if (!uTransID.parseHex(context.params[jss::tx_hash].asString()))
        {
            jvResult[jss::error] = "malformedRequest";
            return jvResult;
        }

        auto [sttx, stobj] = lpLedger->txRead(uTransID);
        if (!sttx)
        {
            jvResult[jss::error] = "transactionNotFound";
        }
        else
        {
            jvResult[jss::tx_json] = sttx->getJson(JsonOptions::none);
            if (stobj)
                jvResult[jss::metadata] = stobj->getJson(JsonOptions::none);
            // 'accounts'
            // 'engine_...'
            // 'ledger_...'
        }
    }

    return jvResult;
}

}  // namespace xrpl
