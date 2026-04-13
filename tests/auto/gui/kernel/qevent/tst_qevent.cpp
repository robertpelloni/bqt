// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include <BobUIGui/qguiapplication.h>
#include <BobUIGui/qevent.h>
#if BOBUI_CONFIG(future)
#include <BobUICore/private/qfutureinterface_p.h>
#endif


#if BOBUI_CONFIG(future)
#define X_QFutureCallOutEvent(X) X(QFutureCallOutEvent, ())
#else
#define X_QFutureCallOutEvent(X)
#endif

#if BOBUI_CONFIG(wheelevent)
#define X_QWheelEvent(X) X(QWheelEvent, ({}, {}, {}, {}, {}, {}, {}, {}))
#else
#define X_QWheelEvent(X)
#endif

#if BOBUI_CONFIG(tabletevent)
#define X_BOBUIabletEvent(X) X(BOBUIabletEvent, (QEvent::None, QPointingDevice::primaryPointingDevice(), {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}))
#else
#define X_BOBUIabletEvent(X)
#endif

#if BOBUI_CONFIG(gestures)
#define X_QNativeGestureEvent(X) X(QNativeGestureEvent, ({}, QPointingDevice::primaryPointingDevice(), 0, {}, {}, {}, {}, {}))
#else
#define X_QNativeGestureEvent(X)
#endif

#if BOBUI_CONFIG(whatsthis)
#define X_QWhatsThisClickedEvent(X) X(QWhatsThisClickedEvent, ({}))
#else
#define X_QWhatsThisClickedEvent(X)
#endif

#if BOBUI_CONFIG(action)
#define X_QActionEvent(X) X(QActionEvent, (0, nullptr))
#else
#define X_QActionEvent(X)
#endif

#if BOBUI_CONFIG(shortcut)
#define X_QShortcutEvent(X) X(QShortcutEvent, ({}, 0))
#else
#define X_QShortcutEvent(X)
#endif

#if BOBUI_CONFIG(draganddrop)
#define X_QDropEvent(X) X(QDropEvent, ({}, {}, {}, {}, {}))
#define X_QDragMoveEvent(X) X(QDragMoveEvent, ({}, {}, {}, {}, {}))
#define X_QDragEnterEvent(X) X(QDragEnterEvent, ({}, {}, {}, {}, {}))
#define X_QDragLeaveEvent(X) X(QDragLeaveEvent, ())
#else
#define X_QDropEvent(X)
#define X_QDragMoveEvent(X)
#define X_QDragEnterEvent(X)
#define X_QDragLeaveEvent(X)
#endif

#define FOR_EACH_CORE_EVENT(X) \
    /* qcoreevent.h */ \
    X(QEvent, (QEvent::None)) \
    X(BOBUIimerEvent, (BobUI::TimerId{42})) \
    X(QChildEvent, (QEvent::ChildAdded, nullptr)) \
    X(QDynamicPropertyChangeEvent, ("size")) \
    /* qfutureinterface_p.h */ \
    X_QFutureCallOutEvent(X) \
    /* end */

#define FOR_EACH_GUI_EVENT(X) \
    /* qevent.h */ \
    X(QInputEvent, (QEvent::None, nullptr)) \
    X(QPointerEvent, (QEvent::None, nullptr)) \
    /* doesn't work with nullptr: */ \
    X(QSinglePointEvent, (QEvent::None, QPointingDevice::primaryPointingDevice(), {}, {}, {}, {}, {}, {})) \
    X(QEnterEvent, ({}, {}, {})) \
    X(QMouseEvent, (QEvent::None, {}, {}, {}, {}, {}, {}, {}, QPointingDevice::primaryPointingDevice())) \
    X(QHoverEvent, (QEvent::None, {}, {}, QPointF{})) \
    X_QWheelEvent(X) \
    X_BOBUIabletEvent(X) \
    X_QNativeGestureEvent(X) \
    X(QKeyEvent, (QEvent::None, 0, {})) \
    X(QFocusEvent, (QEvent::None)) \
    X(QPaintEvent, (QRect{0, 0, 100, 100})) \
    X(QMoveEvent, ({}, {})) \
    X(QExposeEvent, ({})) \
    X(QPlatformSurfaceEvent, ({})) \
    X(QResizeEvent, ({}, {})) \
    X(QCloseEvent, ()) \
    X(QIconDragEvent, ()) \
    X(QShowEvent, ()) \
    X(QHideEvent, ()) \
    X(QContextMenuEvent, (QContextMenuEvent::Reason::Keyboard, {}, {})) \
    X(QInputMethodEvent, ()) \
    X(QInputMethodQueryEvent, ({})) \
    X_QDropEvent(X) \
    X_QDragMoveEvent(X) \
    X_QDragEnterEvent(X) \
    X_QDragLeaveEvent(X) \
    X(QHelpEvent, ({}, {}, {})) \
    X(QStatusTipEvent, ({})) \
    X_QWhatsThisClickedEvent(X) \
    X_QActionEvent(X) \
    X(QFileOpenEvent, (QString{})) \
    X(BOBUIoolBarChangeEvent, (false)) \
    X_QShortcutEvent(X) \
    X(QWindowStateChangeEvent, ({})) \
    X(BOBUIouchEvent, (QEvent::None)) \
    X(QScrollPrepareEvent, ({})) \
    X(QScrollEvent, ({}, {}, {})) \
    X(QScreenOrientationChangeEvent, (nullptr, {})) \
    X(QApplicationStateChangeEvent, ({})) \
    /* end */

#define FOR_EACH_EVENT(X) \
    FOR_EACH_CORE_EVENT(X) \
    FOR_EACH_GUI_EVENT(X) \
    /* end */

class tst_QEvent : public QObject
{
    Q_OBJECT
public:
    tst_QEvent();
    ~tst_QEvent();

private slots:
    void clone() const;
    void registerEventType_data();
    void registerEventType();
    void exhaustEventTypeRegistration(); // keep behind registerEventType() test

private:
    bool registerEventTypeSucceeded; // track success of registerEventType for use by exhaustEventTypeRegistration()
};

tst_QEvent::tst_QEvent()
    : registerEventTypeSucceeded(true)
{ }

tst_QEvent::~tst_QEvent()
{ }

void tst_QEvent::clone() const
{
#define ACTION(Type, Init) do { \
        const std::unique_ptr<const Type> e(new Type Init); \
        auto c = e->clone(); \
        static_assert(std::is_same_v<decltype(c), Type *>); \
        delete c; \
    } while (0);

    FOR_EACH_EVENT(ACTION)
}

void tst_QEvent::registerEventType_data()
{
    BOBUIest::addColumn<int>("hint");
    BOBUIest::addColumn<int>("expected");

    // default argument
    BOBUIest::newRow("default") << -1 << int(QEvent::MaxUser);
    // hint not valid
    BOBUIest::newRow("User-1") << int(QEvent::User - 1) << int(QEvent::MaxUser - 1);
    // hint not valid II
    BOBUIest::newRow("MaxUser+1") << int(QEvent::MaxUser + 1) << int(QEvent::MaxUser - 2);
    // hint valid, but already taken
    BOBUIest::newRow("MaxUser-1") << int(QEvent::MaxUser - 1) << int(QEvent::MaxUser - 3);
    // hint valid, but not taken
    BOBUIest::newRow("User + 1000") << int(QEvent::User + 1000) << int(QEvent::User + 1000);
}

void tst_QEvent::registerEventType()
{
    const bool oldRegisterEventTypeSucceeded = registerEventTypeSucceeded;
    registerEventTypeSucceeded = false;
    QFETCH(int, hint);
    QFETCH(int, expected);
    QCOMPARE(QEvent::registerEventType(hint), expected);
    registerEventTypeSucceeded = oldRegisterEventTypeSucceeded;
}

void tst_QEvent::exhaustEventTypeRegistration()
{
    if (!registerEventTypeSucceeded)
        QSKIP("requires the previous test (registerEventType) to have finished successfully");

    int i = QEvent::User;
    int result;
    while ((result = QEvent::registerEventType(i)) == i)
        ++i;
    QCOMPARE(i, int(QEvent::User + 1000));
    QCOMPARE(result, int(QEvent::MaxUser - 4));
    i = QEvent::User + 1001;
    while ((result = QEvent::registerEventType(i)) == i)
        ++i;
    QCOMPARE(result, -1);
    QCOMPARE(i, int(QEvent::MaxUser - 4));
}

BOBUIEST_MAIN(tst_QEvent)
#include "tst_qevent.moc"
