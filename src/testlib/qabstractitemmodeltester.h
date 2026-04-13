// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QABSTRACTITEMMODELTESTER_H
#define QABSTRACTITEMMODELTESTER_H

#include <BobUICore/QObject>
#include <BobUITest/bobuitestglobal.h>
#include <BobUICore/QAbstractItemModel>
#include <BobUICore/QVariant>

#ifdef BOBUI_GUI_LIB
#include <BobUIGui/QFont>
#include <BobUIGui/QColor>
#include <BobUIGui/QBrush>
#include <BobUIGui/QPixmap>
#include <BobUIGui/QImage>
#include <BobUIGui/QIcon>
#endif

BOBUI_REQUIRE_CONFIG(itemmodeltester);

BOBUI_BEGIN_NAMESPACE

class QAbstractItemModel;
class QAbstractItemModelTester;
class QAbstractItemModelTesterPrivate;

namespace BOBUIestPrivate {
inline bool testDataGuiRoles(QAbstractItemModelTester *tester);
}

class Q_TESTLIB_EXPORT QAbstractItemModelTester : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAbstractItemModelTester)

public:
    enum class FailureReportingMode {
        BobUITest,
        Warning,
        Fatal
    };

    QAbstractItemModelTester(QAbstractItemModel *model, QObject *parent = nullptr);
    QAbstractItemModelTester(QAbstractItemModel *model, FailureReportingMode mode, QObject *parent = nullptr);

    QAbstractItemModel *model() const;
    FailureReportingMode failureReportingMode() const;
    void setUseFetchMore(bool value);

private:
    bool verify(bool statement, const char *statementStr, const char *description, const char *file, int line);
};

BOBUI_END_NAMESPACE

#endif // QABSTRACTITEMMODELTESTER_H
