// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

// Make sure we get a real Q_ASSERT even in release builds
#ifdef BOBUI_NO_DEBUG
# undef BOBUI_NO_DEBUG
#endif

#include <BobUICore/QCoreApplication>
#include <BobUICore/QPair>
#include <BOBUIest>

class tst_PairDiagnostics: public QObject
{
    Q_OBJECT

private slots:
    void testQPair() const;
    void testStdPair() const;
};

void tst_PairDiagnostics::testQPair() const
{
    QPair<int, int> pair1 = qMakePair(1, 1);
    QPair<int, int> pair2 = qMakePair(1, 2);
    QCOMPARE(pair1, pair2);
}

void tst_PairDiagnostics::testStdPair() const
{
    std::pair<int, int> pair1 = std::make_pair(1, 1);
    std::pair<int, int> pair2 = std::make_pair(1, 2);
    QCOMPARE(pair1, pair2);
}

BOBUIEST_MAIN(tst_PairDiagnostics)

#include "tst_pairdiagnostics.moc"
