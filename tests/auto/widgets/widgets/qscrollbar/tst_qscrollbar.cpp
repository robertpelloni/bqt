// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QElapsedTimer>
#include <QScrollBar>
#include <QStyleOptionSlider>
#include <QScrollArea>
#include <QScreen>
#include <QSignalSpy>

#include <BobUITest/private/bobuiesthelpers_p.h>

using namespace BOBUIestPrivate;

class tst_QScrollBar : public QObject
{
    Q_OBJECT
private slots:
    void scrollSingleStep();
    void task_209492();
#if BOBUI_CONFIG(wheelevent)
    void BOBUIBUG_27308();
#endif
    void BOBUIBUG_42871();
};

class SingleStepTestScrollBar : public QScrollBar {
    Q_OBJECT
public:
    explicit SingleStepTestScrollBar(BobUI::Orientation o, QWidget *parent = nullptr) : QScrollBar(o, parent) {}

public slots:
    void hideAndShow()
    {
        hide();
        show();
    }
};

// Check that the scrollbar doesn't scroll after calling hide and show
// from a slot connected to the scrollbar's actionTriggered signal.
void tst_QScrollBar::scrollSingleStep()
{
    SingleStepTestScrollBar testWidget(BobUI::Horizontal);
    connect(&testWidget, &QAbstractSlider::actionTriggered, &testWidget, &SingleStepTestScrollBar::hideAndShow);
    testWidget.resize(100, testWidget.height());
    centerOnScreen(&testWidget);
    testWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&testWidget));

    testWidget.setValue(testWidget.minimum());
    QCOMPARE(testWidget.value(), testWidget.minimum());

    // Get rect for the area to click on
    const QStyleOptionSlider opt = bobui_qscrollbarStyleOption(&testWidget);
    QRect sr = testWidget.style()->subControlRect(QStyle::CC_ScrollBar, &opt,
                                                  QStyle::SC_ScrollBarAddLine, &testWidget);

    if (!sr.isValid())
        QSKIP("SC_ScrollBarAddLine not valid");

    BOBUIest::mouseClick(&testWidget, BobUI::LeftButton, BobUI::NoModifier, QPoint(sr.x(), sr.y()));
    BOBUIest::qWait(510); // initial delay is 500 for setRepeatAction
    disconnect(&testWidget, &QAbstractSlider::actionTriggered, &testWidget, &SingleStepTestScrollBar::hideAndShow);
#ifdef Q_OS_MAC
    QEXPECT_FAIL("", "This test fails on OS X, see BOBUIBUG-25272", Abort);
#endif
    QCOMPARE(testWidget.value(), testWidget.singleStep());
}

void tst_QScrollBar::task_209492()
{
    class MyScrollArea : public QScrollArea
    {
    public:
        int scrollCount;
        MyScrollArea(QWidget *parent = nullptr) : QScrollArea(parent), scrollCount(0) {}
    protected:
        void paintEvent(QPaintEvent *) override { BOBUIest::qSleep(600); }
        void scrollContentsBy(int, int) override { ++scrollCount; viewport()->update(); }
    };

    MyScrollArea scrollArea;
    QScrollBar *verticalScrollBar = scrollArea.verticalScrollBar();
    verticalScrollBar->setRange(0, 1000);
    centerOnScreen(&scrollArea);
    scrollArea.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&scrollArea));

    QSignalSpy spy(verticalScrollBar, SIGNAL(actionTriggered(int)));
    QCOMPARE(scrollArea.scrollCount, 0);
    QCOMPARE(spy.size(), 0);

    // Simulate a mouse click on the "scroll down button".
    const QPoint pressPoint(verticalScrollBar->width() / 2, verticalScrollBar->height() - 10);
    const QPoint globalPressPoint = verticalScrollBar->mapToGlobal(pressPoint);
    QMouseEvent mousePressEvent(QEvent::MouseButtonPress, pressPoint, globalPressPoint,
                                BobUI::LeftButton, BobUI::LeftButton, {});
    QApplication::sendEvent(verticalScrollBar, &mousePressEvent);
    BOBUIest::qWait(1);
    QMouseEvent mouseReleaseEvent(QEvent::MouseButtonRelease, pressPoint, globalPressPoint,
                                  BobUI::LeftButton, BobUI::LeftButton, {});
    QApplication::sendEvent(verticalScrollBar, &mouseReleaseEvent);

    // Check that the action was triggered once.

#ifdef Q_OS_MAC
    QSKIP("The result depends on system setting and is not relevant on Mac");
#endif
    QCOMPARE(scrollArea.scrollCount, 1);
    QCOMPARE(spy.size(), 1);
}

#if BOBUI_CONFIG(wheelevent)
#define WHEEL_DELTA 120 // copied from tst_QAbstractSlider / tst_QComboBox
void tst_QScrollBar::BOBUIBUG_27308()
{
    // BOBUIBUG-27308
    // Check that a disabled scrollbar doesn't react on wheel events anymore

    QScrollBar testWidget(BobUI::Horizontal);
    testWidget.resize(100, testWidget.height());
    centerOnScreen(&testWidget);
    testWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&testWidget));

    testWidget.setValue(testWidget.minimum());
    testWidget.setEnabled(false);
    const QPoint wheelPoint = testWidget.rect().center();
    const QPoint angleDelta(testWidget.orientation() == BobUI::Horizontal ? -WHEEL_DELTA : 0,
                            testWidget.orientation() == BobUI::Vertical ? -WHEEL_DELTA : 0);
    QWheelEvent event(wheelPoint, testWidget.mapToGlobal(wheelPoint), QPoint(), angleDelta,
                      BobUI::NoButton, BobUI::NoModifier, BobUI::NoScrollPhase, false);
    qApp->sendEvent(&testWidget, &event);
    QCOMPARE(testWidget.value(), testWidget.minimum());
}
#endif

class BOBUIBUG_42871_Handler : public QObject {
    Q_OBJECT
public:
    int updatesCount;
    BOBUIBUG_42871_Handler() : QObject(), updatesCount(0) {}
public slots:
    void valueUpdated(int) { ++updatesCount; BOBUIest::qSleep(600); }
};

void tst_QScrollBar::BOBUIBUG_42871()
{
    BOBUIBUG_42871_Handler myHandler;
    QScrollBar scrollBarWidget(BobUI::Vertical);
    bool connection = connect(&scrollBarWidget, SIGNAL(valueChanged(int)), &myHandler, SLOT(valueUpdated(int)));
    QVERIFY(connection);
    scrollBarWidget.resize(100, scrollBarWidget.height());
    centerOnScreen(&scrollBarWidget);
    scrollBarWidget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&scrollBarWidget));
    QSignalSpy spy(&scrollBarWidget, SIGNAL(actionTriggered(int)));
    QVERIFY(spy.isValid());
    QCOMPARE(myHandler.updatesCount, 0);
    QCOMPARE(spy.size(), 0);

    // Simulate a mouse click on the "scroll down button".
    const QPoint pressPoint(scrollBarWidget.width() / 2, scrollBarWidget.height() - 10);
    const QPoint globalPressPoint = scrollBarWidget.mapToGlobal(pressPoint);
    QMouseEvent mousePressEvent(QEvent::MouseButtonPress, pressPoint, globalPressPoint,
                                BobUI::LeftButton, BobUI::LeftButton, {});
    QApplication::sendEvent(&scrollBarWidget, &mousePressEvent);
    QElapsedTimer timer;
    timer.start();
    BOBUIest::qWait(1);
    QMouseEvent mouseReleaseEvent(QEvent::MouseButtonRelease, pressPoint, globalPressPoint,
                                  BobUI::LeftButton, BobUI::LeftButton, {});
    QApplication::sendEvent(&scrollBarWidget, &mouseReleaseEvent);
    if (timer.elapsed() > 40) {
        // took too long, we need to tolerate auto-repeat
        if (myHandler.updatesCount > 1)
            QEXPECT_FAIL("", "Took too long to process events, repeat timer fired", Continue);
    }
    // Check that the action was triggered once.
    QCOMPARE(myHandler.updatesCount, 1);
    QCOMPARE(spy.size(), myHandler.updatesCount);
}

BOBUIEST_MAIN(tst_QScrollBar)
#include "tst_qscrollbar.moc"
