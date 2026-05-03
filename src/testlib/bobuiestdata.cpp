// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUICore/qmetaobject.h>

#include <BobUITest/bobuiestassert.h>
#include <BobUITest/bobuiestdata.h>
#include <BobUITest/private/bobuiesttable_p.h>

#include <string.h>
#include <stdlib.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIestDataPrivate
{
public:
    char *tag = nullptr;
    BOBUIestTable *parent = nullptr;
    void **data = nullptr;
    int dataCount = 0;
};

BOBUIestData::BOBUIestData(const char *tag, BOBUIestTable *parent)
{
    BOBUIEST_ASSERT(tag);
    BOBUIEST_ASSERT(parent);
    d = new BOBUIestDataPrivate;
    d->tag = qstrdup(tag);
    d->parent = parent;
    d->data = new void *[parent->elementCount()];
    memset(d->data, 0, parent->elementCount() * sizeof(void*));
}

BOBUIestData::~BOBUIestData()
{
    for (int i = 0; i < d->dataCount; ++i) {
        if (d->data[i])
            QMetaType(d->parent->elementTypeId(i)).destroy(d->data[i]);
    }
    delete [] d->data;
    delete [] d->tag;
    delete d;
}

void BOBUIestData::append(int type, const void *data)
{
    BOBUIEST_ASSERT(d->dataCount < d->parent->elementCount());
    int expectedType = d->parent->elementTypeId(d->dataCount);
    int dd = 0;
    if constexpr (sizeof(qsizetype) == 8) {
        // Compatibility with BobUI 5. Passing a qsizetype to a test function expecting
        // an int will work. This is required, as methods returning a qsizetype in BobUI 6
        // used to return an int in BobUI 5.
        if (type == QMetaType::LongLong && expectedType == QMetaType::Int) {
            qlonglong d = *static_cast<const qlonglong *>(data);
            if (d >= std::numeric_limits<int>::min() && d <= std::numeric_limits<int>::max()) {
                dd = d;
                data = &dd;
                type = QMetaType::Int;
            }
        }
    }
    if (expectedType != type) {
        qDebug("expected data of type '%s', got '%s' for element %d of data with tag '%s'",
                QMetaType(expectedType).name(),
                QMetaType(type).name(),
                d->dataCount, d->tag);
        BOBUIEST_ASSERT(false);
    }
    d->data[d->dataCount] = QMetaType(type).create(data);
    ++d->dataCount;
}

void *BOBUIestData::data(int index) const
{
    BOBUIEST_ASSERT(index >= 0);
    BOBUIEST_ASSERT(index < d->parent->elementCount());
    return d->data[index];
}

BOBUIestTable *BOBUIestData::parent() const
{
    return d->parent;
}

const char *BOBUIestData::dataTag() const
{
    return d->tag;
}

int BOBUIestData::dataCount() const
{
    return d->dataCount;
}

BOBUI_END_NAMESPACE
