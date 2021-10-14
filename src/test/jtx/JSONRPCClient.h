//------------------------------------------------------------------------------
/*
    This file is part of xrpld: https://github.com/xrplf/xrpld
    Copyright (c) 2016 XRP Ledger Foundation

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

#ifndef RIPPLE_TEST_HTTPCLIENT_H_INCLUDED
#define RIPPLE_TEST_HTTPCLIENT_H_INCLUDED

#include <xrpl/core/Config.h>
#include <memory>
#include <test/jtx/AbstractClient.h>

namespace ripple {
namespace test {

/** Returns a client using JSON-RPC over HTTP/S. */
std::unique_ptr<AbstractClient>
makeJSONRPCClient(Config const& cfg, unsigned rpc_version = 2);

}  // namespace test
}  // namespace ripple

#endif
