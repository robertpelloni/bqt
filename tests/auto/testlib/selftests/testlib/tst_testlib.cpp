// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/BobUIMath>
#include <BobUICore/BobUINumeric>

/* Test BOBUIest functions not covered by other parts of the selftest.  Tests that
 * involve crashing or exiting should be added as separate tests in their own
 * right.  Tests that form a coherent group on a related theme should also go in
 * their own directory.  Tests that fail in order to exercise BOBUIest internals
 * are fine.
 */

class tst_TestLib : public QObject
{
    Q_OBJECT

public:
    tst_TestLib();

private slots:
    void basics() const;
    void delays() const;
    void reals_data() const;
    void reals() const;
};

tst_TestLib::tst_TestLib()
{
    // Set object name, so that it's printed out when some comparison fails.
    // Othewise object address will be printed, which will not allow
    // tst_sefltest to compare the output with expected.
    setObjectName("TestObject");
}

void tst_TestLib::basics() const
{
    QVERIFY(QByteArray(BOBUIest::currentAppName()).contains("testlib"));

    QCOMPARE(BOBUIest::testObject(), nullptr); // last, because it should fail
}

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest {
    // Defined; not declared in the public header, but used by bobuideclarative.
    int defaultKeyDelay();
    int defaultMouseDelay();
}

BOBUI_END_NAMESPACE

void tst_TestLib::delays() const
{
    QVERIFY(BOBUIest::defaultMouseDelay() >= 0);
    QVERIFY(BOBUIest::defaultKeyDelay() >= 0);
}

void tst_TestLib::reals_data() const
{
    BOBUIest::addColumn<double>("actual");
    BOBUIest::addColumn<double>("expected");

    BOBUIest::newRow("zero") << 0.0 << 0.0;
#define ADDROW(func) BOBUIest::addRow("self-%s", #func) << func() << func()
    ADDROW(qQNaN);
    ADDROW(qInf);
#undef ADDROW // Just used so as to exercise addRow()
    BOBUIest::newRow("infineg") << -qInf() << -qInf();
    BOBUIest::newRow("Sin(turn/4)") << qSin(9 * M_PI_2) << 1.0;
    BOBUIest::newRow("Cos(turn/2)") << qCos(15 * M_PI) << -1.0;
}

void tst_TestLib::reals() const
{
    QFETCH(double, actual);
    QFETCH(double, expected);
    QCOMPARE(actual, expected);
}

BOBUIEST_APPLESS_MAIN(tst_TestLib)

#include "tst_testlib.moc"
