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
#include <xrpl/json/json_writer.h>
#include <xrpl/ledger/ReadView.h>
#include <xrpl/net/RPCErr.h>
#include <xrpl/protocol/ErrorCodes.h>
#include <xrpl/protocol/Indexes.h>
#include <xrpl/protocol/LedgerFormats.h>
#include <xrpl/protocol/jss.h>
#include <xrpl/resource/Fees.h>
#include <xrpl/rpc/Context.h>
#include <xrpl/rpc/impl/RPCHelpers.h>
#include <xrpl/rpc/impl/Tuning.h>

#include <sstream>
#include <string>

namespace xrpl {

/** General RPC command that can retrieve objects in the account root.
    {
      account: <account>|<account_public_key>
      ledger_hash: <string> // optional
      ledger_index: <string | unsigned integer> // optional
      type: <string> // optional, defaults to all account objects types
      limit: <integer> // optional
      marker: <opaque> // optional, resume previous query
    }
*/

Json::Value
doAccountObjects(RPC::JsonContext& context)
{
    auto const& params = context.params;
    if (!params.isMember(jss::account))
        return RPC::missing_field_error(jss::account);

    std::shared_ptr<ReadView const> ledger;
    auto result = RPC::lookupLedger(ledger, context);
    if (ledger == nullptr)
        return result;

    AccountID accountID;
    {
        auto const strIdent = params[jss::account].asString();
        if (auto jv = RPC::accountFromString(accountID, strIdent))
        {
            for (auto it = jv.begin(); it != jv.end(); ++it)
                result[it.memberName()] = *it;

            return result;
        }
    }

    if (!ledger->exists(keylet::account(accountID)))
        return rpcError(rpcACT_NOT_FOUND);

    std::optional<std::vector<LedgerEntryType>> typeFilter;

    if (params.isMember(jss::deletion_blockers_only) &&
        params[jss::deletion_blockers_only].asBool())
    {
        struct
        {
            Json::StaticString name;
            LedgerEntryType type;
        } static constexpr deletionBlockers[] = {
            {jss::check, ltCHECK},
            {jss::escrow, ltESCROW},
            {jss::payment_channel, ltPAYCHAN},
            {jss::state, ltRIPPLE_STATE}};

        typeFilter.emplace();
        typeFilter->reserve(std::size(deletionBlockers));

        for (auto [name, type] : deletionBlockers)
        {
            if (params.isMember(jss::type) && name != params[jss::type])
            {
                continue;
            }

            typeFilter->push_back(type);
        }
    }
    else
    {
        auto [rpcStatus, type] = RPC::chooseLedgerEntryType(params);

        if (rpcStatus)
        {
            result.clear();
            rpcStatus.inject(result);
            return result;
        }
        else if (type != ltANY)
        {
            typeFilter = std::vector<LedgerEntryType>({type});
        }
    }

    unsigned int limit;
    if (auto err = readLimitField(limit, RPC::Tuning::accountObjects, context))
        return *err;

    uint256 dirIndex;
    uint256 entryIndex;
    if (params.isMember(jss::marker))
    {
        auto const& marker = params[jss::marker];
        if (!marker.isString())
            return RPC::expected_field_error(jss::marker, "string");

        std::stringstream ss(marker.asString());
        std::string s;
        if (!std::getline(ss, s, ','))
            return RPC::invalid_field_error(jss::marker);

        if (!dirIndex.parseHex(s))
            return RPC::invalid_field_error(jss::marker);

        if (!std::getline(ss, s, ','))
            return RPC::invalid_field_error(jss::marker);

        if (!entryIndex.parseHex(s))
            return RPC::invalid_field_error(jss::marker);
    }

    if (!RPC::getAccountObjects(
            *ledger,
            accountID,
            typeFilter,
            dirIndex,
            entryIndex,
            limit,
            result))
    {
        result[jss::account_objects] = Json::arrayValue;
    }

    result[jss::account] = context.app.accountIDCache().toBase58(accountID);
    context.loadType = Resource::feeMediumBurdenRPC;
    return result;
}

}  // namespace xrpl
