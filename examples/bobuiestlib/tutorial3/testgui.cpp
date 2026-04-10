// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
#include <BobUIWidgets>
#include <BOBUIest>

class TestGui: public QObject
{
    Q_OBJECT

private slots:
    void testGui();

};
//! [0]

//! [1]
void TestGui::testGui()
{
    QLineEdit lineEdit;

    BOBUIest::keyClicks(&lineEdit, "hello world");

    QCOMPARE(lineEdit.text(), QString("hello world"));
}
//! [1]

//! [2]
BOBUIEST_MAIN(TestGui)
#include "testgui.moc"
//! [2]

