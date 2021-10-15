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

#ifndef XRPL_TEST_JTX_OFFER_H_INCLUDED
#define XRPL_TEST_JTX_OFFER_H_INCLUDED

#include <xrpl/json/json_value.h>
#include <xrpl/protocol/STAmount.h>
#include <test/jtx/Account.h>

namespace xrpl {
namespace test {
namespace jtx {

/** Create an offer. */
Json::Value
offer(
    Account const& account,
    STAmount const& in,
    STAmount const& out,
    std::uint32_t flags = 0);

/** Cancel an offer. */
Json::Value
offer_cancel(Account const& account, std::uint32_t offerSeq);

}  // namespace jtx
}  // namespace test
}  // namespace xrpl

#endif
