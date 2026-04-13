// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qevent.h>
#include <qwindow.h>
#include <BobUIGui/private/qpointingdevice_p.h>

#include <BobUICore/qpointer.h>

Q_LOGGING_CATEGORY(lcTests, "bobui.gui.tests")

class MouseEventWidget : public QWindow
{
public:
    MouseEventWidget(QWindow *parent = nullptr) : QWindow(parent)
    {
        setGeometry(100, 100, 100, 100);
    }
    bool grabExclusive = false;
    bool grabPassive = false;
    bool mousePressEventRecieved;
    bool mouseReleaseEventRecieved;
    int mousePressButton;
    int mousePressButtons;
    int mousePressModifiers;
    int mouseReleaseButton;
    int mouseReleaseButtons;
    int mouseReleaseModifiers;
    ulong timestamp;
    ulong pressTimestamp;
    ulong lastTimestamp;
    QVector2D velocity;
protected:
    void mousePressEvent(QMouseEvent *e) override
    {
        const auto &firstPoint = e->point(0);
        qCDebug(lcTests) << e << firstPoint;
        timestamp = firstPoint.timestamp();
        lastTimestamp = firstPoint.lastTimestamp();
        if (e->type() == QEvent::MouseButtonPress) {
            auto firstPoint = e->points().first();
            QCOMPARE(e->exclusiveGrabber(firstPoint), nullptr);
            QVERIFY(e->passiveGrabbers(firstPoint).isEmpty());
            QCOMPARE(firstPoint.timeHeld(), 0);
            QCOMPARE(firstPoint.pressTimestamp(), e->timestamp());
            pressTimestamp = e->timestamp();
        }
        QWindow::mousePressEvent(e);
        mousePressButton = e->button();
        mousePressButtons = e->buttons();
        mousePressModifiers = e->modifiers();
        mousePressEventRecieved = true;
        e->accept();
        // It's not normal for QWindow to be the grabber, but that's easier to test
        // without needing to create child ojects.
        if (grabExclusive)
            e->setExclusiveGrabber(firstPoint, this);
        if (grabPassive)
            e->addPassiveGrabber(firstPoint, this);
    }
    void mouseMoveEvent(QMouseEvent *e) override
    {
        qCDebug(lcTests) << e << e->points().first();
        timestamp = e->points().first().timestamp();
        lastTimestamp = e->points().first().lastTimestamp();
        velocity = e->points().first().velocity();
    }
    void mouseReleaseEvent(QMouseEvent *e) override
    {
        qCDebug(lcTests) << e << e->points().first();
        QWindow::mouseReleaseEvent(e);
        mouseReleaseButton = e->button();
        mouseReleaseButtons = e->buttons();
        mouseReleaseModifiers = e->modifiers();
        timestamp = e->points().first().timestamp();
        lastTimestamp = e->points().first().lastTimestamp();
        velocity = e->points().first().velocity();
        mouseReleaseEventRecieved = true;
        e->accept();
    }
};

class tst_QMouseEvent : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
private slots:
    void mouseEventBasic();
    void checkMousePressEvent_data();
    void checkMousePressEvent();
    void checkMouseReleaseEvent_data();
    void checkMouseReleaseEvent();
    void grabbers_data();
    void grabbers();
    void velocity();
    void clone();

private:
    MouseEventWidget* testMouseWidget;
};

void tst_QMouseEvent::initTestCase()
{
    testMouseWidget = new MouseEventWidget;
    testMouseWidget->show();
}

void tst_QMouseEvent::cleanupTestCase()
{
    delete testMouseWidget;
}

void tst_QMouseEvent::init()
{
    testMouseWidget->mousePressEventRecieved = false;
    testMouseWidget->mouseReleaseEventRecieved = false;
    testMouseWidget->mousePressButton = 0;
    testMouseWidget->mousePressButtons = 0;
    testMouseWidget->mousePressModifiers = 0;
    testMouseWidget->mouseReleaseButton = 0;
    testMouseWidget->mouseReleaseButtons = 0;
    testMouseWidget->mouseReleaseModifiers = 0;
}

void tst_QMouseEvent::mouseEventBasic()
{
    QPointF local(100, 100);
    QPointF scene(200, 200);
    QPointF screen(300, 300);
    // Press left button
    QMouseEvent me(QEvent::MouseButtonPress, local, scene, screen, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier);
    QVERIFY(me.isInputEvent());
    QVERIFY(me.isPointerEvent());
    QVERIFY(me.isSinglePointEvent());
    QCOMPARE(me.isAccepted(), true);
    QCOMPARE(me.button(), BobUI::LeftButton);
    QCOMPARE(me.buttons(), BobUI::LeftButton);
    QVERIFY(me.isBeginEvent());
    QVERIFY(!me.isEndEvent());
    QCOMPARE(me.position(), local);
    QCOMPARE(me.scenePosition(), scene);
    QCOMPARE(me.globalPosition(), screen);
    // Press right button while left is already pressed
    QMouseEvent me2(QEvent::MouseButtonPress, local, scene, screen, BobUI::RightButton, BobUI::LeftButton | BobUI::RightButton, BobUI::NoModifier);
    QVERIFY(me2.isBeginEvent());
    QVERIFY(!me2.isEndEvent());
    // Release right button while left is still pressed
    QMouseEvent me3 = QMouseEvent(QEvent::MouseButtonRelease, local, scene, screen, BobUI::RightButton, BobUI::LeftButton, BobUI::NoModifier);
    QVERIFY(!me3.isBeginEvent());
    QVERIFY(me3.isEndEvent());
    // Release left button in the usual way
    QMouseEvent me4 = QMouseEvent(QEvent::MouseButtonRelease, local, scene, screen, BobUI::LeftButton, BobUI::NoButton, BobUI::NoModifier);
    QVERIFY(!me4.isBeginEvent());
    QVERIFY(me4.isEndEvent());
}

void tst_QMouseEvent::checkMousePressEvent_data()
{
    BOBUIest::addColumn<int>("buttonPressed");
    BOBUIest::addColumn<int>("keyPressed");

    BOBUIest::newRow("leftButton-nokey") << int(BobUI::LeftButton) << int(BobUI::NoButton);
    BOBUIest::newRow("leftButton-shiftkey") << int(BobUI::LeftButton) << int(BobUI::ShiftModifier);
    BOBUIest::newRow("leftButton-controlkey") << int(BobUI::LeftButton) << int(BobUI::ControlModifier);
    BOBUIest::newRow("leftButton-altkey") << int(BobUI::LeftButton) << int(BobUI::AltModifier);
    BOBUIest::newRow("leftButton-metakey") << int(BobUI::LeftButton) << int(BobUI::MetaModifier);
    BOBUIest::newRow("rightButton-nokey") << int(BobUI::RightButton) << int(BobUI::NoButton);
    BOBUIest::newRow("rightButton-shiftkey") << int(BobUI::RightButton) << int(BobUI::ShiftModifier);
    BOBUIest::newRow("rightButton-controlkey") << int(BobUI::RightButton) << int(BobUI::ControlModifier);
    BOBUIest::newRow("rightButton-altkey") << int(BobUI::RightButton) << int(BobUI::AltModifier);
    BOBUIest::newRow("rightButton-metakey") << int(BobUI::RightButton) << int(BobUI::MetaModifier);
    BOBUIest::newRow("middleButton-nokey") << int(BobUI::MiddleButton) << int(BobUI::NoButton);
    BOBUIest::newRow("middleButton-shiftkey") << int(BobUI::MiddleButton) << int(BobUI::ShiftModifier);
    BOBUIest::newRow("middleButton-controlkey") << int(BobUI::MiddleButton) << int(BobUI::ControlModifier);
    BOBUIest::newRow("middleButton-altkey") << int(BobUI::MiddleButton) << int(BobUI::AltModifier);
    BOBUIest::newRow("middleButton-metakey") << int(BobUI::MiddleButton) << int(BobUI::MetaModifier);
}

void tst_QMouseEvent::checkMousePressEvent()
{
    QFETCH(int,buttonPressed);
    QFETCH(int,keyPressed);
    int button = buttonPressed;
    int buttons = button;
    int modifiers = keyPressed;

    BOBUIest::mousePress(testMouseWidget, BobUI::MouseButton(buttonPressed), BobUI::KeyboardModifiers(keyPressed));
    qApp->processEvents();
    QVERIFY(testMouseWidget->mousePressEventRecieved);
    QCOMPARE(testMouseWidget->mousePressButton, button);
    QCOMPARE(testMouseWidget->mousePressButtons, buttons);
    QCOMPARE(testMouseWidget->mousePressModifiers, modifiers);

    BOBUIest::mouseRelease(testMouseWidget, BobUI::MouseButton(buttonPressed), BobUI::KeyboardModifiers(keyPressed));
    qApp->processEvents();
}

void tst_QMouseEvent::checkMouseReleaseEvent_data()
{
    BOBUIest::addColumn<int>("buttonReleased");
    BOBUIest::addColumn<int>("keyPressed");

    BOBUIest::newRow("leftButton-nokey") << int(BobUI::LeftButton) << int(BobUI::NoButton);
    BOBUIest::newRow("leftButton-shiftkey") << int(BobUI::LeftButton) << int(BobUI::ShiftModifier);
    BOBUIest::newRow("leftButton-controlkey") << int(BobUI::LeftButton) << int(BobUI::ControlModifier);
    BOBUIest::newRow("leftButton-altkey") << int(BobUI::LeftButton) << int(BobUI::AltModifier);
    BOBUIest::newRow("leftButton-metakey") << int(BobUI::LeftButton) << int(BobUI::MetaModifier);
    BOBUIest::newRow("rightButton-nokey") << int(BobUI::RightButton) << int(BobUI::NoButton);
    BOBUIest::newRow("rightButton-shiftkey") << int(BobUI::RightButton) << int(BobUI::ShiftModifier);
    BOBUIest::newRow("rightButton-controlkey") << int(BobUI::RightButton) << int(BobUI::ControlModifier);
    BOBUIest::newRow("rightButton-altkey") << int(BobUI::RightButton) << int(BobUI::AltModifier);
    BOBUIest::newRow("rightButton-metakey") << int(BobUI::RightButton) << int(BobUI::MetaModifier);
    BOBUIest::newRow("middleButton-nokey") << int(BobUI::MiddleButton) << int(BobUI::NoButton);
    BOBUIest::newRow("middleButton-shiftkey") << int(BobUI::MiddleButton) << int(BobUI::ShiftModifier);
    BOBUIest::newRow("middleButton-controlkey") << int(BobUI::MiddleButton) << int(BobUI::ControlModifier);
    BOBUIest::newRow("middleButton-altkey") << int(BobUI::MiddleButton) << int(BobUI::AltModifier);
    BOBUIest::newRow("middleButton-metakey") << int(BobUI::MiddleButton) << int(BobUI::MetaModifier);
}

void tst_QMouseEvent::checkMouseReleaseEvent()
{
    QFETCH(int,buttonReleased);
    QFETCH(int,keyPressed);
    int button = buttonReleased;
    int buttons = 0;
    int modifiers = keyPressed;

    BOBUIest::mouseClick(testMouseWidget, BobUI::MouseButton(buttonReleased), BobUI::KeyboardModifiers(keyPressed));
    qApp->processEvents();
    QVERIFY(testMouseWidget->mouseReleaseEventRecieved);
    QCOMPARE(testMouseWidget->mouseReleaseButton, button);
    QCOMPARE(testMouseWidget->mouseReleaseButtons, buttons);
    QCOMPARE(testMouseWidget->mouseReleaseModifiers, modifiers);
}

void tst_QMouseEvent::grabbers_data()
{
    BOBUIest::addColumn<bool>("grabExclusive");
    BOBUIest::addColumn<bool>("grabPassive");

    BOBUIest::newRow("no grab") << false << false;
    BOBUIest::newRow("exclusive") << true << false;
    BOBUIest::newRow("passive") << false << true;
}

void tst_QMouseEvent::grabbers()
{
    QFETCH(bool, grabExclusive);
    QFETCH(bool, grabPassive);

    testMouseWidget->grabExclusive = grabExclusive;
    testMouseWidget->grabPassive = grabPassive;

    BOBUIest::mousePress(testMouseWidget, BobUI::LeftButton, BobUI::KeyboardModifiers(), {10, 10});

    auto devPriv = QPointingDevicePrivate::get(QPointingDevice::primaryPointingDevice());
    QCOMPARE(devPriv->activePoints.count(), 1);

    // Ensure that grabbers are persistent between events, within the stored touchpoints
    auto firstEPD = devPriv->pointById(0);
    QCOMPARE(firstEPD->eventPoint.pressTimestamp(), testMouseWidget->pressTimestamp);
    QCOMPARE(firstEPD->exclusiveGrabber, grabExclusive ? testMouseWidget : nullptr);
    QCOMPARE(firstEPD->passiveGrabbers.size(), grabPassive ? 1 : 0);
    if (grabPassive)
        QCOMPARE(firstEPD->passiveGrabbers.first(), testMouseWidget);

    // Ensure that grabbers are forgotten after release delivery
    BOBUIest::mouseRelease(testMouseWidget, BobUI::LeftButton, BobUI::KeyboardModifiers(), {10, 10});
    BOBUIRY_COMPARE(firstEPD->exclusiveGrabber, nullptr);
    QCOMPARE(firstEPD->passiveGrabbers.size(), 0);
}

void tst_QMouseEvent::velocity()
{
    testMouseWidget->grabExclusive = true;
    auto devPriv = QPointingDevicePrivate::get(const_cast<QPointingDevice *>(QPointingDevice::primaryPointingDevice()));
    devPriv->activePoints.clear();

    qCDebug(lcTests) << "sending mouse press event";
    QPoint pos(10, 10);
    BOBUIest::mousePress(testMouseWidget, BobUI::LeftButton, BobUI::KeyboardModifiers(), pos);
    QCOMPARE(devPriv->activePoints.count(), 1);
    QVERIFY(devPriv->activePoints.count() <= 2);
    const auto &firstPoint = devPriv->pointById(0)->eventPoint;
    QVERIFY(firstPoint.timestamp() > 0);
    QCOMPARE(firstPoint.state(), QEventPoint::State::Pressed);

    ulong timestamp = firstPoint.timestamp();
    for (int i = 1; i < 4; ++i) {
        qCDebug(lcTests) << "sending mouse move event" << i;
        pos += {10, 10};
        BOBUIest::mouseMove(testMouseWidget, pos, 1);
        qApp->processEvents();
        qCDebug(lcTests) << firstPoint;
        // currently we expect it to be updated in-place in devPriv->activePoints
        QVERIFY(firstPoint.timestamp() > timestamp);
        QVERIFY(testMouseWidget->timestamp > testMouseWidget->lastTimestamp);
        QCOMPARE(testMouseWidget->timestamp, firstPoint.timestamp());
        timestamp = firstPoint.timestamp();
        QVERIFY(testMouseWidget->velocity.x() > 0);
        QVERIFY(testMouseWidget->velocity.y() > 0);
    }
    BOBUIest::mouseRelease(testMouseWidget, BobUI::LeftButton, BobUI::KeyboardModifiers(), pos, 1);
    qCDebug(lcTests) << firstPoint;
    QVERIFY(testMouseWidget->velocity.x() > 0);
    QVERIFY(testMouseWidget->velocity.y() > 0);
}

void tst_QMouseEvent::clone()
{
    const QPointF pos(10.0f, 10.0f);

    QMouseEvent originalMe(QEvent::MouseButtonPress, pos, pos, pos, BobUI::LeftButton, BobUI::LeftButton, BobUI::NoModifier);
    QVERIFY(!originalMe.allPointsAccepted());
    QVERIFY(!originalMe.points().first().isAccepted());

    // create a clone of the original
    std::unique_ptr<QMouseEvent> clonedMe(originalMe.clone());
    QVERIFY(!clonedMe->allPointsAccepted());
    QVERIFY(!clonedMe->points().first().isAccepted());

    // now we alter originalMe, which should *not* change clonedMe
    originalMe.setAccepted(true);
    QVERIFY(!clonedMe->allPointsAccepted());
    QVERIFY(!clonedMe->points().first().isAccepted());
}

BOBUIEST_MAIN(tst_QMouseEvent)
#include "tst_qmouseevent.moc"
