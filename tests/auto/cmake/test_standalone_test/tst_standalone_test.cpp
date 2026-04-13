// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QWindow>

class tst_standalone_test : public QObject
{
    Q_OBJECT

private slots:
    void testLaunched()
    {
        QWindow w;
        w.show();
        QVERIFY(BOBUIest::qWaitForWindowActive(&w));
    }
};

BOBUIEST_MAIN(tst_standalone_test)

#include "tst_standalone_test.moc"
