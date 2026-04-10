// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTCOREELEMENT_P_H
#define BOBUIESTCOREELEMENT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/private/bobuiestelementattribute_p.h>

#include <vector>

BOBUI_BEGIN_NAMESPACE


template <class ElementType>
class BOBUIestCoreElement
{
    public:
        BOBUIestCoreElement(BOBUIest::LogElementType type = BOBUIest::LET_Undefined);
        virtual ~BOBUIestCoreElement();

        void addAttribute(const BOBUIest::AttributeIndex index, const char *value);
        const std::vector<BOBUIestElementAttribute*> &attributes() const;
        const char *attributeValue(BOBUIest::AttributeIndex index) const;
        const char *attributeName(BOBUIest::AttributeIndex index) const;
        const BOBUIestElementAttribute *attribute(BOBUIest::AttributeIndex index) const;

        const char *elementName() const;
        BOBUIest::LogElementType elementType() const;

    private:
        std::vector<BOBUIestElementAttribute*> listOfAttributes;
        BOBUIest::LogElementType type;
};

template<class ElementType>
BOBUIestCoreElement<ElementType>::BOBUIestCoreElement(BOBUIest::LogElementType t)
    : type(t)
{
}

template<class ElementType>
BOBUIestCoreElement<ElementType>::~BOBUIestCoreElement()
{
    for (auto *attribute : listOfAttributes)
        delete attribute;
}

template <class ElementType>
void BOBUIestCoreElement<ElementType>::addAttribute(const BOBUIest::AttributeIndex attributeIndex, const char *value)
{
    if (attributeIndex == -1 || attribute(attributeIndex))
        return;

    BOBUIestElementAttribute *testAttribute = new BOBUIestElementAttribute;
    testAttribute->setPair(attributeIndex, value);
    listOfAttributes.push_back(testAttribute);
}

template <class ElementType>
const std::vector<BOBUIestElementAttribute*> &BOBUIestCoreElement<ElementType>::attributes() const
{
    return listOfAttributes;
}

template <class ElementType>
const char *BOBUIestCoreElement<ElementType>::attributeValue(BOBUIest::AttributeIndex index) const
{
    const BOBUIestElementAttribute *attrb = attribute(index);
    if (attrb)
        return attrb->value();

    return nullptr;
}

template <class ElementType>
const char *BOBUIestCoreElement<ElementType>::attributeName(BOBUIest::AttributeIndex index) const
{
    const BOBUIestElementAttribute *attrb = attribute(index);
    if (attrb)
        return attrb->name();

    return nullptr;
}

template <class ElementType>
const char *BOBUIestCoreElement<ElementType>::elementName() const
{
    const char *xmlElementNames[] =
    {
        "property",
        "properties",
        "failure",
        "error",
        "testcase",
        "testsuite",
        "message",
        "system-err",
        "system-out",
        "skipped"
    };

    if (type != BOBUIest::LET_Undefined)
        return xmlElementNames[type];

    return nullptr;
}

template <class ElementType>
BOBUIest::LogElementType BOBUIestCoreElement<ElementType>::elementType() const
{
    return type;
}

template <class ElementType>
const BOBUIestElementAttribute *BOBUIestCoreElement<ElementType>::attribute(BOBUIest::AttributeIndex index) const
{
    for (auto *attribute : listOfAttributes) {
        if (attribute->index() == index)
            return attribute;
    }

    return nullptr;
}

BOBUI_END_NAMESPACE

#endif
