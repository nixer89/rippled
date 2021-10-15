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

#ifndef XRPL_SHAMAP_SHAMAPTXLEAFNODE_H_INCLUDED
#define XRPL_SHAMAP_SHAMAPTXLEAFNODE_H_INCLUDED

#include <xrpl/basics/CountedObject.h>
#include <xrpl/protocol/HashPrefix.h>
#include <xrpl/protocol/digest.h>
#include <xrpl/shamap/SHAMapItem.h>
#include <xrpl/shamap/SHAMapLeafNode.h>
#include <xrpl/shamap/SHAMapNodeID.h>

namespace xrpl {

/** A leaf node for a transaction. No metadata is included. */
class SHAMapTxLeafNode final : public SHAMapLeafNode,
                               public CountedObject<SHAMapTxLeafNode>
{
public:
    SHAMapTxLeafNode(
        std::shared_ptr<SHAMapItem const> item,
        std::uint32_t cowid)
        : SHAMapLeafNode(std::move(item), cowid)
    {
        updateHash();
    }

    SHAMapTxLeafNode(
        std::shared_ptr<SHAMapItem const> item,
        std::uint32_t cowid,
        SHAMapHash const& hash)
        : SHAMapLeafNode(std::move(item), cowid, hash)
    {
    }

    std::shared_ptr<SHAMapTreeNode>
    clone(std::uint32_t cowid) const final override
    {
        return std::make_shared<SHAMapTxLeafNode>(item_, cowid, hash_);
    }

    SHAMapNodeType
    getType() const final override
    {
        return SHAMapNodeType::tnTRANSACTION_NM;
    }

    void
    updateHash() final override
    {
        hash_ =
            SHAMapHash{sha512Half(HashPrefix::transactionID, item_->slice())};
    }

    void
    serializeForWire(Serializer& s) const final override
    {
        s.addRaw(item_->slice());
        s.add8(wireTypeTransaction);
    }

    void
    serializeWithPrefix(Serializer& s) const final override
    {
        s.add32(HashPrefix::transactionID);
        s.addRaw(item_->slice());
    }
};

}  // namespace xrpl

#endif
