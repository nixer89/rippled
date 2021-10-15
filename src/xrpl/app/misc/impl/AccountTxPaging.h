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

#ifndef XRPL_APP_MISC_IMPL_ACCOUNTTXPAGING_H_INCLUDED
#define XRPL_APP_MISC_IMPL_ACCOUNTTXPAGING_H_INCLUDED

#include <xrpl/app/rdb/RelationalDBInterface.h>
#include <cstdint>
#include <string>
#include <utility>

//------------------------------------------------------------------------------

namespace xrpl {

void
convertBlobsToTxResult(
    RelationalDBInterface::AccountTxs& to,
    std::uint32_t ledger_index,
    std::string const& status,
    Blob const& rawTxn,
    Blob const& rawMeta,
    Application& app);

void
saveLedgerAsync(Application& app, std::uint32_t seq);

}  // namespace xrpl

#endif
