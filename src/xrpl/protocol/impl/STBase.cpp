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

#include <xrpl/protocol/STBase.h>
#include <boost/checked_delete.hpp>
#include <cassert>
#include <memory>

namespace ripple {

STBase::STBase() : fName(&sfGeneric)
{
}

STBase::STBase(SField const& n) : fName(&n)
{
    assert(fName);
}

STBase&
STBase::operator=(const STBase& t)
{
    if (!fName->isUseful())
        fName = t.fName;
    return *this;
}

bool
STBase::operator==(const STBase& t) const
{
    return (getSType() == t.getSType()) && isEquivalent(t);
}

bool
STBase::operator!=(const STBase& t) const
{
    return (getSType() != t.getSType()) || !isEquivalent(t);
}

SerializedTypeID
STBase::getSType() const
{
    return STI_NOTPRESENT;
}

std::string
STBase::getFullText() const
{
    std::string ret;

    if (getSType() != STI_NOTPRESENT)
    {
        if (fName->hasName())
        {
            ret = fName->fieldName;
            ret += " = ";
        }

        ret += getText();
    }

    return ret;
}

std::string
STBase::getText() const
{
    return std::string();
}

Json::Value STBase::getJson(JsonOptions /*options*/) const
{
    return getText();
}

void
STBase::add(Serializer& s) const
{
    // Should never be called
    assert(false);
}

bool
STBase::isEquivalent(const STBase& t) const
{
    assert(getSType() == STI_NOTPRESENT);
    return t.getSType() == STI_NOTPRESENT;
}

bool
STBase::isDefault() const
{
    return true;
}

void
STBase::setFName(SField const& n)
{
    fName = &n;
    assert(fName);
}

SField const&
STBase::getFName() const
{
    return *fName;
}

void
STBase::addFieldID(Serializer& s) const
{
    assert(fName->isBinary());
    s.addFieldID(fName->fieldType, fName->fieldValue);
}

//------------------------------------------------------------------------------

std::ostream&
operator<<(std::ostream& out, const STBase& t)
{
    return out << t.getFullText();
}

}  // namespace ripple
