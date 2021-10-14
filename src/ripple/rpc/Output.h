//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/xrplf/xrpld
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

#ifndef RIPPLE_RPC_OUTPUT_H_INCLUDED
#define RIPPLE_RPC_OUTPUT_H_INCLUDED

#include <boost/utility/string_ref.hpp>

namespace ripple {
namespace RPC {

using Output = std::function<void(boost::string_ref const&)>;

inline Output
stringOutput(std::string& s)
{
    return [&](boost::string_ref const& b) { s.append(b.data(), b.size()); };
}

}  // namespace RPC
}  // namespace ripple

#endif
