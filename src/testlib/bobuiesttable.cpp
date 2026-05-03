// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/private/bobuiesttable_p.h>
#include <BobUITest/bobuiestdata.h>
#include <BobUITest/bobuiestassert.h>

#include <BobUICore/private/qduplicatetracker_p.h>
#include <BobUICore/qmetaobject.h>

#include <string.h>
#include <vector>
#include <algorithm>

BOBUI_BEGIN_NAMESPACE

class BOBUIestTablePrivate
{
public:
    ~BOBUIestTablePrivate()
    {
        qDeleteAll(dataList.begin(), dataList.end());
    }

    struct Element {
        Element() = default;
        Element(const char *n, int t) : name(n), type(t) {}

        const char *name = nullptr;
        int type = 0;
    };

    using ElementList = std::vector<Element>;
    ElementList elementList;

    using DataList = std::vector<BOBUIestData *>;
    DataList dataList;

    using TagSet = QDuplicateTracker<std::string>;
    TagSet tagSet;

    void addColumn(int elemType, const char *elemName) { elementList.push_back(Element(elemName, elemType)); }
    void addRow(BOBUIestData *data) { dataList.push_back(data); }

    static BOBUIestTable *currentTestTable;
    static BOBUIestTable *gTable;
};

BOBUIestTable *BOBUIestTablePrivate::currentTestTable = nullptr;
BOBUIestTable *BOBUIestTablePrivate::gTable = nullptr;

void BOBUIestTable::addColumn(int type, const char *name)
{
    BOBUIEST_ASSERT(type);
    BOBUIEST_ASSERT(name);
    if (indexOf(name) != -1)
        qWarning() << "Duplicate data column" << name << "- please rename.";

    d->addColumn(type, name);
}

int BOBUIestTable::elementCount() const
{
    return int(d->elementList.size());
}

int BOBUIestTable::dataCount() const
{
    return int(d->dataList.size());
}

bool BOBUIestTable::isEmpty() const
{
    return d->elementList.empty();
}

BOBUIestData *BOBUIestTable::newData(const char *tag)
{
    BOBUIEST_ASSERT(tag);
    if (d->tagSet.hasSeen(tag))
        qWarning("Duplicate data tag \"%s\" - please rename.", tag);

    BOBUIestData *dt = new BOBUIestData(tag, this);
    d->addRow(dt);
    return dt;
}

BOBUIestTable::BOBUIestTable()
{
    d = new BOBUIestTablePrivate;
    BOBUIestTablePrivate::currentTestTable = this;
}

BOBUIestTable::~BOBUIestTable()
{
    BOBUIestTablePrivate::currentTestTable = nullptr;
    delete d;
}

int BOBUIestTable::elementTypeId(int index) const
{
    return size_t(index) < d->elementList.size() ? d->elementList[index].type : -1;
}

const char *BOBUIestTable::dataTag(int index) const
{
    return size_t(index) < d->elementList.size() ? d->elementList[index].name : nullptr;
}

BOBUIestData *BOBUIestTable::testData(int index) const
{
    return size_t(index) < d->dataList.size() ? d->dataList[index] : nullptr;
}

class NamePredicate
{
public:
    explicit NamePredicate(const char *needle) : m_needle(needle) {}

    bool operator()(const BOBUIestTablePrivate::Element &e) const
        { return !strcmp(e.name, m_needle); }

    bool operator()(const BOBUIestData *e) const
        { return !strcmp(e->dataTag(), m_needle); }

private:
    const char *m_needle;
};

int BOBUIestTable::indexOf(const char *elementName) const
{
    BOBUIEST_ASSERT(elementName);

    const BOBUIestTablePrivate::ElementList &elementList = d->elementList;

    const auto it = std::find_if(elementList.begin(), elementList.end(),
                                 NamePredicate(elementName));
    return it != elementList.end() ?
        int(it - elementList.begin()) : -1;
}

BOBUIestTable *BOBUIestTable::globalTestTable()
{
    if (!BOBUIestTablePrivate::gTable)
        BOBUIestTablePrivate::gTable = new BOBUIestTable();
    return BOBUIestTablePrivate::gTable;
}

void BOBUIestTable::clearGlobalTestTable()
{
    delete BOBUIestTablePrivate::gTable;
    BOBUIestTablePrivate::gTable = nullptr;
}

BOBUIestTable *BOBUIestTable::currentTestTable()
{
    return BOBUIestTablePrivate::currentTestTable;
}

BOBUI_END_NAMESPACE
