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

#ifndef XRPL_PROTOCOL_SYSTEMPARAMETERS_H_INCLUDED
#define XRPL_PROTOCOL_SYSTEMPARAMETERS_H_INCLUDED

#include <xrpl/basics/XRPAmount.h>
#include <xrpl/basics/chrono.h>
#include <cstdint>
#include <string>

namespace xrpl {

// Various protocol and system specific constant globals.

/* The name of the system. */
static inline std::string const&
systemName()
{
    static std::string const name = "xrpl";
    return name;
}

/** Configure the native currency. */

/** Number of drops in the genesis account. */
constexpr XRPAmount INITIAL_XRP{100'000'000'000 * DROPS_PER_XRP};

/** Returns true if the amount does not exceed the initial XRP in existence. */
inline bool
isLegalAmount(XRPAmount const& amount)
{
    return amount <= INITIAL_XRP;
}

/* The currency code for the native currency. */
static inline std::string const&
systemCurrencyCode()
{
    static std::string const code = "XRP";
    return code;
}

/** The XRP ledger network's earliest allowed sequence */
static constexpr std::uint32_t XRP_LEDGER_EARLIEST_SEQ{32570u};

/** The number of ledgers in a shard */
static constexpr std::uint32_t DEFAULT_LEDGERS_PER_SHARD{16384u};

/** The minimum amount of support an amendment should have.

    @note This value is used by legacy code and will become obsolete
          once the fixAmendmentMajorityCalc amendment activates.
*/
constexpr std::ratio<204, 256> preFixAmendmentMajorityCalcThreshold;

constexpr std::ratio<80, 100> postFixAmendmentMajorityCalcThreshold;

/** The minimum amount of time an amendment must hold a majority */
constexpr std::chrono::seconds const defaultAmendmentMajorityTime = weeks{2};

}  // namespace xrpl

/** Default peer port (IANA registered) */
inline std::uint16_t constexpr DEFAULT_PEER_PORT{2459};

#endif
