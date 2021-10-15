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

#include <xrpl/app/misc/ValidatorKeys.h>

#include <xrpl/app/misc/Manifest.h>
#include <xrpl/basics/Log.h>
#include <xrpl/basics/base64.h>
#include <xrpl/core/Config.h>
#include <xrpl/core/ConfigSections.h>

namespace xrpl {
ValidatorKeys::ValidatorKeys(Config const& config, beast::Journal j)
{
    if (config.exists(SECTION_VALIDATOR_TOKEN) &&
        config.exists(SECTION_VALIDATION_SEED))
    {
        configInvalid_ = true;
        JLOG(j.fatal()) << "Cannot specify both [" SECTION_VALIDATION_SEED
                           "] and [" SECTION_VALIDATOR_TOKEN "]";
        return;
    }

    if (config.exists(SECTION_VALIDATOR_TOKEN))
    {
        // token is non-const so it can be moved from
        if (auto token = loadValidatorToken(
                config.section(SECTION_VALIDATOR_TOKEN).lines()))
        {
            auto const pk =
                derivePublicKey(KeyType::secp256k1, token->validationSecret);
            auto const m = deserializeManifest(base64_decode(token->manifest));

            if (!m || pk != m->signingKey)
            {
                configInvalid_ = true;
                JLOG(j.fatal())
                    << "Invalid token specified in [" SECTION_VALIDATOR_TOKEN
                       "]";
            }
            else
            {
                secretKey = token->validationSecret;
                publicKey = pk;
                masterPublicKey = m->masterKey;
                nodeID = calcNodeID(m->masterKey);
                sequence = m->sequence;
                manifest = std::move(token->manifest);
            }
        }
        else
        {
            configInvalid_ = true;
            JLOG(j.fatal())
                << "Invalid token specified in [" SECTION_VALIDATOR_TOKEN "]";
        }
    }
    else if (config.exists(SECTION_VALIDATION_SEED))
    {
        auto const seed = parseBase58<Seed>(
            config.section(SECTION_VALIDATION_SEED).lines().front());
        if (!seed)
        {
            configInvalid_ = true;
            JLOG(j.fatal())
                << "Invalid seed specified in [" SECTION_VALIDATION_SEED "]";
        }
        else
        {
            secretKey = generateSecretKey(KeyType::secp256k1, *seed);
            publicKey = derivePublicKey(KeyType::secp256k1, secretKey);
            masterPublicKey = publicKey;
            nodeID = calcNodeID(publicKey);
            sequence = 0;
        }
    }
}
}  // namespace xrpl
