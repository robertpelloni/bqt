// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/private/bobuiestelement_p.h>

BOBUI_BEGIN_NAMESPACE

BOBUIestElement::BOBUIestElement(BOBUIest::LogElementType type)
    : BOBUIestCoreElement<BOBUIestElement>(type)
{
}

BOBUIestElement::~BOBUIestElement()
{
    for (auto *child : listOfChildren)
        delete child;
}

bool BOBUIestElement::addChild(BOBUIestElement *element)
{
    if (!element)
        return false;

    if (element->elementType() != BOBUIest::LET_Undefined) {
        listOfChildren.push_back(element);
        element->setParent(this);
        return true;
    }

    return false;
}

const std::vector<BOBUIestElement*> &BOBUIestElement::childElements() const
{
    return listOfChildren;
}

const BOBUIestElement *BOBUIestElement::parentElement() const
{
    return parent;
}

void BOBUIestElement::setParent(const BOBUIestElement *p)
{
    parent = p;
}

BOBUI_END_NAMESPACE

