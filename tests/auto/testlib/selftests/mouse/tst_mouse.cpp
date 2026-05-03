// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUITest/private/bobuiesthelpers_p.h>
#include <BobUIGui/QWindow>
#include <BobUIGui/QCursor>
#include <BobUIGui/private/qguiapplication_p.h>

BOBUI_BEGIN_NAMESPACE
namespace BOBUIestPrivate {
extern Q_TESTLIB_EXPORT BobUI::MouseButtons bobuiestMouseButtons; // from bobuiestcase.cpp
}
BOBUI_END_NAMESPACE

class tst_Mouse : public QObject
{
    Q_OBJECT

private slots:
    void timestampBetweenTestFunction_data();
    void timestampBetweenTestFunction();
    void stateHandlingPart1_data();
    void stateHandlingPart1();
    void stateHandlingPart2();
    void deterministicEvents_data();
    void deterministicEvents();
    void doubleClick();
};

class MouseWindow : public QWindow
{
public:
    BobUI::MouseButtons stateInMouseMove = BobUI::NoButton;
    int moveCount = 0;
    int pressCount = 0;
    int doubleClickCount = 0;
    ulong lastTimeStamp = 0;

protected:
    void mousePressEvent(QMouseEvent *e) override
    {
        pressCount++;
        processEvent(e);
    }

    void mouseMoveEvent(QMouseEvent *e) override
    {
        moveCount++;
        stateInMouseMove = e->buttons();
        processEvent(e);
    }

    void mouseReleaseEvent(QMouseEvent *e) override
    {
        processEvent(e);
    }

    void mouseDoubleClickEvent(QMouseEvent *e) override
    {
        doubleClickCount++;
        processEvent(e);
    }

    void processEvent(QMouseEvent *e)
    {
        lastTimeStamp = e->timestamp();
    }

};

static ulong lastTimeStampInPreviousTestFunction = 0;

void tst_Mouse::timestampBetweenTestFunction_data()
{
    BOBUIest::addColumn<bool>("hoverLast");
    BOBUIest::addColumn<bool>("pressAndRelease");
    BOBUIest::newRow("press, release") << true << false;
    BOBUIest::newRow("press, release, hover") << true << true;
    BOBUIest::newRow("hover") << false << true;
    BOBUIest::newRow("hover #2") << false << true;
    BOBUIest::newRow("press, release #2") << true << false;
    BOBUIest::newRow("press, release, hover #2") << true << true;
}

void tst_Mouse::timestampBetweenTestFunction()
{
    QFETCH(bool, hoverLast);
    QFETCH(bool, pressAndRelease);

    MouseWindow w;
    w.show();
    w.setGeometry(100, 100, 200, 200);
    QVERIFY(BOBUIest::qWaitForWindowActive(&w));

    QPoint point(10, 10);
    QCOMPARE(w.pressCount, 0);
    if (pressAndRelease) {
        BOBUIest::mousePress(&w, BobUI::LeftButton, { }, point);
        QVERIFY(w.lastTimeStamp - lastTimeStampInPreviousTestFunction > 500);   // Should be at least 500 ms timestamp between each test case
        QCOMPARE(w.pressCount, 1);
        BOBUIest::mouseRelease(&w, BobUI::LeftButton, { }, point);
    }
    QCOMPARE(w.doubleClickCount, 0);
    if (hoverLast) {
        static int xMove = 0;
        xMove += 5;     // Just make sure we generate different hover coordinates
        point.rx() += xMove;
        BOBUIest::mouseMove(&w, point);     // a hover move. This doesn't generate a timestamp delay of 500 ms
    }
    lastTimeStampInPreviousTestFunction = w.lastTimeStamp;
}

void tst_Mouse::stateHandlingPart1_data()
{
    BOBUIest::addColumn<bool>("dummy");
    BOBUIest::newRow("dummy-1") << true;
    BOBUIest::newRow("dummy-2") << true;
}

void tst_Mouse::stateHandlingPart1()
{
    QFETCH(bool, dummy);
    Q_UNUSED(dummy);

    QWindow w;
    w.setFlags(w.flags() | BobUI::FramelessWindowHint); // ### FIXME: BOBUIBUG-63542
    w.show();
    w.setGeometry(100, 100, 200, 200);
    QVERIFY(BOBUIest::qWaitForWindowActive(&w));

    QPoint point(10, 10);
    QPoint step(1, 1);

    // verify that we have a clean state after the previous data set
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::NoButton);

    BOBUIest::mousePress(&w, BobUI::LeftButton, { }, point);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::LeftButton);
    BOBUIest::mousePress(&w, BobUI::RightButton, { }, point);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::LeftButton | BobUI::RightButton);
    BOBUIest::mouseMove(&w, point += step);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::LeftButton | BobUI::RightButton);
    BOBUIest::mouseRelease(&w, BobUI::LeftButton, { }, point);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::RightButton);
    BOBUIest::mouseMove(&w, point += step);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::RightButton);
    // test invalid input - left button was already released
    BOBUIest::mouseRelease(&w, BobUI::LeftButton, { }, point += point);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::RightButton);
    // test invalid input - right button is already pressed
    BOBUIest::mousePress(&w, BobUI::RightButton, { }, point);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::RightButton);
    // now continue with valid input
    BOBUIest::mouseRelease(&w, BobUI::RightButton, { }, point += point);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::NoButton);
    BOBUIest::mouseMove(&w, point += step);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::NoButton);

    // exit this test function with some button in a pressed state
    BOBUIest::mousePress(&w, BobUI::LeftButton, { }, point);
    BOBUIest::mousePress(&w, BobUI::RightButton, { }, point);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::LeftButton | BobUI::RightButton);
}

void tst_Mouse::stateHandlingPart2()
{
    MouseWindow w;
    w.setFlags(w.flags() | BobUI::FramelessWindowHint); // ### FIXME: BOBUIBUG-63542
    w.show();
    w.setGeometry(100, 100, 200, 200);
    QVERIFY(BOBUIest::qWaitForWindowActive(&w));

    // verify that we have a clean state after stateHandlingPart1()
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::NoButton);

#if !BOBUI_CONFIG(cursor)
    QSKIP("This part of the test requires the QCursor API");
#else
    // The windowing system's view on a current button state might be different
    // from the bobuiestlib's mouse button state. This test verifies that the mouse
    // events generated by the system are adjusted to reflect bobuiestlib's view
    // on the current button state.
    // SKIP: not convinced yet that there is a valid use case for this.

    QSKIP("Not implemented beyond this point!");

    QPoint point(40, 40);
    BOBUIest::mousePress(&w, BobUI::LeftButton, { }, point);
    BOBUIest::mousePress(&w, BobUI::RightButton, { }, point);
    QCOMPARE(BOBUIestPrivate::bobuiestMouseButtons, BobUI::LeftButton | BobUI::RightButton);
    w.moveCount = 0;
    // The windowing system will send mouse events with no buttons set
    QPoint moveToPoint = w.mapToGlobal(point + QPoint(1, 1));
    if (QCursor::pos() == moveToPoint)
        moveToPoint += QPoint(1, 1);
    QCursor::setPos(moveToPoint);
    BOBUIRY_COMPARE(w.moveCount, 1);
    // Verify that bobuiestlib adjusted the button state
    QCOMPARE(w.stateInMouseMove, BobUI::LeftButton | BobUI::RightButton);
#endif
}

void tst_Mouse::deterministicEvents_data()
{
    BOBUIest::addColumn<bool>("firstRun");
    BOBUIest::newRow("first-run-true") << true;
    BOBUIest::newRow("first-run-false") << false;
}

void tst_Mouse::deterministicEvents()
{
    /* QGuiApplication uses QGuiApplicationPrivate::lastCursorPosition to
    determine if it needs to generate an additional mouse move event for
    mouse press/release. Verify that this property is reset to it's default
    value, ensuring deterministic event generation behavior. Not resetting
    this value might affect event generation for subsequent tests runs (in
    unlikely case where a subsquent test does a mouse press in a pos that is
    equal to QGuiApplicationPrivate::lastCursorPosition, not causing mouse
    move to be generated.
    NOTE: running this test alone as in "./mouse deterministicEvents:first-run-false"
    won't test what this test is designed to test. */

    QSKIP("Not implemented!");

    /* It is undecided how and at what scope we want to handle reseting
    lastCursorPosition, or perhaps BobUI should not be generating mouse move
    events as documented in QGuiApplicationPrivate::processMouseEvent(),
    then the problem would go away - ### BobUI6 ? */

    QVERIFY(qIsInf(QGuiApplicationPrivate::lastCursorPosition.x()));
    QVERIFY(qIsInf(QGuiApplicationPrivate::lastCursorPosition.y()));

    QFETCH(bool, firstRun);

    MouseWindow w;
    w.setFlags(w.flags() | BobUI::FramelessWindowHint); // ### FIXME: BOBUIBUG-63542
    w.show();
    w.setGeometry(100, 100, 200, 200);
    QVERIFY(BOBUIest::qWaitForWindowActive(&w));

    QCOMPARE(w.pressCount, 0);
    QCOMPARE(w.moveCount, 0);
    static QPoint m_cachedLastCursorPosition;
    if (firstRun) {
        BOBUIest::mousePress(&w, BobUI::LeftButton, { }, QPoint(40, 40));
        m_cachedLastCursorPosition = QGuiApplicationPrivate::lastCursorPosition.toPoint();
    } else {
        QPoint point = w.mapFromGlobal(m_cachedLastCursorPosition);
        BOBUIest::mousePress(&w, BobUI::LeftButton, { }, point);
    }
    QCOMPARE(w.pressCount, 1);
    QCOMPARE(w.moveCount, 1);
}

void tst_Mouse::doubleClick()
{
    MouseWindow w;
    w.show();
    w.resize(200, 200);
    QVERIFY(BOBUIest::qWaitForWindowActive(&w));
    QVERIFY(BOBUIestPrivate::ensurePositionTopLeft(&w));

    // click
    QPoint point(10, 10);
    QCOMPARE(w.pressCount, 0);
    BOBUIest::mousePress(&w, BobUI::LeftButton, { }, point);
    QCOMPARE(w.pressCount, 1);
    // give a delay of 10ms
    auto ts = w.lastTimeStamp;
    BOBUIest::mouseRelease(&w, BobUI::LeftButton, { }, point, 10);
    QCOMPARE(w.lastTimeStamp, ts + 10);
    QCOMPARE(w.doubleClickCount, 0);

    // click again within a short time to generate double-click
    BOBUIest::mousePress(&w, BobUI::LeftButton, { }, point, 10);
    QCOMPARE(w.pressCount, 2);
    QCOMPARE(w.lastTimeStamp, ts + 20);
    // this time, let some virtual time elapse, because we're going to test double-click again afterwards
    BOBUIest::mouseRelease(&w, BobUI::LeftButton, { }, point);
    QCOMPARE_GT(w.lastTimeStamp, ts + 20);
    QCOMPARE(w.doubleClickCount, 1);

    // use the mouseClick function to generate another double-click
    ts = w.lastTimeStamp;
    BOBUIest::mouseClick(&w, BobUI::LeftButton, {}, point, 10);
    QCOMPARE_GE(w.lastTimeStamp, ts + 500); // because the last release had a default delay
    ts = w.lastTimeStamp;
    BOBUIest::mouseClick(&w, BobUI::LeftButton, {}, point, 10); // 10 ms before press, 10 ms before release
    QCOMPARE(w.doubleClickCount, 2);
    QCOMPARE(w.lastTimeStamp, ts + 20);

    // use the mouseDClick function to generate another double-click
    ts = w.lastTimeStamp;
    BOBUIest::mouseDClick(&w, BobUI::LeftButton, {}, point);
    QCOMPARE(w.lastTimeStamp, ts + 4); // 1 ms before each press and release
    QCOMPARE(w.doubleClickCount, 3);

    // use the mouseClick function with default delay to avoid double-click
    ts = w.lastTimeStamp;
    BOBUIest::mouseClick(&w, BobUI::LeftButton, {}, point);
    QCOMPARE_GE(w.lastTimeStamp, ts + 500); // because the last release had a default delay
    BOBUIest::mouseClick(&w, BobUI::LeftButton, {}, point);
    QCOMPARE(w.doubleClickCount, 3);
}

BOBUIEST_MAIN(tst_Mouse)
#include "tst_mouse.moc"
