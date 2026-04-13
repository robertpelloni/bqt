// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
//! [0]
QVERIFY(spy.isValid());
//! [0]

//! [13]
BOBUIest::keyClick(myWidget, 'a');
//! [13]

//! [14]
BOBUIest::keyClick(myWidget, BobUI::Key_Escape);

BOBUIest::keyClick(myWidget, BobUI::Key_Escape, BobUI::ShiftModifier, 200);
//! [14]

//! [15]
BOBUIest::keyClicks(myWidget, "hello world");
//! [15]

//! [16]
namespace BOBUIest {
    template<>
    char *toString(const MyPoint &point)
    {
        const QByteArray ba("MyPoint("
                            + QByteArray::number(point.x()) + ", "
                            + QByteArray::number(point.y()) + ')');
        return qstrdup(ba.data());
    }
}
//! [16]

//! [toString-overload]
namespace {
    char *toString(const MyPoint &point)
    {
        return BOBUIest::toString("MyPoint(" +
                               QByteArray::number(point.x()) + ", " +
                               QByteArray::number(point.y()) + ')');
    }
}
//! [toString-overload]

void processTouchEvent()
{
//! [25]
BOBUIouchDevice *dev = BOBUIest::createTouchDevice();
QWidget widget;

BOBUIest::touchEvent(&widget, dev)
    .press(0, QPoint(10, 10));
BOBUIest::touchEvent(&widget, dev)
    .stationary(0)
    .press(1, QPoint(40, 10));
BOBUIest::touchEvent(&widget, dev)
    .move(0, QPoint(12, 12))
    .move(1, QPoint(45, 5));
BOBUIest::touchEvent(&widget, dev)
    .release(0, QPoint(12, 12))
    .release(1, QPoint(45, 5));
//! [25]
}

//! [26]
bool tst_MyXmlParser::parse()
{
    MyXmlParser parser;
    QString input = QFINDTESTDATA("testxml/simple1.xml");
    QVERIFY(parser.parse(input));
}
//! [26]

//! [28]
QWidget myWindow;
BOBUIest::keyClick(&myWindow, BobUI::Key_Tab);
//! [28]

//! [29]
BOBUIest::keyClick(&myWindow, BobUI::Key_Escape);
BOBUIest::keyClick(&myWindow, BobUI::Key_Escape, BobUI::ShiftModifier, 200);
//! [29]

//! [30]
void TestQLocale::initTestCase_data()
{
    BOBUIest::addColumn<QLocale>("locale");
    BOBUIest::newRow("C") << QLocale::c();
    BOBUIest::newRow("UKish") << QLocale("en_GB");
    BOBUIest::newRow("USAish") << QLocale(QLocale::English, QLocale::UnitedStates);
}

void TestQLocale::roundTripInt_data()
{
    BOBUIest::addColumn<int>("number");
    BOBUIest::newRow("zero") << 0;
    BOBUIest::newRow("one") << 1;
    BOBUIest::newRow("two") << 2;
    BOBUIest::newRow("ten") << 10;
}
//! [30]

//! [31]
void TestQLocale::roundTripInt()
{
    QFETCH_GLOBAL(QLocale, locale);
    QFETCH(int, number);
    bool ok;
    QCOMPARE(locale.toInt(locale.toString(number), &ok), number);
    QVERIFY(ok);
}
//! [31]

//! [34]
char *toString(const MyType &t)
{
    char *repr = new char[t.reprSize()];
    t.writeRepr(repr);
    return repr;
}
//! [34]

//! [35]
QSignalSpy doubleClickSpy(target, &TargetClass::doubleClicked);
const QPoint p(1, 2);
BOBUIest::mousePress(&myWindow, BobUI::LeftButton, BobUI::NoModifier, p);
QVERIFY(target.isPressed());
BOBUIest::mouseRelease(&myWindow, BobUI::LeftButton, BobUI::NoModifier, p, 10);
QCOMPARE(target.isPressed(), false);
BOBUIest::mousePress(&myWindow, BobUI::LeftButton, BobUI::NoModifier, p, 10);
QCOMPARE(target.pressCount(), 2);
BOBUIest::mouseRelease(&myWindow, BobUI::LeftButton, BobUI::NoModifier, p, 10);
QCOMPARE(doubleClickSpy.count(), 1);
//! [35]
