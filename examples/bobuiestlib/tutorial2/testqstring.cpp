// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BOBUIest>

//! [0]
class TestQString: public QObject
{
    Q_OBJECT

private slots:
    void toUpper_data();
    void toUpper();
};
//! [0]

//! [1]
void TestQString::toUpper_data()
{
    BOBUIest::addColumn<QString>("string");
    BOBUIest::addColumn<QString>("result");

    BOBUIest::newRow("all-lower") << "hello" << "HELLO";
    BOBUIest::newRow("mixed")     << "Hello" << "HELLO";
    BOBUIest::newRow("all-upper") << "HELLO" << "HELLO";
}
//! [1]

//! [2]
void TestQString::toUpper()
{
    QFETCH(QString, string);
    QFETCH(QString, result);

    QCOMPARE(string.toUpper(), result);
}
//! [2]

//! [3]
BOBUIEST_MAIN(TestQString)
#include "testqstring.moc"
//! [3]

