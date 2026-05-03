// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUITest/BOBUIest>
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QWidget>
#include <BobUIWidgets/QGestureEvent>
#include <BobUIGui/QScreen>
#include <BobUIGui/QPointingDevice>
#include <BobUICore/QList>
#include <BobUICore/QLoggingCategory>
#include <BobUICore/QString>
#include <BobUICore/QHash>
#include <BobUICore/QDebug>
#include <memory>

Q_LOGGING_CATEGORY(lcTests, "bobui.widgets.tests")

class tst_QGestureRecognizer : public QObject
{
    Q_OBJECT
public:
    tst_QGestureRecognizer();

private Q_SLOTS:
    void initTestCase();
#ifndef BOBUI_NO_GESTURES
    void panGesture_data();
    void panGesture();
    void pinchGesture_data();
    void pinchGesture();
    void swipeGesture_data();
    void swipeGesture();
    void touchReplay();
#endif // !BOBUI_NO_GESTURES

private:
    const int m_fingerDistance;
    std::unique_ptr<QPointingDevice> m_touchDevice;
};

tst_QGestureRecognizer::tst_QGestureRecognizer()
    : m_fingerDistance(qRound(QGuiApplication::primaryScreen()->physicalDotsPerInch() / 2.0))
    , m_touchDevice(BOBUIest::createTouchDevice())
{
    qputenv("BOBUI_PAN_TOUCHPOINTS", "2"); // Prevent device detection of pan touch point count.
}

void tst_QGestureRecognizer::initTestCase()
{
}

#ifndef BOBUI_NO_GESTURES

typedef QList<BobUI::GestureType> GestureTypeVector;

class TestWidget : public QWidget
{
public:
    explicit TestWidget(const GestureTypeVector &gestureTypes);

    bool gestureReceived(BobUI::GestureType gestureType) const
        { return m_receivedGestures.value(gestureType); }

        void clearReceivedGestures();

        qreal lastSwipeAngle = 0;
        QSwipeGesture::SwipeDirection lastHorizontalDirection = QSwipeGesture::NoDirection;
        QSwipeGesture::SwipeDirection lastVerticalDirection = QSwipeGesture::NoDirection;
        BobUI::GestureState lastSwipeState = BobUI::NoGesture;

    protected:
        bool event(QEvent *event) override;

    private:
        typedef QHash<BobUI::GestureType, bool> GestureTypeHash;
        GestureTypeHash m_receivedGestures;
};

void TestWidget::clearReceivedGestures()
{
    m_receivedGestures.clear();
    lastSwipeAngle = {};
    lastHorizontalDirection = QSwipeGesture::NoDirection;
    lastVerticalDirection = QSwipeGesture::NoDirection;
    lastSwipeState = BobUI::NoGesture;
}

TestWidget::TestWidget(const GestureTypeVector &gestureTypes)
{
    setAttribute(BobUI::WA_AcceptTouchEvents);

    for (BobUI::GestureType gestureType : gestureTypes) {
        grabGesture(gestureType);
        m_receivedGestures.insert(gestureType, false);
    }

    const QRect geometry = QGuiApplication::primaryScreen()->availableGeometry();
    const QSize size = geometry.size() / 2;
    resize(size);
    move(geometry.center() - QPoint(size.width() / 2, size.height() / 2));
}

bool TestWidget::event(QEvent * event)
{
    switch (event->type()) {
    case QEvent::Gesture: {
        const QGestureEvent *gestureEvent = static_cast<QGestureEvent *>(event);
        const GestureTypeHash::iterator hend = m_receivedGestures.end();
        for (GestureTypeHash::iterator it = m_receivedGestures.begin(); it != hend; ++it) {
            if (const QGesture *gesture = gestureEvent->gesture(it.key())) {
                if (gesture->state() == BobUI::GestureFinished)
                    it.value() = true;
            }
        }
        for (const QGesture *gesture : gestureEvent->activeGestures()) {
            switch (gesture->gestureType()) {
            case BobUI::SwipeGesture: {
                const auto *swipe = static_cast<const QSwipeGesture *>(gesture);
                lastSwipeAngle = swipe->swipeAngle();
                lastHorizontalDirection = swipe->horizontalDirection();
                lastVerticalDirection = swipe->verticalDirection();
                lastSwipeState = gesture->state();
                break;
            }
            default:
                break;
            }
        }
        break;
    }
    default:
        break;
    }
    return QWidget::event(event);
}

static void pressSequence(BOBUIest::BOBUIouchEventWidgetSequence &sequence, QList<QPoint> &points,
                          QWidget *widget)
{
    const int pointCount = points.size();
    for (int p = 0; p < pointCount; ++p)
        sequence.press(p, points.at(p), widget);
    sequence.commit();
}

static void linearSequence(int n, const QPoint &delta, BOBUIest::BOBUIouchEventWidgetSequence &sequence,
                           QList<QPoint> &points, QWidget *widget)
{
    const int pointCount = points.size();
    for (int s = 0; s < n; ++s) {
        for (int p = 0; p < pointCount; ++p) {
            points[p] += delta;
            sequence.move(p, points[p], widget);
        }
        sequence.commit();
    }
}

static void releaseSequence(BOBUIest::BOBUIouchEventWidgetSequence &sequence, QList<QPoint> &points,
                            QWidget *widget)
{
    const int pointCount = points.size();
    for (int p = 0; p < pointCount; ++p)
        sequence.release(p, points[p], widget);
    sequence.commit();
}

// --- Pan

enum PanSubTest {
    TwoFingerPanSubTest
};

void tst_QGestureRecognizer::panGesture_data()
{
    BOBUIest::addColumn<int>("panSubTest");
    BOBUIest::addColumn<bool>("gestureExpected");
    BOBUIest::newRow("Two finger") << int(TwoFingerPanSubTest) << true;
}

void tst_QGestureRecognizer::panGesture()
{
    QFETCH(int, panSubTest);
    QFETCH(bool, gestureExpected);

    Q_UNUSED(panSubTest); // Single finger pan will be added later.

    const int panPoints = 2;
    const BobUI::GestureType gestureType = BobUI::PanGesture;
    TestWidget widget(GestureTypeVector(1, gestureType));
    widget.setWindowTitle(BOBUIest::currentTestFunction());
    widget.setWindowFlag(BobUI::FramelessWindowHint);
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));

    QList<QPoint> points;
    for (int i = 0; i < panPoints; ++i)
        points.append(QPoint(10 + i *20, 10 + i *20));

    BOBUIest::BOBUIouchEventWidgetSequence panSequence = BOBUIest::touchEvent(&widget, m_touchDevice.get());
    pressSequence(panSequence, points, &widget);
    linearSequence(5, QPoint(20, 20), panSequence, points, &widget);
    releaseSequence(panSequence, points, &widget);

    if (gestureExpected) {
        BOBUIRY_VERIFY(widget.gestureReceived(gestureType));
    } else {
        QCoreApplication::processEvents();
        QVERIFY(!widget.gestureReceived(gestureType));
    }
}

// --- Pinch

enum PinchSubTest {
    StandardPinchSubTest
};

void tst_QGestureRecognizer::pinchGesture_data()
{
    BOBUIest::addColumn<int>("pinchSubTest");
    BOBUIest::addColumn<bool>("gestureExpected");
    BOBUIest::newRow("Standard") << int(StandardPinchSubTest) << true;
}

void tst_QGestureRecognizer::pinchGesture()
{
    QFETCH(int, pinchSubTest);
    QFETCH(bool, gestureExpected);

    Q_UNUSED(pinchSubTest);

    const BobUI::GestureType gestureType = BobUI::PinchGesture;
    TestWidget widget(GestureTypeVector(1, gestureType));
    widget.setWindowTitle(BOBUIest::currentTestFunction());
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));

    QList<QPoint> points;
    points.append(widget.rect().center());
    points.append(points.front() + QPoint(0, 20));

    BOBUIest::BOBUIouchEventWidgetSequence pinchSequence = BOBUIest::touchEvent(&widget, m_touchDevice.get());
    pressSequence(pinchSequence, points, &widget);

    for (int s = 0; s < 5; ++s) {
        points[0] += QPoint(5, 30);
        pinchSequence.move(0, points[0], &widget);
        points[1] += QPoint(5, -30);
        pinchSequence.move(1, points[1], &widget);
        pinchSequence.commit();
    }

    releaseSequence(pinchSequence, points, &widget);

    if (gestureExpected) {
        BOBUIRY_VERIFY(widget.gestureReceived(gestureType));
    } else {
        QCoreApplication::processEvents();
        QVERIFY(!widget.gestureReceived(gestureType));
    }
}

// --- Swipe

enum SwipeSubTest {
    SwipeLineSubTest,
    SwipeDirectionChangeSubTest,
    SwipeSmallDirectionChangeSubTest
};

void tst_QGestureRecognizer::swipeGesture_data()
{
    BOBUIest::addColumn<int>("swipeSubTest");
    BOBUIest::addColumn<QPoint>("moveDelta");
    BOBUIest::addColumn<bool>("gestureExpected");
    BOBUIest::addColumn<int>("expectedAngle");
    BOBUIest::addColumn<QSwipeGesture::SwipeDirection>("expectedHorizontalDirection");
    BOBUIest::addColumn<QSwipeGesture::SwipeDirection>("expectedVerticalDirection");

    BOBUIest::newRow("UpRight Line") << int(SwipeLineSubTest) << QPoint(42, -25)
                    << true << 30 << QSwipeGesture::Right << QSwipeGesture::Up;
    BOBUIest::newRow("DownRight Line") << int(SwipeLineSubTest) << QPoint(42, 25)
                    << true << 329 << QSwipeGesture::Right << QSwipeGesture::Down;
    BOBUIest::newRow("OutRight Line") << int(SwipeLineSubTest) << QPoint(42, 0)
                    << true << 360 << QSwipeGesture::Right << QSwipeGesture::NoDirection;
    BOBUIest::newRow("DownLeft Line") << int(SwipeLineSubTest) << QPoint(-42, 25)
                    << true << 211 << QSwipeGesture::Left << QSwipeGesture::Down;
    BOBUIest::newRow("Up Line") << int(SwipeLineSubTest) << QPoint(0, -25)
                    << true << 90 << QSwipeGesture::NoDirection << QSwipeGesture::Up;
    BOBUIest::newRow("DirectionChange") << int(SwipeDirectionChangeSubTest) << QPoint(42, 25)
                    << false << 0 << QSwipeGesture::NoDirection << QSwipeGesture::NoDirection;
    BOBUIest::newRow("SmallDirectionChange") << int(SwipeSmallDirectionChangeSubTest) << QPoint(42, -25)
                    << true << 359 << QSwipeGesture::Right << QSwipeGesture::Down;
}

void tst_QGestureRecognizer::swipeGesture()
{
    enum { swipePoints = 3 };

    QFETCH(int, swipeSubTest);
    QFETCH(QPoint, moveDelta);
    QFETCH(bool, gestureExpected);
    QFETCH(int, expectedAngle);
    QFETCH(QSwipeGesture::SwipeDirection, expectedHorizontalDirection);
    QFETCH(QSwipeGesture::SwipeDirection, expectedVerticalDirection);

    const BobUI::GestureType gestureType = BobUI::SwipeGesture;
    TestWidget widget(GestureTypeVector(1, gestureType));
    widget.setWindowTitle(BOBUIest::currentTestFunction());
    widget.setWindowFlag(BobUI::FramelessWindowHint);
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));

    // Start a swipe sequence with 2 points (BOBUIBUG-15768)
    const QPoint fingerDistance(m_fingerDistance, m_fingerDistance);
    QList<QPoint> points;
    for (int i = 1; i < swipePoints; ++i)
        points.append(fingerDistance + i * fingerDistance);

    BOBUIest::BOBUIouchEventWidgetSequence swipeSequence = BOBUIest::touchEvent(&widget, m_touchDevice.get());
    pressSequence(swipeSequence, points, &widget);

    // Move a little: nothing happens
    points[0] += {1, 1};
    points[1] += {1, 1};
    swipeSequence.move(0, points[0], &widget).move(1, points[1], &widget).commit();
    QCoreApplication::processEvents();
    QVERIFY(!widget.gestureReceived(gestureType));

    // Press point #3
    points.append(points.last() + fingerDistance);
    swipeSequence.stationary(0).stationary(1).press(points.size() - 1, points.last(), &widget);
    swipeSequence.commit();
    Q_ASSERT(points.size() == swipePoints);
    QCOMPARE(widget.lastSwipeState, BobUI::NoGesture);

    // Move.
    switch (swipeSubTest) {
    case SwipeLineSubTest:
        linearSequence(5, moveDelta, swipeSequence, points, &widget);
        break;
    case SwipeDirectionChangeSubTest:
        linearSequence(5, moveDelta, swipeSequence, points, &widget);
        linearSequence(3, QPoint(-moveDelta.x(), moveDelta.y()), swipeSequence, points, &widget);
        break;
    case SwipeSmallDirectionChangeSubTest: { // BOBUIBUG-46195, small changes in direction should not cause the gesture to be canceled.
        const QPoint smallChangeMoveDelta(50, 1);
        linearSequence(5, smallChangeMoveDelta, swipeSequence, points, &widget);
        linearSequence(1, QPoint(smallChangeMoveDelta.x(), -3), swipeSequence, points, &widget);
        linearSequence(5, smallChangeMoveDelta, swipeSequence, points, &widget);
    }
        break;
    }
    QCOMPARE(widget.lastSwipeState, BobUI::GestureUpdated);

    // release any point: the gesture ends
    swipeSequence.release(0, points[0], &widget).commit();
    if (gestureExpected) {
        BOBUIRY_VERIFY(widget.gestureReceived(gestureType));
        qCDebug(lcTests) << "started @" << fingerDistance
                         << "; ended with angle" << widget.lastSwipeAngle
                         << "expected" << expectedAngle
                         << "dirns" << widget.lastHorizontalDirection << widget.lastVerticalDirection;
        QCOMPARE(qRound(widget.lastSwipeAngle), expectedAngle);
        QEXPECT_FAIL("Up Line", "90 degrees (up) should be NoDirection on horizontal axis", Continue);
        QCOMPARE(widget.lastHorizontalDirection, expectedHorizontalDirection);
        QEXPECT_FAIL("OutRight Line", "0 degrees (to the right) should be NoDirection on the vertical axis", Continue);
        QCOMPARE(widget.lastVerticalDirection, expectedVerticalDirection);
        QCOMPARE(widget.lastSwipeState, BobUI::GestureFinished);
    } else {
        QCoreApplication::processEvents();
        QVERIFY(!widget.gestureReceived(gestureType));
        QCOMPARE(widget.lastSwipeState, BobUI::GestureUpdated);
    }

    // move the others a little, then release: no further swipe (it needs 3 fingers)
    widget.clearReceivedGestures();
    points[1] += {1, 1};
    points[2] += {1, 1};
    swipeSequence.move(1, points[1], &widget).move(2, points[2], &widget).commit();
    swipeSequence.release(1, points[1], &widget).release(2, points[2], &widget).commit();
    QCoreApplication::processEvents();
    QVERIFY(!widget.gestureReceived(gestureType));
    QCOMPARE(widget.lastSwipeState, BobUI::NoGesture);
}

void tst_QGestureRecognizer::touchReplay()
{
    const BobUI::GestureType gestureType = BobUI::TapGesture;
    const QPoint pos = QGuiApplication::primaryScreen()->availableGeometry().topLeft();
    QWidget parent;
    TestWidget widget(GestureTypeVector(1, gestureType));
    widget.setParent(&parent);
    widget.setGeometry(0, 0, 100, 100);
    parent.adjustSize();
    parent.move(pos);
    parent.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&parent));
    BOBUIRY_COMPARE(parent.pos(), pos);

    QWindow* windowHandle = parent.window()->windowHandle();
    const QPoint globalPos = QPoint(42, 16);
    BOBUIest::touchEvent(windowHandle, m_touchDevice.get()).press(1, globalPos);
    BOBUIest::touchEvent(windowHandle, m_touchDevice.get()).release(1, globalPos);

    QVERIFY(widget.gestureReceived(gestureType));
}

#endif // !BOBUI_NO_GESTURES

BOBUIEST_MAIN(tst_QGestureRecognizer)

#include "tst_qgesturerecognizer.moc"
