//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/xrplf/xrpld
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

#ifndef RIPPLE_BASICS_FILEUTILITIES_H_INCLUDED
#define RIPPLE_BASICS_FILEUTILITIES_H_INCLUDED

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>

#include <optional>

namespace ripple {

std::string
getFileContents(
    boost::system::error_code& ec,
    boost::filesystem::path const& sourcePath,
    std::optional<std::size_t> maxSize = std::nullopt);

void
writeFileContents(
    boost::system::error_code& ec,
    boost::filesystem::path const& destPath,
    std::string const& contents);

}  // namespace ripple

#endif
