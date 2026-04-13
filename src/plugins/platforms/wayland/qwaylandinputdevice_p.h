// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2024 Jie Liu <liujie01@kylinos.cn>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDINPUTDEVICE_H
#define QWAYLANDINPUTDEVICE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWaylandClient/private/bobuiwaylandclientglobal_p.h>
#include <BobUIWaylandClient/private/qwaylandwindow_p.h>

#include <BobUICore/QScopedPointer>
#include <QSocketNotifier>
#include <QObject>
#include <BOBUIimer>
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformscreen.h>
#include <qpa/qwindowsysteminterface.h>

#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include <BobUIWaylandClient/private/qwayland-pointer-gestures-unstable-v1.h>

#if BOBUI_CONFIG(xkbcommon)
#include <BobUIGui/private/qxkbcommon_p.h>
#endif

#include <BobUICore/QDebug>
#include <BobUICore/QElapsedTimer>
#include <BobUICore/QPointer>

#if BOBUI_CONFIG(cursor)
struct wl_cursor_image;
#endif

BOBUI_BEGIN_NAMESPACE

namespace BobUIWayland {
class zwp_primary_selection_device_v1;
} //namespace BobUIWayland

namespace BobUIWaylandClient {

class QWaylandDataDevice;
class QWaylandDisplay;
#if BOBUI_CONFIG(clipboard)
class QWaylandDataControlDeviceV1;
#endif
#if BOBUI_CONFIG(wayland_client_primary_selection)
class QWaylandPrimarySelectionDeviceV1;
#endif
#if BOBUI_CONFIG(tabletevent)
class QWaylandTabletSeatV2;
#endif
class QWaylandPointerGestures;
class QWaylandPointerGestureSwipe;
class QWaylandPointerGesturePinch;
class QWaylandTextInputInterface;
class QWaylandTextInputMethod;
#if BOBUI_CONFIG(cursor)
class QWaylandCursorTheme;
class QWaylandCursorShape;
template <typename T>
class CursorSurface;
#endif

Q_DECLARE_LOGGING_CATEGORY(lcQpaWaylandInput);

struct QWaylandEventCompressionPrivate
{
    QWaylandEventCompressionPrivate();

    bool compressEvent();

    QElapsedTimer timeElapsed;
    BOBUIimer delayTimer;
};

class Q_WAYLANDCLIENT_EXPORT QWaylandInputDevice
                            : public QObject
                            , public BobUIWayland::wl_seat
{
    Q_OBJECT
public:
    class Keyboard;
    class Pointer;
    class Touch;

    QWaylandInputDevice(QWaylandDisplay *display, int version, uint32_t id);
    ~QWaylandInputDevice() override;

    uint32_t id() const { return mId; }
    uint32_t capabilities() const { return mCaps; }
    QString seatname()  const { return mSeatName; }

    QWaylandDisplay *display() const { return mQDisplay; }
    struct ::wl_seat *wl_seat() { return BobUIWayland::wl_seat::object(); }

#if BOBUI_CONFIG(cursor)
    void setCursor(const QCursor *cursor, const QSharedPointer<QWaylandBuffer> &cachedBuffer = {}, int fallbackOutputScale = 1);
#endif
    void handleStartDrag();
    void handleEndDrag();

#if BOBUI_CONFIG(wayland_datadevice)
    void setDataDevice(QWaylandDataDevice *device);
    QWaylandDataDevice *dataDevice() const;
#endif

#if BOBUI_CONFIG(clipboard)
    void setDataControlDevice(QWaylandDataControlDeviceV1 *dataControlDevice);
    QWaylandDataControlDeviceV1 *dataControlDevice() const;
#endif

#if BOBUI_CONFIG(wayland_client_primary_selection)
    void setPrimarySelectionDevice(QWaylandPrimarySelectionDeviceV1 *primarySelectionDevice);
    QWaylandPrimarySelectionDeviceV1 *primarySelectionDevice() const;
#endif

#if BOBUI_CONFIG(tabletevent)
    void setTabletSeat(QWaylandTabletSeatV2 *tabletSeat);
    QWaylandTabletSeatV2* tabletSeat() const;
#endif

    void setTextInput(QWaylandTextInputInterface *textInput);
    QWaylandTextInputInterface *textInput() const;

    void setTextInputMethod(QWaylandTextInputMethod *textInputMethod);
    QWaylandTextInputMethod *textInputMethod() const;

    void removeMouseButtonFromState(BobUI::MouseButton button);

    QWaylandWindow *pointerFocus() const;
    QWaylandWindow *keyboardFocus() const;
    QWaylandWindow *touchFocus() const;

    QList<int> possibleKeys(const QKeyEvent *event) const;

    QPointF pointerSurfacePosition() const;

    BobUI::KeyboardModifiers modifiers() const;

    uint32_t serial() const;

    virtual Keyboard *createKeyboard(QWaylandInputDevice *device);
    virtual Pointer *createPointer(QWaylandInputDevice *device);
    virtual Touch *createTouch(QWaylandInputDevice *device);

    Keyboard *keyboard() const;
    Pointer *pointer() const;
    QWaylandPointerGestureSwipe *pointerGestureSwipe() const;
    QWaylandPointerGesturePinch *pointerGesturePinch() const;
    Touch *touch() const;

protected:
    QWaylandDisplay *mQDisplay = nullptr;
    struct wl_display *mDisplay = nullptr;

    uint32_t mId = -1;
    uint32_t mCaps = 0;
    QString mSeatName;

#if BOBUI_CONFIG(cursor)
    struct CursorState {
        QSharedPointer<QWaylandBuffer> bitmapBuffer; // not used with shape cursors
        int bitmapScale = 1;
        BobUI::CursorShape shape = BobUI::ArrowCursor;
        int fallbackOutputScale = 1;
        QPoint hotspot;
        QElapsedTimer animationTimer;
    } mCursor;
#endif

#if BOBUI_CONFIG(wayland_datadevice)
    QWaylandDataDevice *mDataDevice = nullptr;
#endif

#if BOBUI_CONFIG(clipboard)
    QScopedPointer<QWaylandDataControlDeviceV1> mDataControlDevice;
#endif

#if BOBUI_CONFIG(wayland_client_primary_selection)
    QScopedPointer<QWaylandPrimarySelectionDeviceV1> mPrimarySelectionDevice;
#endif

    QScopedPointer<Keyboard> mKeyboard;
    QScopedPointer<Pointer> mPointer;
    QScopedPointer<QWaylandPointerGestureSwipe> mPointerGestureSwipe;
    QScopedPointer<QWaylandPointerGesturePinch> mPointerGesturePinch;
    QScopedPointer<Touch> mTouch;

    QScopedPointer<QWaylandTextInputInterface> mTextInput;
    QScopedPointer<QWaylandTextInputMethod> mTextInputMethod;
#if BOBUI_CONFIG(tabletevent)
    QScopedPointer<QWaylandTabletSeatV2> mTabletSeat;
#endif

    uint32_t mTime = 0;
    uint32_t mSerial = 0;

    void seat_capabilities(uint32_t caps) override;
    void seat_name(const QString &name) override;
    void handleTouchPoint(int id, QEventPoint::State state, const QPointF &surfacePosition = QPoint());

    QPointingDevice *mTouchDevice = nullptr;
    QPointingDevice *mTouchPadDevice = nullptr;

    friend class QWaylandTouchExtension;
    friend class QWaylandBobUIKeyExtension;
    friend class QWaylandPointerGestureSwipe;
    friend class QWaylandPointerGesturePinch;
    friend class QWaylandWindow;
    friend class QWaylandTabletToolV2;
};

inline uint32_t QWaylandInputDevice::serial() const
{
    return mSerial;
}


class Q_WAYLANDCLIENT_EXPORT QWaylandInputDevice::Keyboard : public QObject, public BobUIWayland::wl_keyboard
{
    Q_OBJECT

public:
    Keyboard(QWaylandInputDevice *p);
    ~Keyboard() override;

    QWaylandWindow *focusWindow() const;

    void keyboard_keymap(uint32_t format,
                         int32_t fd,
                         uint32_t size) override;
    void keyboard_enter(uint32_t time,
                        struct wl_surface *surface,
                        struct wl_array *keys) override;
    void keyboard_leave(uint32_t time,
                        struct wl_surface *surface) override;
    void keyboard_key(uint32_t serial, uint32_t time,
                      uint32_t key, uint32_t state) override;
    void keyboard_modifiers(uint32_t serial,
                            uint32_t mods_depressed,
                            uint32_t mods_latched,
                            uint32_t mods_locked,
                            uint32_t group) override;
    void keyboard_repeat_info(int32_t rate, int32_t delay) override;

    QWaylandInputDevice *mParent = nullptr;
    QPointer<QWaylandSurface> mFocus;

    uint32_t mNativeModifiers = 0;

    struct repeatKey {
        int key = 0;
        uint32_t code = 0;
        uint32_t time = 0 ;
        QString text;
        uint32_t nativeVirtualKey = 0;
    } mRepeatKey;

    BOBUIimer mRepeatTimer;
    int mRepeatRate = 25;
    int mRepeatDelay = 400;

    uint32_t mKeymapFormat = WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1;

    BobUI::KeyboardModifiers modifiers() const;

    struct ::wl_keyboard *wl_keyboard() { return BobUIWayland::wl_keyboard::object(); }

#if BOBUI_CONFIG(xkbcommon)
    virtual int keysymToBobUIKey(xkb_keysym_t keysym, BobUI::KeyboardModifiers modifiers, xkb_state *state, xkb_keycode_t code) {
        return QXkbCommon::keysymToBobUIKey(keysym, modifiers, state, code);
    }
#endif

private Q_SLOTS:
    void handleFocusDestroyed();
    void handleFocusLost();

private:
#if BOBUI_CONFIG(xkbcommon)
    bool createDefaultKeymap();
#endif
    void handleKey(ulong timestamp, QEvent::Type type, int key, BobUI::KeyboardModifiers modifiers,
                   quint32 nativeScanCode, quint32 nativeVirtualKey, quint32 nativeModifiers,
                   const QString &text, bool autorepeat = false, ushort count = 1);

#if BOBUI_CONFIG(xkbcommon)
    QXkbCommon::ScopedXKBKeymap mXkbKeymap;
    QXkbCommon::ScopedXKBState mXkbState;
#endif
    friend class QWaylandInputDevice;
};

class Q_WAYLANDCLIENT_EXPORT QWaylandInputDevice::Pointer : public QObject, public BobUIWayland::wl_pointer
{
    Q_OBJECT
public:
    explicit Pointer(QWaylandInputDevice *seat);
    ~Pointer() override;
    QWaylandWindow *focusWindow() const;
#if BOBUI_CONFIG(cursor)
    int idealCursorScale() const;
    void updateCursorTheme();
    void updateCursor();
    void cursorTimerCallback();
    void cursorFrameCallback();
    CursorSurface<QWaylandInputDevice::Pointer> *getOrCreateCursorSurface();
#endif
    QWaylandInputDevice *seat() const { return mParent; }

    struct ::wl_pointer *wl_pointer() { return BobUIWayland::wl_pointer::object(); }

protected:
    void pointer_enter(uint32_t serial, struct wl_surface *surface,
                       wl_fixed_t sx, wl_fixed_t sy) override;
    void pointer_leave(uint32_t time, struct wl_surface *surface) override;
    void pointer_motion(uint32_t time,
                        wl_fixed_t sx, wl_fixed_t sy) override;
    void pointer_button(uint32_t serial, uint32_t time,
                        uint32_t button, uint32_t state) override;
    void pointer_axis(uint32_t time,
                      uint32_t axis,
                      wl_fixed_t value) override;
    void pointer_axis_source(uint32_t source) override;
    void pointer_axis_stop(uint32_t time, uint32_t axis) override;
    void pointer_axis_discrete(uint32_t axis, int32_t value) override;
    void pointer_frame() override;
    void pointer_axis_value120(uint32_t axis, int32_t value120) override;
    void pointer_axis_relative_direction(uint32_t axis, uint32_t direction) override;

private Q_SLOTS:
    void handleFocusDestroyed() { invalidateFocus(); }

private:
    void invalidateFocus();

public:
    void releaseButtons();
    void leavePointers();

    QWaylandInputDevice *mParent = nullptr;
    QPointer<QWaylandSurface> mFocus;
    uint32_t mEnterSerial = 0;
#if BOBUI_CONFIG(cursor)
    struct {
        QScopedPointer<QWaylandCursorShape> shape;
        QWaylandCursorTheme *theme = nullptr;
        int themeBufferScale = 0;
        QScopedPointer<CursorSurface<QWaylandInputDevice::Pointer>> surface;
        BOBUIimer frameTimer;
        bool gotFrameCallback = false;
        bool gotTimerCallback = false;
    } mCursor;
#endif
    QPointF mSurfacePos;
    QPointF mGlobalPos;
    BobUI::MouseButtons mButtons = BobUI::NoButton;
    BobUI::MouseButton mLastButton = BobUI::NoButton;

    struct FrameData {
        QScopedPointer<QWaylandPointerEvent> event;

        QPointF delta;
        QPoint delta120;
        axis_source axisSource = axis_source_wheel;
        bool verticalAxisInverted = false;
        bool horizontalAxisInverted = false;

        void resetScrollData();
        bool hasPixelDelta() const;
        QPoint pixelDeltaAndError(QPointF *accumulatedError) const;
        QPoint pixelDelta() const { return hasPixelDelta() ? delta.toPoint() : QPoint(); }
        QPoint angleDelta() const;
        BobUI::MouseEventSource wheelEventSource() const;
    } mFrameData;

    bool mScrollBeginSent = false;
    bool mScrollEnd = false;
    QPointF mScrollDeltaRemainder;
    QPointer<QWaylandWindow> mScrollTarget;

    QWaylandEventCompressionPrivate mEventCompression;

    void maybePointerFrame();

    void setFrameEvent(QWaylandPointerEvent *event);
    void flushScrollEvent();
    void flushFrameEvent();
private: //TODO: should other methods be private as well?
    bool isDefinitelyTerminated(axis_source source) const;
};

class Q_WAYLANDCLIENT_EXPORT QWaylandInputDevice::Touch : public BobUIWayland::wl_touch
{
public:
    Touch(QWaylandInputDevice *p);
    ~Touch() override;

    void touch_down(uint32_t serial,
                    uint32_t time,
                    struct wl_surface *surface,
                    int32_t id,
                    wl_fixed_t x,
                    wl_fixed_t y) override;
    void touch_up(uint32_t serial,
                  uint32_t time,
                  int32_t id) override;
    void touch_motion(uint32_t time,
                      int32_t id,
                      wl_fixed_t x,
                      wl_fixed_t y) override;
    void touch_frame() override;
    void touch_cancel() override;

    bool allTouchPointsReleased();
    void releasePoints();

    struct ::wl_touch *wl_touch() { return BobUIWayland::wl_touch::object(); }

    QWaylandInputDevice *mParent = nullptr;
    QPointer<QWaylandWindow> mFocus;
    QList<QWindowSystemInterface::TouchPoint> mPendingTouchPoints;
};

class QWaylandPointerEvent
{
    Q_GADGET
public:
    inline QWaylandPointerEvent(QEvent::Type type, BobUI::ScrollPhase phase, QWaylandWindow *surface,
                                ulong timestamp, const QPointF &localPos, const QPointF &globalPos,
                                BobUI::MouseButtons buttons, BobUI::MouseButton button,
                                BobUI::KeyboardModifiers modifiers)
        : type(type)
        , phase(phase)
        , timestamp(timestamp)
        , local(localPos)
        , global(globalPos)
        , buttons(buttons)
        , button(button)
        , modifiers(modifiers)
        , surface(surface)
    {}
    inline QWaylandPointerEvent(QEvent::Type type, BobUI::ScrollPhase phase, QWaylandWindow *surface,
                                ulong timestamp, const QPointF &local, const QPointF &global,
                                const QPoint &pixelDelta, const QPoint &angleDelta,
                                BobUI::MouseEventSource source,
                                BobUI::KeyboardModifiers modifiers, bool inverted)
        : type(type)
        , phase(phase)
        , timestamp(timestamp)
        , local(local)
        , global(global)
        , modifiers(modifiers)
        , pixelDelta(pixelDelta)
        , angleDelta(angleDelta)
        , source(source)
        , surface(surface)
        , inverted(inverted)
    {}

    QEvent::Type type = QEvent::None;
    BobUI::ScrollPhase phase = BobUI::NoScrollPhase;
    ulong timestamp = 0;
    QPointF local;
    QPointF global;
    BobUI::MouseButtons buttons;
    BobUI::MouseButton button = BobUI::NoButton; // Button that caused the event (QMouseEvent::button)
    BobUI::KeyboardModifiers modifiers;
    QPoint pixelDelta;
    QPoint angleDelta;
    BobUI::MouseEventSource source = BobUI::MouseEventNotSynthesized;
    QPointer<QWaylandWindow> surface;
    bool inverted = false;
};

#ifndef BOBUI_NO_GESTURES
class QWaylandPointerGestureSwipeEvent
{
    Q_GADGET
public:
    inline QWaylandPointerGestureSwipeEvent(QWaylandWindow *surface, BobUI::GestureState state,
                                            ulong timestamp, const QPointF &local,
                                            const QPointF &global, uint fingers, const QPointF& delta)
        : surface(surface)
        , state(state)
        , timestamp(timestamp)
        , local(local)
        , global(global)
        , fingers(fingers)
        , delta(delta)
    {}

    QPointer<QWaylandWindow> surface;
    BobUI::GestureState state = BobUI::GestureState::NoGesture;
    ulong timestamp = 0;
    QPointF local;
    QPointF global;
    uint fingers = 0;
    QPointF delta;
};

class QWaylandPointerGesturePinchEvent
{
    Q_GADGET
public:
    inline QWaylandPointerGesturePinchEvent(QWaylandWindow *surface, BobUI::GestureState state,
                                            ulong timestamp, const QPointF &local,
                                            const QPointF &global, uint fingers, const QPointF& delta,
                                            qreal scale_delta, qreal rotation_delta)
        : surface(surface)
        , state(state)
        , timestamp(timestamp)
        , local(local)
        , global(global)
        , fingers(fingers)
        , delta(delta)
        , scale_delta(scale_delta)
        , rotation_delta(rotation_delta)
    {}

    QPointer<QWaylandWindow> surface;
    BobUI::GestureState state = BobUI::GestureState::NoGesture;
    ulong timestamp = 0;
    QPointF local;
    QPointF global;
    uint fingers = 0;
    QPointF delta;
    qreal scale_delta = 0;
    qreal rotation_delta = 0;
};
#endif // #ifndef BOBUI_NO_GESTURES

}

BOBUI_END_NAMESPACE

#endif
