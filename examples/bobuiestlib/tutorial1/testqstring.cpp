// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
#include <BOBUIest>

class TestQString: public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
};
//! [0]

//! [1]
void TestQString::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}
//! [1]

//! [2]
BOBUIEST_MAIN(TestQString)
#include "testqstring.moc"
//! [2]

