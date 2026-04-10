// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#include "qwindowsysteminterface.h"
#include <qpa/qplatformwindow.h>
#include "qwindowsysteminterface_p.h"
#include "private/qguiapplication_p.h"
#include "private/qevent_p.h"
#include "private/qeventpoint_p.h"
#include "private/qpointingdevice_p.h"
#include "private/qscreen_p.h"
#include <QAbstractEventDispatcher>
#include <qpa/qplatformintegration.h>
#include <qdebug.h>
#include "qhighdpiscaling_p.h"

#include <BobUICore/qscopedvaluerollback.h>
#include <BobUICore/private/qlocking_p.h>

#if BOBUI_CONFIG(draganddrop)
#include <qpa/qplatformdrag.h>
#endif

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

Q_LOGGING_CATEGORY(lcQpaInputDevices, "bobui.qpa.input.devices", BobUIWarningMsg)

Q_CONSTINIT QElapsedTimer QWindowSystemInterfacePrivate::eventTime;
bool QWindowSystemInterfacePrivate::synchronousWindowSystemEvents = false;
bool QWindowSystemInterfacePrivate::TabletEvent::platformSynthesizesMouse = true;
QWaitCondition QWindowSystemInterfacePrivate::eventsFlushed;
Q_CONSTINIT QMutex QWindowSystemInterfacePrivate::flushEventMutex;
Q_CONSTINIT QAtomicInt QWindowSystemInterfacePrivate::eventAccepted;
QWindowSystemEventHandler *QWindowSystemInterfacePrivate::eventHandler;
QWindowSystemInterfacePrivate::WindowSystemEventList QWindowSystemInterfacePrivate::windowSystemEventQueue;

extern QPointer<QWindow> bobui_last_mouse_receiver;


// ------------------- QWindowSystemInterfacePrivate -------------------

/*!
    \internal

    The QWindowSystemHelper creates synchronously delivered events on the stack, unless
    the calling thread is not the Gui thread.

    Asynchronously delivered events, and events created outside the Gui thread are
    allocated on the heap.
*/

template<typename Delivery>
struct QWindowSystemHelper
{
    template<typename EventType, typename ...Args>
    static bool handleEvent(Args ...);
};

/*
    Handles a window system event.

    By default this function posts the event on the window system event queue and
    wakes the Gui event dispatcher. BobUI Gui will then handle the event asynchronously
    at a later point. The return value is not used in asynchronous mode and will
    always be true.

    In synchronous mode BobUI Gui will process the event immediately. The return value
    indicates if BobUI accepted the event. If the event is delivered from another thread
    than the BobUI main thread the window system event queue is flushed, which may deliver
    other events as well.

    \sa flushWindowSystemEvents(), setSynchronousWindowSystemEvents()
*/
template<>
template<typename EventType, typename ...Args>
bool QWindowSystemHelper<QWindowSystemInterface::DefaultDelivery>::handleEvent(Args ...args)
{
    return QWindowSystemInterfacePrivate::synchronousWindowSystemEvents
        ? QWindowSystemHelper<QWindowSystemInterface::SynchronousDelivery>::handleEvent<EventType>(args...)
        : QWindowSystemHelper<QWindowSystemInterface::AsynchronousDelivery>::handleEvent<EventType>(args...);
}

/*
    Handles a window system event synchronously.

    BobUI Gui will process the event immediately. The return value indicates if BobUI
    accepted the event.

    If the event is delivered from another thread than the BobUI main thread the
    window system event queue is flushed, which may deliver other events as
    well.
*/
template<>
template<typename EventType, typename ...Args>
bool QWindowSystemHelper<QWindowSystemInterface::SynchronousDelivery>::handleEvent(Args ...args)
{
    if (BOBUIhread::isMainThread()) {
        EventType event(args...);
        // Process the event immediately on the Gui thread and return the accepted state
        if (QWindowSystemInterfacePrivate::eventHandler) {
            if (!QWindowSystemInterfacePrivate::eventHandler->sendEvent(&event))
                return false;
        } else {
            QGuiApplicationPrivate::processWindowSystemEvent(&event);
        }
        return event.eventAccepted;
    } else {
        // Post the event on the BobUI main thread queue and flush the queue.
        // This will wake up the Gui thread which will process the event.
        // Return the accepted state for the last event on the queue,
        // which is the event posted by this function.
        QWindowSystemHelper<QWindowSystemInterface::AsynchronousDelivery>::handleEvent<EventType>(args...);
        return QWindowSystemInterface::flushWindowSystemEvents();
    }
}

/*
    Handles a window system event asynchronously by posting the event to BobUI Gui.

    This function posts the event on the window system event queue and wakes the
    Gui event dispatcher. BobUI Gui will then handle the event asynchronously at a
    later point.
*/
template<>
template<typename EventType, typename ...Args>
bool QWindowSystemHelper<QWindowSystemInterface::AsynchronousDelivery>::handleEvent(Args ...args)
{
    QWindowSystemInterfacePrivate::windowSystemEventQueue.append(new EventType(args...));
    if (QAbstractEventDispatcher *dispatcher = QGuiApplicationPrivate::bobui_qpa_core_dispatcher())
        dispatcher->wakeUp();
    return true;
}

template <typename EventType, typename Delivery = QWindowSystemInterface::DefaultDelivery, typename ...Args>
static bool handleWindowSystemEvent(Args ...args)
{
    return QWindowSystemHelper<Delivery>::template handleEvent<EventType>(args...);
}

qsizetype QWindowSystemInterfacePrivate::windowSystemEventsQueued()
{
    return windowSystemEventQueue.count();
}

bool QWindowSystemInterfacePrivate::nonUserInputEventsQueued()
{
    return windowSystemEventQueue.nonUserInputEventsQueued();
}

QWindowSystemInterfacePrivate::WindowSystemEvent * QWindowSystemInterfacePrivate::getWindowSystemEvent()
{
    return windowSystemEventQueue.takeFirstOrReturnNull();
}

QWindowSystemInterfacePrivate::WindowSystemEvent *QWindowSystemInterfacePrivate::getNonUserInputWindowSystemEvent()
{
    return windowSystemEventQueue.takeFirstNonUserInputOrReturnNull();
}

QWindowSystemInterfacePrivate::WindowSystemEvent *QWindowSystemInterfacePrivate::peekWindowSystemEvent(EventType t)
{
    return windowSystemEventQueue.peekAtFirstOfType(t);
}

void QWindowSystemInterfacePrivate::removeWindowSystemEvent(WindowSystemEvent *event)
{
    windowSystemEventQueue.remove(event);
}

void QWindowSystemInterfacePrivate::installWindowSystemEventHandler(QWindowSystemEventHandler *handler)
{
    if (!eventHandler)
        eventHandler = handler;
}

void QWindowSystemInterfacePrivate::removeWindowSystemEventhandler(QWindowSystemEventHandler *handler)
{
    if (eventHandler == handler)
        eventHandler = nullptr;
}

QWindowSystemEventHandler::~QWindowSystemEventHandler()
{
    QWindowSystemInterfacePrivate::removeWindowSystemEventhandler(this);
}

bool QWindowSystemEventHandler::sendEvent(QWindowSystemInterfacePrivate::WindowSystemEvent *e)
{
    QGuiApplicationPrivate::processWindowSystemEvent(e);
    return true;
}

//------------------------------------------------------------
//
// Callback functions for plugins:
//

#define BOBUI_DEFINE_QPA_EVENT_HANDLER(ReturnType, HandlerName, ...) \
    template Q_GUI_EXPORT ReturnType QWindowSystemInterface::HandlerName<QWindowSystemInterface::DefaultDelivery>(__VA_ARGS__); \
    template Q_GUI_EXPORT ReturnType QWindowSystemInterface::HandlerName<QWindowSystemInterface::SynchronousDelivery>(__VA_ARGS__); \
    template Q_GUI_EXPORT ReturnType QWindowSystemInterface::HandlerName<QWindowSystemInterface::AsynchronousDelivery>(__VA_ARGS__); \
    template<typename Delivery> ReturnType QWindowSystemInterface::HandlerName(__VA_ARGS__)

/*!
    \class QWindowSystemInterface
    \since 5.0
    \internal
    \preliminary
    \ingroup qpa
    \brief The QWindowSystemInterface provides an event queue for the QPA platform.

    The platform plugins call the various functions to notify about events. The events are queued
    until sendWindowSystemEvents() is called by the event dispatcher.
*/

BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleEnterEvent, QWindow *window, const QPointF &local, const QPointF &global)
{
    if (window) {
        handleWindowSystemEvent<QWindowSystemInterfacePrivate::EnterEvent, Delivery>(window,
            QHighDpi::fromNativeLocalPosition(local, window), QHighDpi::fromNativeGlobalPosition(global, window));
    }
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleLeaveEvent, QWindow *window)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::LeaveEvent, Delivery>(window);
}

/*!
    This method can be used to ensure leave and enter events are both in queue when moving from
    one QWindow to another. This allows QWindow subclasses to check for a queued enter event
    when handling the leave event (\c QWindowSystemInterfacePrivate::peekWindowSystemEvent) to
    determine where mouse went and act accordingly. E.g. QWidgetWindow needs to know if mouse
    cursor moves between windows in same window hierarchy.
*/
void QWindowSystemInterface::handleEnterLeaveEvent(QWindow *enter, QWindow *leave, const QPointF &local, const QPointF& global)
{
    handleLeaveEvent<AsynchronousDelivery>(leave);
    handleEnterEvent(enter, local, global);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleFocusWindowChanged, QWindow *window, BobUI::FocusReason r)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::FocusWindowEvent, Delivery>(window, r);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleWindowStateChanged, QWindow *window, BobUI::WindowStates newState, int oldState)
{
    Q_ASSERT(window);
    if (oldState < BobUI::WindowNoState)
        oldState = window->windowStates();

    handleWindowSystemEvent<QWindowSystemInterfacePrivate::WindowStateChangedEvent, Delivery>(window, newState, BobUI::WindowStates(oldState));
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleWindowScreenChanged, QWindow *window, QScreen *screen)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::WindowScreenChangedEvent, Delivery>(window, screen);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleWindowDevicePixelRatioChanged, QWindow *window)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::WindowDevicePixelRatioChangedEvent, Delivery>(window);
}


BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleSafeAreaMarginsChanged, QWindow *window)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::SafeAreaMarginsChangedEvent, Delivery>(window);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleApplicationStateChanged, BobUI::ApplicationState newState, bool forcePropagate)
{
    Q_ASSERT(QGuiApplicationPrivate::platformIntegration()->hasCapability(QPlatformIntegration::ApplicationState));
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::ApplicationStateChangedEvent, Delivery>(newState, forcePropagate);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleApplicationTermination)
{
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::WindowSystemEvent, Delivery>(
        QWindowSystemInterfacePrivate::ApplicationTermination);
}

QWindowSystemInterfacePrivate::GeometryChangeEvent::GeometryChangeEvent(QWindow *window,
                                                                        QRect requestedGeometry,
                                                                        QRect newGeometry)
    : WindowSystemEvent(GeometryChange)
    , window(window)
    , requestedGeometry(requestedGeometry)
    , newGeometry(newGeometry)
{
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleGeometryChange, QWindow *window, const QRect &newRect)
{
    Q_ASSERT(window);
    const auto newRectDi = QHighDpi::fromNativeWindowGeometry(newRect, window);
    QRect requestedGeometry;
    if (auto *handle = window->handle()) {
        requestedGeometry = QHighDpi::fromNativeWindowGeometry(handle->QPlatformWindow::geometry(),
                                                               window);
        // Persist the new geometry so that QWindow::geometry() can be queried in the resize event
        handle->QPlatformWindow::setGeometry(newRect);
        // FIXME: This does not work during platform window creation, where the QWindow does not
        // have its handle set up yet. Platforms that deliver events during window creation need
        // to handle the persistence manually, e.g. by overriding geometry().
    }
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::GeometryChangeEvent, Delivery>(window,
                                                                                          requestedGeometry,
                                                                                          newRectDi);
}

QWindowSystemInterfacePrivate::ExposeEvent::ExposeEvent(QWindow *window, const QRegion &region)
    : WindowSystemEvent(Expose)
    , window(window)
    , isExposed(window && window->handle() ? window->handle()->isExposed() : false)
    , region(region)
{
}

/*! \internal
    Handles an expose event.

    The platform plugin sends expose events when an area of the window
    is invalidated or window exposure changes. \a region is in window
    local coordinates. An empty region indicates that the window is
    obscured, but note that the exposed property of the QWindow will be set
    based on what QPlatformWindow::isExposed() returns at the time of this call,
    not based on what the region is. // FIXME: this should probably be fixed.

    The platform plugin may omit sending expose events (or send obscure
    events) for windows that are on screen but where the client area is
    completely covered by other windows or otherwise not visible. Expose
    event consumers can then use this to disable updates for such windows.
    This is required behavior on platforms where OpenGL swapbuffers stops
    blocking for obscured windows (like macOS).
*/
BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleExposeEvent, QWindow *window, const QRegion &region)
{
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::ExposeEvent, Delivery>(window,
        QHighDpi::fromNativeLocalExposedRegion(region, window));
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handlePaintEvent, QWindow *window, const QRegion &region)
{
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::PaintEvent, Delivery>(window,
        QHighDpi::fromNativeLocalExposedRegion(region, window));
}


BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleCloseEvent, QWindow *window)
{
    Q_ASSERT(window);
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::CloseEvent, Delivery>(window);
}

/*!

\a w == 0 means that the event is in global coords only, \a local will be ignored in this case

*/

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleMouseEvent, QWindow *window,
                            const QPointF &local, const QPointF &global, BobUI::MouseButtons state,
                            BobUI::MouseButton button, QEvent::Type type, BobUI::KeyboardModifiers mods,
                            BobUI::MouseEventSource source)
{
    unsigned long time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleMouseEvent<Delivery>(window, time, local, global, state, button, type, mods, source);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleMouseEvent, QWindow *window, const QPointingDevice *device,
                            const QPointF &local, const QPointF &global, BobUI::MouseButtons state,
                            BobUI::MouseButton button, QEvent::Type type, BobUI::KeyboardModifiers mods,
                            BobUI::MouseEventSource source)
{
    unsigned long time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleMouseEvent<Delivery>(window, time, device, local, global, state, button, type, mods, source);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleMouseEvent, QWindow *window, ulong timestamp,
                            const QPointF &local, const QPointF &global, BobUI::MouseButtons state,
                            BobUI::MouseButton button, QEvent::Type type, BobUI::KeyboardModifiers mods,
                            BobUI::MouseEventSource source)
{
    return handleMouseEvent<Delivery>(window, timestamp, QPointingDevice::primaryPointingDevice(),
                                      local, global, state, button, type, mods, source);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleMouseEvent, QWindow *window, ulong timestamp, const QPointingDevice *device,
                            const QPointF &local, const QPointF &global, BobUI::MouseButtons state,
                            BobUI::MouseButton button, QEvent::Type type, BobUI::KeyboardModifiers mods,
                            BobUI::MouseEventSource source)
{

    bool isNonClientArea = {};

    switch (type) {
    case QEvent::MouseButtonDblClick:
    case QEvent::NonClientAreaMouseButtonDblClick:
        Q_ASSERT_X(false, "QWindowSystemInterface::handleMouseEvent",
               "BOBUIBUG-71263: Native double clicks are not implemented.");
        return false;
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        isNonClientArea = false;
        break;
    case QEvent::NonClientAreaMouseMove:
    case QEvent::NonClientAreaMouseButtonPress:
    case QEvent::NonClientAreaMouseButtonRelease:
        isNonClientArea = true;
        break;
    default:
        Q_UNREACHABLE();
    }

    auto localPos = QHighDpi::fromNativeLocalPosition(local, window);
    auto globalPos = QHighDpi::fromNativeGlobalPosition(global, window);

    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::MouseEvent, Delivery>(window,
        timestamp, localPos, globalPos, state, mods, button, type, source, isNonClientArea, device);
}

bool QWindowSystemInterface::handleShortcutEvent(QWindow *window, ulong timestamp, int keyCode, BobUI::KeyboardModifiers modifiers, quint32 nativeScanCode,
                                      quint32 nativeVirtualKey, quint32 nativeModifiers, const QString &text, bool autorepeat, ushort count)
{
#if BOBUI_CONFIG(shortcut)
    if (!window)
        window = QGuiApplication::focusWindow();

    QShortcutMap &shortcutMap = QGuiApplicationPrivate::instance()->shortcutMap;
    if (shortcutMap.state() == QKeySequence::NoMatch) {
        // Check if the shortcut is overridden by some object in the event delivery path (typically the focus object).
        // If so, we should not look up the shortcut in the shortcut map, but instead deliver the event as a regular
        // key event, so that the target that accepted the shortcut override event can handle it. Note that we only
        // do this if the shortcut map hasn't found a partial shortcut match yet. If it has, the shortcut can not be
        // overridden.
        bool overridden = handleWindowSystemEvent<QWindowSystemInterfacePrivate::KeyEvent, SynchronousDelivery>
            (window,timestamp, QEvent::ShortcutOverride, keyCode, modifiers, nativeScanCode,
             nativeVirtualKey, nativeModifiers, text, autorepeat, count);
        if (overridden)
            return false;
    }

    // The shortcut event is dispatched as a QShortcutEvent, not a QKeyEvent, but we use
    // the QKeyEvent as a container for the various properties that the shortcut map needs
    // to inspect to determine if a shortcut matched the keys that were pressed.
    QKeyEvent keyEvent(QEvent::ShortcutOverride, keyCode, modifiers, nativeScanCode,
        nativeVirtualKey, nativeModifiers, text, autorepeat, count);

    return shortcutMap.tryShortcut(&keyEvent);
#else
    Q_UNUSED(window);
    Q_UNUSED(timestamp);
    Q_UNUSED(keyCode);
    Q_UNUSED(modifiers);
    Q_UNUSED(nativeScanCode);
    Q_UNUSED(nativeVirtualKey);
    Q_UNUSED(nativeModifiers);
    Q_UNUSED(text);
    Q_UNUSED(autorepeat);
    Q_UNUSED(count);
    return false;
#endif
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleKeyEvent, QWindow *window, QEvent::Type t, int k, BobUI::KeyboardModifiers mods, const QString & text, bool autorep, ushort count) {
    unsigned long time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleKeyEvent<Delivery>(window, time, t, k, mods, text, autorep, count);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleKeyEvent, QWindow *window, ulong timestamp, QEvent::Type t, int k, BobUI::KeyboardModifiers mods, const QString & text, bool autorep, ushort count)
{
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::KeyEvent, Delivery>(window,
        timestamp, t, k, mods, text, autorep, count);
}

bool QWindowSystemInterface::handleExtendedKeyEvent(QWindow *window, QEvent::Type type, int key, BobUI::KeyboardModifiers modifiers,
                                                    quint32 nativeScanCode, quint32 nativeVirtualKey,
                                                    quint32 nativeModifiers,
                                                    const QString& text, bool autorep,
                                                    ushort count)
{
    unsigned long time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleExtendedKeyEvent(window, time, type, key, modifiers, nativeScanCode, nativeVirtualKey, nativeModifiers,
                           text, autorep, count);
}

bool QWindowSystemInterface::handleExtendedKeyEvent(QWindow *window, ulong timestamp, QEvent::Type type, int key,
                                                    BobUI::KeyboardModifiers modifiers,
                                                    quint32 nativeScanCode, quint32 nativeVirtualKey,
                                                    quint32 nativeModifiers,
                                                    const QString& text, bool autorep,
                                                    ushort count)
{
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::KeyEvent>(window,
        timestamp, type, key, modifiers, nativeScanCode, nativeVirtualKey, nativeModifiers, text, autorep, count);
}

bool QWindowSystemInterface::handleWheelEvent(QWindow *window, const QPointF &local, const QPointF &global, QPoint pixelDelta, QPoint angleDelta, BobUI::KeyboardModifiers mods, BobUI::ScrollPhase phase, BobUI::MouseEventSource source)
{
    unsigned long time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleWheelEvent(window, time, local, global, pixelDelta, angleDelta, mods, phase, source);
}

bool QWindowSystemInterface::handleWheelEvent(QWindow *window, ulong timestamp, const QPointF &local, const QPointF &global, QPoint pixelDelta, QPoint angleDelta, BobUI::KeyboardModifiers mods, BobUI::ScrollPhase phase,
                                              BobUI::MouseEventSource source, bool invertedScrolling)
{
    return handleWheelEvent(window, timestamp, QPointingDevice::primaryPointingDevice(), local, global,
                            pixelDelta, angleDelta, mods, phase, source, invertedScrolling);
}

bool QWindowSystemInterface::handleWheelEvent(QWindow *window, ulong timestamp, const QPointingDevice *device,
                                              const QPointF &local, const QPointF &global, QPoint pixelDelta, QPoint angleDelta,
                                              BobUI::KeyboardModifiers mods, BobUI::ScrollPhase phase,
                                              BobUI::MouseEventSource source, bool invertedScrolling)
{
    // BobUI 4 sends two separate wheel events for horizontal and vertical
    // deltas. For BobUI 5 we want to send the deltas in one event, but at the
    // same time preserve source and behavior compatibility with BobUI 4.
    //
    // In addition high-resolution pixel-based deltas are also supported.
    // Platforms that does not support these may pass a null point here.
    // Angle deltas must always be sent in addition to pixel deltas.

    // Pass BobUI::ScrollBegin and BobUI::ScrollEnd through
    // even if the wheel delta is null.
    if (angleDelta.isNull() && phase == BobUI::ScrollUpdate)
        return false;

    // Simple case: vertical deltas only:
    if (angleDelta.y() != 0 && angleDelta.x() == 0) {
        return handleWindowSystemEvent<QWindowSystemInterfacePrivate::WheelEvent>(window,
            timestamp, QHighDpi::fromNativeLocalPosition(local, window), QHighDpi::fromNativeGlobalPosition(global, window),
            pixelDelta, angleDelta, angleDelta.y(), BobUI::Vertical, mods, phase, source, invertedScrolling, device);
    }

    // Simple case: horizontal deltas only:
    if (angleDelta.y() == 0 && angleDelta.x() != 0) {
        return handleWindowSystemEvent<QWindowSystemInterfacePrivate::WheelEvent>(window,
            timestamp, QHighDpi::fromNativeLocalPosition(local, window), QHighDpi::fromNativeGlobalPosition(global, window),
            pixelDelta, angleDelta, angleDelta.x(), BobUI::Horizontal, mods, phase, source, invertedScrolling, device);
    }

    bool acceptVert;
    bool acceptHorz;
    // Both horizontal and vertical deltas: Send two wheel events.
    // The first event contains the BobUI 5 pixel and angle delta as points,
    // and in addition the BobUI 4 compatibility vertical angle delta.
    acceptVert = handleWindowSystemEvent<QWindowSystemInterfacePrivate::WheelEvent>(window,
        timestamp, QHighDpi::fromNativeLocalPosition(local, window), QHighDpi::fromNativeGlobalPosition(global, window),
        pixelDelta, angleDelta, angleDelta.y(), BobUI::Vertical, mods, phase, source, invertedScrolling, device);

    // The second event contains null pixel and angle points and the
    // BobUI 4 compatibility horizontal angle delta.
    acceptHorz = handleWindowSystemEvent<QWindowSystemInterfacePrivate::WheelEvent>(window,
        timestamp, QHighDpi::fromNativeLocalPosition(local, window), QHighDpi::fromNativeGlobalPosition(global, window),
        QPoint(), QPoint(), angleDelta.x(), BobUI::Horizontal, mods, phase, source, invertedScrolling, device);

    return acceptVert || acceptHorz;
}

/*!
    \internal
    Register a new input \a device.

    It is expected that every platform plugin will discover available input
    devices at startup, and whenever a new device is plugged in, if possible.
    If that's not possible, then it at least must call this function before
    sending an event whose QInputEvent::source() is this device.

    When a device is unplugged, the platform plugin should destroy the
    corresponding QInputDevice instance. There is no unregisterInputDevice()
    function, because it's enough for the destructor to call
    QInputDevicePrivate::unregisterDevice(); while other parts of BobUI can
    connect to the QObject::destroyed() signal to be notified when a device is
    unplugged or otherwise destroyed.
*/
void QWindowSystemInterface::registerInputDevice(const QInputDevice *device)
{
    QInputDevicePrivate::registerDevice(device);
}

/*!
    \internal
    Convert a list of \l QWindowSystemInterface::TouchPoint \a points to a list
    of \e temporary QEventPoint instances, scaled (but not localized)
    for delivery to the given \a window.

    This is called from QWindowSystemInterface::handleTouchEvent():
    that is too early to update the QEventPoint instances in QPointingDevice,
    because we want those to hold "current" state from the applcation's
    point of view.  The QWindowSystemInterfacePrivate::TouchEvent, to which
    the returned touchpoints will "belong", might go through the queue before
    being processed; the application doesn't see the equivalent BOBUIouchEvent
    until later on.  Therefore the responsibility to update the QEventPoint
    instances in QPointingDevice is in QGuiApplication, not here.

    QGuiApplicationPrivate::processMouseEvent() also calls this function
    when it synthesizes a touch event from a mouse event.  But that's outside
    the normal use case.

    It might be better if we change all the platform plugins to create
    temporary instances of QEventPoint directly, and remove
    QWindowSystemInterface::TouchPoint completely. Then we will no longer need
    this function either. But that's only possible as long as QEventPoint
    remains a Q_GADGET, not a QObject, so that it continues to be small and
    suitable for temporary stack allocation.  QEventPoint is a little bigger
    than QWindowSystemInterface::TouchPoint, though.
*/
QList<QEventPoint>
    QWindowSystemInterfacePrivate::fromNativeTouchPoints(const QList<QWindowSystemInterface::TouchPoint> &points,
                                                         const QWindow *window, QEvent::Type *type)
{
    QList<QEventPoint> touchPoints;
    QEventPoint::States states;

    touchPoints.reserve(points.size());
    QList<QWindowSystemInterface::TouchPoint>::const_iterator point = points.constBegin();
    QList<QWindowSystemInterface::TouchPoint>::const_iterator end = points.constEnd();
    while (point != end) {
        QPointF globalPos = QHighDpi::fromNativePixels(point->area.center(), window);
        QEventPoint p(point->id, point->state, globalPos, globalPos);
        states |= point->state;
        if (point->uniqueId >= 0)
            QMutableEventPoint::setUniqueId(p, QPointingDeviceUniqueId::fromNumericId(point->uniqueId));
        QMutableEventPoint::setPressure(p, point->pressure);
        QMutableEventPoint::setRotation(p, point->rotation);
        QMutableEventPoint::setEllipseDiameters(p, QHighDpi::fromNativePixels(point->area.size(), window));
        QMutableEventPoint::setVelocity(p, QHighDpi::fromNativePixels(point->velocity, window));

        // The local pos is not set: it will be calculated
        // when the event gets processed by QGuiApplication.

        touchPoints.append(p);
        ++point;
    }

    // Determine the event type based on the combined point states.
    if (type) {
        *type = QEvent::TouchUpdate;
        if (states == QEventPoint::State::Pressed)
            *type = QEvent::TouchBegin;
        else if (states == QEventPoint::State::Released)
            *type = QEvent::TouchEnd;
    }

    return touchPoints;
}

QWindowSystemInterface::TouchPoint
QWindowSystemInterfacePrivate::toNativeTouchPoint(const QEventPoint &pt, const QWindow *window)
{
    QWindowSystemInterface::TouchPoint p;
    p.id = pt.id();
    QRectF area(QPointF(), pt.ellipseDiameters());
    area.moveCenter(pt.globalPosition());
    // TODO store ellipseDiameters in QWindowSystemInterface::TouchPoint or just use QEventPoint
    p.area = QHighDpi::toNativePixels(area, window);
    p.pressure = pt.pressure();
    p.state = pt.state();
    p.velocity = QHighDpi::toNativePixels(pt.velocity(), window);
    return p;
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleTouchEvent, QWindow *window, const QPointingDevice *device,
                                              const QList<TouchPoint> &points, BobUI::KeyboardModifiers mods)
{
    unsigned long time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleTouchEvent<Delivery>(window, time, device, points, mods);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleTouchEvent, QWindow *window, ulong timestamp, const QPointingDevice *device,
                                              const QList<TouchPoint> &points, BobUI::KeyboardModifiers mods)
{
    if (!points.size()) // Touch events must have at least one point
        return false;

    if (!QPointingDevicePrivate::isRegistered(device)) // Disallow passing bogus, non-registered devices.
        return false;

    QEvent::Type type;
    QList<QEventPoint> touchPoints =
            QWindowSystemInterfacePrivate::fromNativeTouchPoints(points, window, &type);

    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::TouchEvent, Delivery>(window,
        timestamp, type, device, touchPoints, mods);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleTouchCancelEvent, QWindow *window, const QPointingDevice *device,
                                                    BobUI::KeyboardModifiers mods)
{
    unsigned long time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleTouchCancelEvent<Delivery>(window, time, device, mods);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleTouchCancelEvent, QWindow *window, ulong timestamp, const QPointingDevice *device,
                                                    BobUI::KeyboardModifiers mods)
{
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::TouchEvent, Delivery>(window,
        timestamp, QEvent::TouchCancel, device, QList<QEventPoint>(), mods);
}

/*!
    Should be called by the implementation whenever a new screen is added.

    The first screen added will be the primary screen, used for default-created
    windows, GL contexts, and other resources unless otherwise specified.

    This adds the screen to QGuiApplication::screens(), and emits the
    QGuiApplication::screenAdded() signal.

    The screen should be deleted by calling QWindowSystemInterface::handleScreenRemoved().
*/
void QWindowSystemInterface::handleScreenAdded(QPlatformScreen *platformScreen, bool isPrimary)
{
    QScreen *screen = new QScreen(platformScreen);

    if (isPrimary)
        QGuiApplicationPrivate::screen_list.prepend(screen);
    else
        QGuiApplicationPrivate::screen_list.append(screen);

    QGuiApplicationPrivate::resetCachedDevicePixelRatio();
    QHighDpiScaling::updateHighDpiScaling();
    screen->d_func()->updateGeometry();

    emit qGuiApp->screenAdded(screen);

    if (isPrimary)
        emit qGuiApp->primaryScreenChanged(screen);
}

/*!
    Should be called by the implementation whenever a screen is removed.

    This removes the screen from QGuiApplication::screens(), and deletes it.

    Failing to call this and manually deleting the QPlatformScreen instead may
    lead to a crash due to a pure virtual call.
*/
void QWindowSystemInterface::handleScreenRemoved(QPlatformScreen *platformScreen)
{
    QScreen *screen = platformScreen->screen();

    // Remove screen
    const bool wasPrimary = QGuiApplication::primaryScreen() == screen;
    QGuiApplicationPrivate::screen_list.removeOne(screen);
    QGuiApplicationPrivate::resetCachedDevicePixelRatio();

    if (qGuiApp) {
        QScreen *newPrimaryScreen = QGuiApplication::primaryScreen();
        if (wasPrimary && newPrimaryScreen)
            emit qGuiApp->primaryScreenChanged(newPrimaryScreen);

        // Allow clients to manage windows that are affected by the screen going
        // away, before we fall back to moving them to the primary screen.
        emit qApp->screenRemoved(screen);

        if (!QGuiApplication::closingDown()) {
            bool movingFromVirtualSibling = newPrimaryScreen
                && newPrimaryScreen->handle()->virtualSiblings().contains(platformScreen);

            // Move any leftover windows to the primary screen
            const auto allWindows = QGuiApplication::allWindows();
            for (QWindow *window : allWindows) {
                if (!window->isTopLevel() || window->screen() != screen)
                    continue;

                const bool wasVisible = window->isVisible();
                window->setScreen(newPrimaryScreen);

                // Re-show window if moved from a virtual sibling screen. Otherwise
                // leave it up to the application developer to show the window.
                if (movingFromVirtualSibling)
                    window->setVisible(wasVisible);
            }
        }
    }

    // Important to keep this order since the QSceen doesn't own the platform screen
    delete screen;
    delete platformScreen;
}

/*!
    Should be called whenever the primary screen changes.

    When the screen specified as primary changes, this method will notify
    QGuiApplication and emit the QGuiApplication::primaryScreenChanged signal.
 */
void QWindowSystemInterface::handlePrimaryScreenChanged(QPlatformScreen *newPrimary)
{
    QScreen *newPrimaryScreen = newPrimary->screen();
    qsizetype indexOfScreen = QGuiApplicationPrivate::screen_list.indexOf(newPrimaryScreen);
    Q_ASSERT(indexOfScreen >= 0);
    if (indexOfScreen == 0)
        return;

    QGuiApplicationPrivate::screen_list.swapItemsAt(0, indexOfScreen);
    emit qGuiApp->primaryScreenChanged(newPrimaryScreen);
}

void QWindowSystemInterface::handleScreenOrientationChange(QScreen *screen, BobUI::ScreenOrientation orientation)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::ScreenOrientationEvent>(screen, orientation);
}

void QWindowSystemInterface::handleScreenGeometryChange(QScreen *screen, const QRect &geometry, const QRect &availableGeometry)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::ScreenGeometryEvent>(screen,
        QHighDpi::fromNativeScreenGeometry(geometry, screen), QHighDpi::fromNative(availableGeometry,
        screen, geometry.topLeft()));
}

void QWindowSystemInterface::handleScreenLogicalDotsPerInchChange(QScreen *screen, qreal dpiX, qreal dpiY)
{
    // Keep QHighDpiScaling::m_active in sync with platform screen state, in
    // order to make scaling calls made during DPI change use the new state.
    // FIXME: Remove when QHighDpiScaling::m_active has been removed.
    QHighDpiScaling::updateHighDpiScaling();

    const QDpi effectiveDpi = QPlatformScreen::overrideDpi(QDpi{dpiX, dpiY});
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::ScreenLogicalDotsPerInchEvent>(screen,
                    effectiveDpi.first, effectiveDpi.second);
}

void QWindowSystemInterface::handleScreenRefreshRateChange(QScreen *screen, qreal newRefreshRate)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::ScreenRefreshRateEvent>(screen, newRefreshRate);
}

BOBUI_DEFINE_QPA_EVENT_HANDLER(void, handleThemeChange)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::ThemeChangeEvent, Delivery>();
}

#if BOBUI_CONFIG(draganddrop)
/*!
    Drag and drop events are sent immediately.

    ### FIXME? Perhaps DnD API should add some convenience APIs that are more
    intuitive for the possible DND operations. Here passing nullptr as drop data is used to
    indicate that drop was canceled and QDragLeaveEvent should be sent as a result.
*/
QPlatformDragBobUIResponse QWindowSystemInterface::handleDrag(QWindow *window, const QMimeData *dropData,
                                                           const QPoint &p, BobUI::DropActions supportedActions,
                                                           BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers)
{
    auto pos = QHighDpi::fromNativeLocalPosition(p, window);
    return QGuiApplicationPrivate::processDrag(window, dropData, pos, supportedActions, buttons, modifiers);
}

QPlatformDropBobUIResponse QWindowSystemInterface::handleDrop(QWindow *window, const QMimeData *dropData,
                                                           const QPoint &p, BobUI::DropActions supportedActions,
                                                           BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers)
{
    auto pos = QHighDpi::fromNativeLocalPosition(p, window);
    return QGuiApplicationPrivate::processDrop(window, dropData, pos, supportedActions, buttons, modifiers);
}
#endif // BOBUI_CONFIG(draganddrop)

/*!
    \fn static QWindowSystemInterface::handleNativeEvent(QWindow *window, const QByteArray &eventType, void *message, long *result)
    \brief Passes a native event identified by \a eventType to the \a window.

    \note This function can only be called from the GUI thread.
*/

bool QWindowSystemInterface::handleNativeEvent(QWindow *window, const QByteArray &eventType, void *message, qintptr *result)
{
    return QGuiApplicationPrivate::processNativeEvent(window, eventType, message, result);
}

void QWindowSystemInterface::handleFileOpenEvent(const QString& fileName)
{
    QWindowSystemInterfacePrivate::FileOpenEvent e(fileName);
    QGuiApplicationPrivate::processWindowSystemEvent(&e);
}

void QWindowSystemInterface::handleFileOpenEvent(const QUrl &url)
{
    QWindowSystemInterfacePrivate::FileOpenEvent e(url);
    QGuiApplicationPrivate::processWindowSystemEvent(&e);
}

void QWindowSystemInterfacePrivate::TabletEvent::setPlatformSynthesizesMouse(bool v)
{
    platformSynthesizesMouse = v;
}

bool QWindowSystemInterface::handleTabletEvent(QWindow *window, ulong timestamp, const QPointingDevice *device,
                                               const QPointF &local, const QPointF &global,
                                               BobUI::MouseButtons buttons, qreal pressure, qreal xTilt, qreal yTilt,
                                               qreal tangentialPressure, qreal rotation, int z,
                                               BobUI::KeyboardModifiers modifiers)
{
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::TabletEvent>(window,
        timestamp,
        QHighDpi::fromNativeLocalPosition(local, window),
        QHighDpi::fromNativeGlobalPosition(global, window),
        device, buttons, pressure,
        xTilt, yTilt, tangentialPressure, rotation, z, modifiers);
}

bool QWindowSystemInterface::handleTabletEvent(QWindow *window, const QPointingDevice *device,
                                               const QPointF &local, const QPointF &global,
                                               BobUI::MouseButtons buttons, qreal pressure, qreal xTilt, qreal yTilt,
                                               qreal tangentialPressure, qreal rotation, int z,
                                               BobUI::KeyboardModifiers modifiers)
{
    const ulong time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleTabletEvent(window, time, device, local, global,
                             buttons, pressure, xTilt, yTilt, tangentialPressure,
                             rotation, z, modifiers);
}

bool QWindowSystemInterface::handleTabletEvent(QWindow *window, ulong timestamp, const QPointF &local, const QPointF &global,
                                               int device, int pointerType, BobUI::MouseButtons buttons, qreal pressure, qreal xTilt, qreal yTilt,
                                               qreal tangentialPressure, qreal rotation, int z, qint64 uid,
                                               BobUI::KeyboardModifiers modifiers)
{
    const QPointingDevice *dev = QPointingDevicePrivate::tabletDevice(QInputDevice::DeviceType(device),QPointingDevice::PointerType(pointerType),
                                                                      QPointingDeviceUniqueId::fromNumericId(uid));
    return handleTabletEvent(window, timestamp, dev, local, global, buttons, pressure,
                      xTilt, yTilt, tangentialPressure, rotation, z, modifiers);
}

bool QWindowSystemInterface::handleTabletEvent(QWindow *window, const QPointF &local, const QPointF &global,
                                               int device, int pointerType, BobUI::MouseButtons buttons, qreal pressure, qreal xTilt, qreal yTilt,
                                               qreal tangentialPressure, qreal rotation, int z, qint64 uid,
                                               BobUI::KeyboardModifiers modifiers)
{
    ulong time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleTabletEvent(window, time, local, global, device, pointerType, buttons, pressure,
                      xTilt, yTilt, tangentialPressure, rotation, z, uid, modifiers);
}

bool QWindowSystemInterface::handleTabletEnterLeaveProximityEvent(QWindow *window, ulong timestamp, const QPointingDevice *device,
                                                                  bool inProximity, const QPointF &local, const QPointF &global,
                                                                  BobUI::MouseButtons buttons, qreal xTilt, qreal yTilt,
                                                                  qreal tangentialPressure, qreal rotation, int z,
                                                                  BobUI::KeyboardModifiers modifiers)
{
    Q_UNUSED(window);
    Q_UNUSED(local);
    Q_UNUSED(global);
    Q_UNUSED(buttons);
    Q_UNUSED(xTilt);
    Q_UNUSED(yTilt);
    Q_UNUSED(tangentialPressure);
    Q_UNUSED(rotation);
    Q_UNUSED(z);
    Q_UNUSED(modifiers);
    return inProximity
        ? handleWindowSystemEvent<QWindowSystemInterfacePrivate::TabletEnterProximityEvent>(timestamp, device)
        : handleWindowSystemEvent<QWindowSystemInterfacePrivate::TabletLeaveProximityEvent>(timestamp, device);
}

bool QWindowSystemInterface::handleTabletEnterLeaveProximityEvent(QWindow *window, const QPointingDevice *device,
                                                                  bool inProximity, const QPointF &local, const QPointF &global,
                                                                  BobUI::MouseButtons buttons, qreal xTilt, qreal yTilt,
                                                                  qreal tangentialPressure, qreal rotation, int z,
                                                                  BobUI::KeyboardModifiers modifiers)
{
    const ulong time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    return handleTabletEnterLeaveProximityEvent(window, time, device, inProximity,
                                                local, global, buttons, xTilt, yTilt,
                                                tangentialPressure, rotation, z, modifiers);
}


bool QWindowSystemInterface::handleTabletEnterProximityEvent(ulong timestamp, int deviceType, int pointerType, qint64 uid)
{
    const QPointingDevice *device = QPointingDevicePrivate::tabletDevice(QInputDevice::DeviceType(deviceType),
                                                                         QPointingDevice::PointerType(pointerType),
                                                                         QPointingDeviceUniqueId::fromNumericId(uid));
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::TabletEnterProximityEvent>(timestamp, device);
}

void QWindowSystemInterface::handleTabletEnterProximityEvent(int deviceType, int pointerType, qint64 uid)
{
    ulong time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    handleTabletEnterProximityEvent(time, deviceType, pointerType, uid);
}

bool QWindowSystemInterface::handleTabletLeaveProximityEvent(ulong timestamp, int deviceType, int pointerType, qint64 uid)
{
    const QPointingDevice *device = QPointingDevicePrivate::tabletDevice(QInputDevice::DeviceType(deviceType),
                                                                         QPointingDevice::PointerType(pointerType),
                                                                         QPointingDeviceUniqueId::fromNumericId(uid));
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::TabletLeaveProximityEvent>(timestamp, device);
}

void QWindowSystemInterface::handleTabletLeaveProximityEvent(int deviceType, int pointerType, qint64 uid)
{
    ulong time = QWindowSystemInterfacePrivate::eventTime.elapsed();
    handleTabletLeaveProximityEvent(time, deviceType, pointerType, uid);
}

#ifndef BOBUI_NO_GESTURES
bool QWindowSystemInterface::handleGestureEvent(QWindow *window, ulong timestamp, const QPointingDevice *device,
                                                BobUI::NativeGestureType type, const QPointF &local, const QPointF &global, int fingerCount)
{
    return handleGestureEventWithValueAndDelta(window, timestamp, device, type, {}, {}, local, global, fingerCount);
}

bool QWindowSystemInterface::handleGestureEventWithRealValue(QWindow *window, ulong timestamp, const QPointingDevice *device,
                                                             BobUI::NativeGestureType type, qreal value, const QPointF &local, const QPointF &global, int fingerCount)
{
    return handleGestureEventWithValueAndDelta(window, timestamp, device, type, value, {}, local, global, fingerCount);
}

bool QWindowSystemInterface::handleGestureEventWithValueAndDelta(QWindow *window, ulong timestamp, const QPointingDevice *device,
                                                                 BobUI::NativeGestureType type, qreal value, const QPointF &delta,
                                                                 const QPointF &local, const QPointF &global, int fingerCount)
{
    auto localPos = QHighDpi::fromNativeLocalPosition(local, window);
    auto globalPos = QHighDpi::fromNativeGlobalPosition(global, window);

    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::GestureEvent>(window,
        timestamp, type, device, fingerCount, localPos, globalPos, value, delta);
}
#endif // BOBUI_NO_GESTURES

void QWindowSystemInterface::handlePlatformPanelEvent(QWindow *w)
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::PlatformPanelEvent>(w);
}

#ifndef BOBUI_NO_CONTEXTMENU
BOBUI_DEFINE_QPA_EVENT_HANDLER(bool, handleContextMenuEvent, QWindow *window, bool mouseTriggered,
                                                    const QPoint &pos, const QPoint &globalPos,
                                                    BobUI::KeyboardModifiers modifiers)
{
    return handleWindowSystemEvent<QWindowSystemInterfacePrivate::ContextMenuEvent, Delivery>(
        window, mouseTriggered, pos, globalPos, modifiers);
}
#endif

#if BOBUI_CONFIG(whatsthis)
void QWindowSystemInterface::handleEnterWhatsThisEvent()
{
    handleWindowSystemEvent<QWindowSystemInterfacePrivate::WindowSystemEvent>(
        QWindowSystemInterfacePrivate::EnterWhatsThisMode);
}
#endif

#ifndef BOBUI_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug dbg, const QWindowSystemInterface::TouchPoint &p)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "TouchPoint(" << p.id << " @" << p.area << " normalized " << p.normalPosition
                  << " press " << p.pressure << " vel " << p.velocity << " state " << (int)p.state;
    return dbg;
}
#endif

// ------------------ Event dispatcher functionality ------------------

/*!
    Make BobUI Gui process all events on the event queue immediately. Return the
    accepted state for the last event on the queue.
*/
bool QWindowSystemInterface::flushWindowSystemEvents(QEventLoop::ProcessEventsFlags flags)
{
    const qsizetype count = QWindowSystemInterfacePrivate::windowSystemEventQueue.count();
    if (!count)
        return false;
    if (!QGuiApplication::instance()) {
        qWarning().nospace()
            << "QWindowSystemInterface::flushWindowSystemEvents() invoked after "
               "QGuiApplication destruction, discarding " << count << " events.";
        QWindowSystemInterfacePrivate::windowSystemEventQueue.clear();
        return false;
    }
    if (BOBUIhread::currentThread() != QGuiApplication::instance()->thread()) {
        // Post a FlushEvents event which will trigger a call back to
        // deferredFlushWindowSystemEvents from the Gui thread.
        QMutexLocker locker(&QWindowSystemInterfacePrivate::flushEventMutex);
        handleWindowSystemEvent<QWindowSystemInterfacePrivate::FlushEventsEvent, AsynchronousDelivery>(flags);
        QWindowSystemInterfacePrivate::eventsFlushed.wait(&QWindowSystemInterfacePrivate::flushEventMutex);
    } else {
        sendWindowSystemEvents(flags);
    }
    return QWindowSystemInterfacePrivate::eventAccepted.loadRelaxed() > 0;
}

void QWindowSystemInterface::deferredFlushWindowSystemEvents(QEventLoop::ProcessEventsFlags flags)
{
    Q_ASSERT(BOBUIhread::currentThread() == QGuiApplication::instance()->thread());

    QMutexLocker locker(&QWindowSystemInterfacePrivate::flushEventMutex);
    sendWindowSystemEvents(flags);
    QWindowSystemInterfacePrivate::eventsFlushed.wakeOne();
}

bool QWindowSystemInterface::sendWindowSystemEvents(QEventLoop::ProcessEventsFlags flags)
{
    int nevents = 0;

    while (QWindowSystemInterfacePrivate::windowSystemEventsQueued()) {
        QWindowSystemInterfacePrivate::WindowSystemEvent *event =
                flags & QEventLoop::ExcludeUserInputEvents ?
                        QWindowSystemInterfacePrivate::getNonUserInputWindowSystemEvent() :
                        QWindowSystemInterfacePrivate::getWindowSystemEvent();
        if (!event)
            break;

        if (QWindowSystemInterfacePrivate::eventHandler) {
            if (QWindowSystemInterfacePrivate::eventHandler->sendEvent(event))
                nevents++;
        } else {
            nevents++;
            QGuiApplicationPrivate::processWindowSystemEvent(event);
        }

        // Record the accepted state for the processed event
        // (excluding flush events). This state can then be
        // returned by flushWindowSystemEvents().
        if (event->type != QWindowSystemInterfacePrivate::FlushEvents)
            QWindowSystemInterfacePrivate::eventAccepted.storeRelaxed(event->eventAccepted);

        delete event;
    }

    return (nevents > 0);
}

void QWindowSystemInterface::setSynchronousWindowSystemEvents(bool enable)
{
    QWindowSystemInterfacePrivate::synchronousWindowSystemEvents = enable;
}

int QWindowSystemInterface::windowSystemEventsQueued()
{
    return QWindowSystemInterfacePrivate::windowSystemEventsQueued();
}

bool QWindowSystemInterface::nonUserInputEventsQueued()
{
    return QWindowSystemInterfacePrivate::nonUserInputEventsQueued();
}

// --------------------- BobUITestLib support ---------------------

// The following functions are used by testlib, and need to be synchronous to avoid
// race conditions with plugins delivering native events from secondary threads.
// FIXME: It seems unnecessary to export these wrapper functions, when bobuiestlib could access
// QWindowSystemInterface directly (by adding dependency to gui-private), see BOBUIBUG-63146.

Q_GUI_EXPORT void bobui_handleMouseEvent(QWindow *window, const QPointF &local, const QPointF &global,
                                      BobUI::MouseButtons state, BobUI::MouseButton button,
                                      QEvent::Type type, BobUI::KeyboardModifiers mods, int timestamp)
{
    QPointF nativeLocal = QHighDpi::toNativeLocalPosition(local, window);
    QPointF nativeGlobal = QHighDpi::toNativeGlobalPosition(global, window);
    QWindowSystemInterface::handleMouseEvent<QWindowSystemInterface::SynchronousDelivery>(window,
                timestamp, nativeLocal, nativeGlobal, state, button, type, mods);
}

/*
    Used by BOBUIest::simulateEvent() to synthesize key events during testing
*/
Q_GUI_EXPORT void bobui_handleKeyEvent(QWindow *window, QEvent::Type t, int k, BobUI::KeyboardModifiers mods, const QString & text = QString(), bool autorep = false, ushort count = 1)
{
#if defined(Q_OS_MACOS)
    // FIXME: Move into BOBUIest::simulateEvent() and align with QGuiApplicationPrivate::processKeyEvent()
    auto timestamp = QWindowSystemInterfacePrivate::eventTime.elapsed();
    if (t == QEvent::KeyPress && QWindowSystemInterface::handleShortcutEvent(window, timestamp, k, mods, 0, 0, 0, text, autorep, count))
        return;
#endif

    QWindowSystemInterface::handleKeyEvent<QWindowSystemInterface::SynchronousDelivery>(window, t, k, mods, text, autorep, count);
}

Q_GUI_EXPORT bool bobui_sendShortcutOverrideEvent(QObject *o, ulong timestamp, int k, BobUI::KeyboardModifiers mods, const QString &text = QString(), bool autorep = false, ushort count = 1)
{
#if BOBUI_CONFIG(shortcut)

    // FIXME: This method should not allow targeting a specific object, but should
    // instead forward the event to a window, which then takes care of normal event
    // propagation. We need to fix a lot of tests before we can refactor this (the
    // window needs to be exposed and active and have a focus object), so we leave
    // it as is for now. See BOBUIBUG-48577.

    QGuiApplicationPrivate::modifier_buttons = mods;

    QKeyEvent qevent(QEvent::ShortcutOverride, k, mods, text, autorep, count);
    qevent.setTimestamp(timestamp);

    QShortcutMap &shortcutMap = QGuiApplicationPrivate::instance()->shortcutMap;
    if (shortcutMap.state() == QKeySequence::NoMatch) {
        // Try sending as QKeyEvent::ShortcutOverride first
        QCoreApplication::sendEvent(o, &qevent);
        if (qevent.isAccepted())
            return false;
    }

    // Then as QShortcutEvent
    return shortcutMap.tryShortcut(&qevent);
#else
    Q_UNUSED(o);
    Q_UNUSED(timestamp);
    Q_UNUSED(k);
    Q_UNUSED(mods);
    Q_UNUSED(text);
    Q_UNUSED(autorep);
    Q_UNUSED(count);
    return false;
#endif
}

Q_GUI_EXPORT void bobui_handleWheelEvent(QWindow *window, const QPointF &local, const QPointF &global,
                                      QPoint pixelDelta, QPoint angleDelta, BobUI::KeyboardModifiers mods,
                                      BobUI::ScrollPhase phase)
{
    QWindowSystemInterface::handleWheelEvent(window, local, global, pixelDelta, angleDelta, mods, phase);
}

namespace BOBUIest
{
    Q_GUI_EXPORT QPointingDevice * createTouchDevice(QInputDevice::DeviceType devType,
                                                     QInputDevice::Capabilities caps)
    {
        static qint64 nextId = 0x100000000;
        QPointingDevice *ret = new QPointingDevice("test touch device"_L1, nextId++,
                                                   devType, QPointingDevice::PointerType::Finger,
                                                   caps, 8, 0);
        QWindowSystemInterface::registerInputDevice(ret);
        return ret;
    }
}

Q_GUI_EXPORT bool bobui_handleTouchEventv2(QWindow *window, const QPointingDevice *device,
                                const QList<QEventPoint> &points,
                                BobUI::KeyboardModifiers mods)
{
    return QWindowSystemInterface::handleTouchEvent<QWindowSystemInterface::SynchronousDelivery>(window, device,
        QWindowSystemInterfacePrivate::toNativeTouchPoints(points, window), mods);
}

Q_GUI_EXPORT void bobui_handleTouchEvent(QWindow *window, const QPointingDevice *device,
                                const QList<QEventPoint> &points,
                                BobUI::KeyboardModifiers mods)
{
    bobui_handleTouchEventv2(window, device, points, mods);
}

BOBUI_END_NAMESPACE
