//------------------------------------------------------------------------------
/*
    This file is part of xrpld: https://github.com/xrplf/xrpld
    Copyright (c) 2018 XRP Ledger Foundation

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
#include <xrpl/basics/RangeSet.h>
#include <xrpl/net/RPCErr.h>
#include <xrpl/nodestore/DatabaseShard.h>
#include <xrpl/overlay/Overlay.h>
#include <xrpl/protocol/ErrorCodes.h>
#include <xrpl/protocol/jss.h>
#include <xrpl/resource/Fees.h>
#include <xrpl/rpc/Context.h>

namespace ripple {

/** RPC command that reports stored shards by nodes.
    {
        // Determines if the result includes node public key.
        // optional, default is false
        public_key: <bool>

        // The maximum number of peer hops to attempt.
        // optional, default is zero, maximum is 3
        limit: <integer>
    }
*/
Json::Value
doCrawlShards(RPC::JsonContext& context)
{
    if (context.app.config().reporting())
        return rpcError(rpcREPORTING_UNSUPPORTED);

    if (context.role != Role::ADMIN)
        return rpcError(rpcNO_PERMISSION);

    std::uint32_t relays{0};
    if (auto const& jv = context.params[jss::limit])
    {
        if (!(jv.isUInt() || (jv.isInt() && jv.asInt() >= 0)))
            return RPC::expected_field_error(jss::limit, "unsigned integer");
        relays = std::min(jv.asUInt(), relayLimit);
        context.loadType = Resource::feeHighBurdenRPC;
    }
    else
        context.loadType = Resource::feeMediumBurdenRPC;

    // Collect shard info from server and peers
    bool const includePublicKey{
        context.params.isMember(jss::public_key) &&
        context.params[jss::public_key].asBool()};
    Json::Value jvResult{
        context.app.overlay().crawlShards(includePublicKey, relays)};

    return jvResult;
}

}  // namespace ripple
