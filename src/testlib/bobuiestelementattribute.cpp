// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/private/bobuiestelementattribute_p.h>
#include <BobUICore/qbytearray.h>
#include <string.h>
#include <stdlib.h>

BOBUI_BEGIN_NAMESPACE

BOBUIestElementAttribute::BOBUIestElementAttribute() = default;

BOBUIestElementAttribute::~BOBUIestElementAttribute()
{
    delete[] attributeValue;
}

const char *BOBUIestElementAttribute::value() const
{
    return attributeValue;
}

const char *BOBUIestElementAttribute::name() const
{
    const char *AttributeNames[] =
    {
        "name",
        "tests",
        "failures",
        "errors",
        "type",
        "message",
        "value",
        "value",
        "time",
        "timestamp",
        "hostname",
        "classname",
        "skipped"
    };

    if (attributeIndex != BOBUIest::AI_Undefined)
        return AttributeNames[attributeIndex];

    return nullptr;
}

BOBUIest::AttributeIndex BOBUIestElementAttribute::index() const
{
    return attributeIndex;
}

bool BOBUIestElementAttribute::isNull() const
{
    return attributeIndex == BOBUIest::AI_Undefined;
}

bool BOBUIestElementAttribute::setPair(BOBUIest::AttributeIndex index, const char *value)
{
    if (!value)
        return false;

    delete[] attributeValue;

    attributeIndex = index;
    attributeValue = qstrdup(value);

    return attributeValue != nullptr;
}

BOBUI_END_NAMESPACE

