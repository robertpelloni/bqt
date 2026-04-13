// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTTABLE_P_H
#define BOBUIESTTABLE_P_H

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
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIestData;
class BOBUIestTablePrivate;

class Q_TESTLIB_EXPORT BOBUIestTable
{
public:
    BOBUIestTable();
    ~BOBUIestTable();

    void addColumn(int elementType, const char *elementName);
    BOBUIestData *newData(const char *tag);

    int elementCount() const;
    int dataCount() const;

    int elementTypeId(int index) const;
    const char *dataTag(int index) const;
    int indexOf(const char *elementName) const;
    bool isEmpty() const;
    BOBUIestData *testData(int index) const;

    static BOBUIestTable *globalTestTable();
    static BOBUIestTable *currentTestTable();
    static void clearGlobalTestTable();

private:
    Q_DISABLE_COPY(BOBUIestTable)

    BOBUIestTablePrivate *d;
};

BOBUI_END_NAMESPACE

#endif
