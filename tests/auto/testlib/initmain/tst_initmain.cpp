// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BobUICore/qxptype_traits.h>
#include <BOBUIest>

class tst_InitMain : public QObject
{
    Q_OBJECT

public:
    static void initMain() { m_initMainCalled = true; }

private slots:
    void testcase();

private:
    static bool m_initMainCalled;
};

static_assert(BOBUIest::Internal::hasInitMain<tst_InitMain>);
static_assert(!BOBUIest::Internal::hasInitMain<QObject>);

bool tst_InitMain::m_initMainCalled = false;

void tst_InitMain::testcase()
{
    QVERIFY(m_initMainCalled);
}

BOBUIEST_MAIN(tst_InitMain)

#include "tst_initmain.moc"
