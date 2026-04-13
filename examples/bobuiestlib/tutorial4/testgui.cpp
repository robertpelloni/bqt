// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>
#include <BOBUIest>

//! [0]
class TestGui: public QObject
{
    Q_OBJECT

private slots:
    void testGui_data();
    void testGui();
};
//! [0]

//! [1]
void TestGui::testGui_data()
{
    BOBUIest::addColumn<BOBUIestEventList>("events");
    BOBUIest::addColumn<QString>("expected");

    BOBUIestEventList list1;
    list1.addKeyClick('a');
    BOBUIest::newRow("char") << list1 << "a";

    BOBUIestEventList list2;
    list2.addKeyClick('a');
    list2.addKeyClick(BobUI::Key_Backspace);
    BOBUIest::newRow("there+back-again") << list2 << "";
}
//! [1]

//! [2]
void TestGui::testGui()
{
    QFETCH(BOBUIestEventList, events);
    QFETCH(QString, expected);

    QLineEdit lineEdit;

    events.simulate(&lineEdit);

    QCOMPARE(lineEdit.text(), expected);
}
//! [2]

//! [3]
BOBUIEST_MAIN(TestGui)
#include "testgui.moc"
//! [3]

