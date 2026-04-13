// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEVENT_H
#define QEVENT_H

#if 0
#pragma bobui_class(BobUIEvents)
#endif

#include <BobUIGui/bobuiguiglobal.h>

#include <BobUICore/qcoreevent.h>
#include <BobUICore/qiodevice.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qurl.h>
#include <BobUICore/qvariant.h>
#include <BobUIGui/qeventpoint.h>
#include <BobUIGui/qpointingdevice.h>
#include <BobUIGui/qregion.h>
#include <BobUIGui/qwindowdefs.h>

#if BOBUI_CONFIG(shortcut)
#  include <BobUIGui/qkeysequence.h>
#endif

class tst_QEvent;

BOBUI_BEGIN_NAMESPACE

class QFile;
class QAction;
class QMouseEvent;
template <typename T> class QPointer;
class QPointerEvent;
class QScreen;
#if BOBUI_CONFIG(shortcut)
class QShortcut;
#endif
class BOBUIabletEvent;
class BOBUIouchEvent;
#if BOBUI_CONFIG(gestures)
class QGesture;
#endif

class Q_GUI_EXPORT QInputEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QInputEvent)
public:
    explicit QInputEvent(Type type, const QInputDevice *m_dev, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);

    const QInputDevice *device() const { return m_dev; }
    QInputDevice::DeviceType deviceType() const { return m_dev ? m_dev->type() : QInputDevice::DeviceType::Unknown; }
    inline BobUI::KeyboardModifiers modifiers() const { return m_modState; }
    inline void setModifiers(BobUI::KeyboardModifiers modifiers) { m_modState = modifiers; }
    inline quint64 timestamp() const { return m_timeStamp; }
    virtual void setTimestamp(quint64 timestamp) { m_timeStamp = timestamp; }

protected:
    QInputEvent(Type type, PointerEventTag, const QInputDevice *dev, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);
    QInputEvent(Type type, SinglePointEventTag, const QInputDevice *dev, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);

    const QInputDevice *m_dev = nullptr;
    quint64 m_timeStamp = 0;
    BobUI::KeyboardModifiers m_modState = BobUI::NoModifier;
    // fill up to the closest 8-byte aligned size: 48
    quint32 m_reserved = 0;
};

class Q_GUI_EXPORT QPointerEvent : public QInputEvent
{
    Q_GADGET
    Q_DECL_EVENT_COMMON(QPointerEvent)
public:
    explicit QPointerEvent(Type type, const QPointingDevice *dev,
                           BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, const QList<QEventPoint> &points = {});

    const QPointingDevice *pointingDevice() const;
    QPointingDevice::PointerType pointerType() const {
        return pointingDevice() ? pointingDevice()->pointerType() : QPointingDevice::PointerType::Unknown;
    }
    void setTimestamp(quint64 timestamp) override;
    qsizetype pointCount() const { return m_points.size(); }
    QEventPoint &point(qsizetype i);
    const QList<QEventPoint> &points() const { return m_points; }
    QEventPoint *pointById(int id);
    bool allPointsGrabbed() const;
    virtual bool isBeginEvent() const { return false; }
    virtual bool isUpdateEvent() const { return false; }
    virtual bool isEndEvent() const { return false; }
    bool allPointsAccepted() const;
    virtual void setAccepted(bool accepted) override;
    QObject *exclusiveGrabber(const QEventPoint &point) const;
    void setExclusiveGrabber(const QEventPoint &point, QObject *exclusiveGrabber);
    QList<QPointer <QObject>> passiveGrabbers(const QEventPoint &point) const;
    void clearPassiveGrabbers(const QEventPoint &point);
    bool addPassiveGrabber(const QEventPoint &point, QObject *grabber);
    bool removePassiveGrabber(const QEventPoint &point, QObject *grabber);

protected:
    QPointerEvent(Type type, SinglePointEventTag, const QInputDevice *dev, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);

    QList<QEventPoint> m_points;
};

class Q_GUI_EXPORT QSinglePointEvent : public QPointerEvent
{
    Q_GADGET
    Q_PROPERTY(QObject *exclusivePointGrabber READ exclusivePointGrabber
               WRITE setExclusivePointGrabber)

    Q_DECL_EVENT_COMMON(QSinglePointEvent)
public:
    inline BobUI::MouseButton button() const { return m_button; }
    inline BobUI::MouseButtons buttons() const { return m_mouseState; }

    inline QPointF position() const
    { Q_ASSERT(!m_points.isEmpty()); return m_points.first().position(); }
    inline QPointF scenePosition() const
    { Q_ASSERT(!m_points.isEmpty()); return m_points.first().scenePosition(); }
    inline QPointF globalPosition() const
    { Q_ASSERT(!m_points.isEmpty()); return m_points.first().globalPosition(); }

    bool isBeginEvent() const override;
    bool isUpdateEvent() const override;
    bool isEndEvent() const override;

    QObject *exclusivePointGrabber() const
    { return QPointerEvent::exclusiveGrabber(points().first()); }
    void setExclusivePointGrabber(QObject *exclusiveGrabber)
    { QPointerEvent::setExclusiveGrabber(points().first(), exclusiveGrabber); }

protected:
    friend class ::tst_QEvent;
    friend class QMutableSinglePointEvent;
    QSinglePointEvent(Type type, const QPointingDevice *dev, const QEventPoint &point,
                      BobUI::MouseButton button, BobUI::MouseButtons buttons,
                      BobUI::KeyboardModifiers modifiers, BobUI::MouseEventSource source);
    QSinglePointEvent(Type type, const QPointingDevice *dev, const QPointF &localPos,
                      const QPointF &scenePos, const QPointF &globalPos,
                      BobUI::MouseButton button, BobUI::MouseButtons buttons,
                      BobUI::KeyboardModifiers modifiers,
                      BobUI::MouseEventSource source = BobUI::MouseEventNotSynthesized);

    BobUI::MouseButton m_button = BobUI::NoButton;
    BobUI::MouseButtons m_mouseState = BobUI::NoButton;
    BobUI::MouseEventSource m_source;
    /*
        Fill up to the next 8-byte aligned size: 88
        We have 32bits left, use some for QSinglePointEvent subclasses so that
        we don't end up with gaps.
    */
    // split this in two quint16; with a quint32, MSVC would 32-bit align it
    quint16 m_reserved;
    quint16 m_reserved2  : 11;
    // for QMouseEvent
    quint16 m_doubleClick : 1;
    // for QWheelEvent
    quint16 m_phase : 3;
    quint16 m_invertedScrolling : 1;
};

class Q_GUI_EXPORT QEnterEvent : public QSinglePointEvent
{
    Q_DECL_EVENT_COMMON(QEnterEvent)
public:
    QEnterEvent(const QPointF &localPos, const QPointF &scenePos, const QPointF &globalPos,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());

#if BOBUI_DEPRECATED_SINCE(6, 0)
#ifndef BOBUI_NO_INTEGER_EVENT_COORDINATES
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPoint pos() const { return position().toPoint(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline QPoint globalPos() const { return globalPosition().toPoint(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline int x() const { return qRound(position().x()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline int y() const { return qRound(position().y()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline int globalX() const { return qRound(globalPosition().x()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline int globalY() const { return qRound(globalPosition().y()); }
#endif
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    QPointF localPos() const { return position(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use scenePosition()")
    QPointF windowPos() const { return scenePosition(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    QPointF screenPos() const { return globalPosition(); }
#endif // BOBUI_DEPRECATED_SINCE(6, 0)
};

class Q_GUI_EXPORT QMouseEvent : public QSinglePointEvent
{
    Q_DECL_EVENT_COMMON(QMouseEvent)
public:
#if BOBUI_DEPRECATED_SINCE(6, 4)
    BOBUI_DEPRECATED_VERSION_X_6_4("Use another constructor")
    QMouseEvent(Type type, const QPointF &localPos, BobUI::MouseButton button,
                BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
#endif
    QMouseEvent(Type type, const QPointF &localPos, const QPointF &globalPos,
                BobUI::MouseButton button, BobUI::MouseButtons buttons,
                BobUI::KeyboardModifiers modifiers,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
    QMouseEvent(Type type, const QPointF &localPos, const QPointF &scenePos, const QPointF &globalPos,
                BobUI::MouseButton button, BobUI::MouseButtons buttons,
                BobUI::KeyboardModifiers modifiers,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
    QMouseEvent(Type type, const QPointF &localPos, const QPointF &scenePos, const QPointF &globalPos,
                BobUI::MouseButton button, BobUI::MouseButtons buttons,
                BobUI::KeyboardModifiers modifiers, BobUI::MouseEventSource source,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());

#ifndef BOBUI_NO_INTEGER_EVENT_COORDINATES
    inline QPoint pos() const { return position().toPoint(); }
#endif
#if BOBUI_DEPRECATED_SINCE(6, 0)
#ifndef BOBUI_NO_INTEGER_EVENT_COORDINATES
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline QPoint globalPos() const { return globalPosition().toPoint(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline int x() const { return qRound(position().x()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline int y() const { return qRound(position().y()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline int globalX() const { return qRound(globalPosition().x()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline int globalY() const { return qRound(globalPosition().y()); }
#endif // BOBUI_NO_INTEGER_EVENT_COORDINATES
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    QPointF localPos() const { return position(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use scenePosition()")
    QPointF windowPos() const { return scenePosition(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    QPointF screenPos() const { return globalPosition(); }
#endif // BOBUI_DEPRECATED_SINCE(6, 0)
    BobUI::MouseEventSource source() const;
    BobUI::MouseEventFlags flags() const;
};

class Q_GUI_EXPORT QHoverEvent : public QSinglePointEvent
{
    Q_DECL_EVENT_COMMON(QHoverEvent)
public:
    QHoverEvent(Type type, const QPointF &scenePos, const QPointF &globalPos, const QPointF &oldPos,
                BobUI::KeyboardModifiers modifiers = BobUI::NoModifier,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
#if BOBUI_DEPRECATED_SINCE(6, 3)
    BOBUI_DEPRECATED_VERSION_X_6_3("Use the other constructor")
    QHoverEvent(Type type, const QPointF &pos, const QPointF &oldPos,
                BobUI::KeyboardModifiers modifiers = BobUI::NoModifier,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());
#endif

#if BOBUI_DEPRECATED_SINCE(6, 0)
#ifndef BOBUI_NO_INTEGER_EVENT_COORDINATES
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPoint pos() const { return position().toPoint(); }
#endif

    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPointF posF() const { return position(); }
#endif // BOBUI_DEPRECATED_SINCE(6, 0)

    bool isUpdateEvent() const override  { return true; }

    // TODO deprecate when we figure out an actual replacement (point history?)
    inline QPoint oldPos() const { return m_oldPos.toPoint(); }
    inline QPointF oldPosF() const { return m_oldPos; }

protected:
    QPointF m_oldPos; // TODO remove?
};

#if BOBUI_CONFIG(wheelevent)
class Q_GUI_EXPORT QWheelEvent : public QSinglePointEvent
{
    Q_GADGET
    Q_PROPERTY(const QPointingDevice *device READ pointingDevice)
    Q_PROPERTY(QPoint pixelDelta READ pixelDelta)
    Q_PROPERTY(QPoint angleDelta READ angleDelta)
    Q_PROPERTY(BobUI::ScrollPhase phase READ phase)
    Q_PROPERTY(bool inverted READ inverted)

    Q_DECL_EVENT_COMMON(QWheelEvent)
public:
    enum { DefaultDeltasPerStep = 120 };

    QWheelEvent(const QPointF &pos, const QPointF &globalPos, QPoint pixelDelta, QPoint angleDelta,
                BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers, BobUI::ScrollPhase phase,
                bool inverted, BobUI::MouseEventSource source = BobUI::MouseEventNotSynthesized,
                const QPointingDevice *device = QPointingDevice::primaryPointingDevice());

    inline QPoint pixelDelta() const { return m_pixelDelta; }
    inline QPoint angleDelta() const { return m_angleDelta; }

    inline BobUI::ScrollPhase phase() const { return BobUI::ScrollPhase(m_phase); }
    inline bool inverted() const { return m_invertedScrolling; }
    inline bool isInverted() const { return m_invertedScrolling; }
    inline bool hasPixelDelta() const { return !m_pixelDelta.isNull(); }

    bool isBeginEvent() const override;
    bool isUpdateEvent() const override;
    bool isEndEvent() const override;
    BobUI::MouseEventSource source() const { return BobUI::MouseEventSource(m_source); }

protected:
    QPoint m_pixelDelta;
    QPoint m_angleDelta;
};
#endif

#if BOBUI_CONFIG(tabletevent)
class Q_GUI_EXPORT BOBUIabletEvent : public QSinglePointEvent
{
    Q_DECL_EVENT_COMMON(BOBUIabletEvent)
public:
    BOBUIabletEvent(Type t, const QPointingDevice *device,
                 const QPointF &pos, const QPointF &globalPos,
                 qreal pressure, float xTilt, float yTilt,
                 float tangentialPressure, qreal rotation, float z,
                 BobUI::KeyboardModifiers keyState,
                 BobUI::MouseButton button, BobUI::MouseButtons buttons);

#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPoint pos() const { return position().toPoint(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline QPoint globalPos() const { return globalPosition().toPoint(); }

    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline const QPointF posF() const { return position(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    inline const QPointF globalPosF() const { return globalPosition(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position().x()")
    inline int x() const { return qRound(position().x()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position().y()")
    inline int y() const { return qRound(position().y()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition().x()")
    inline int globalX() const { return qRound(globalPosition().x()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition().y()")
    inline int globalY() const { return qRound(globalPosition().y()); }
    BOBUI_DEPRECATED_VERSION_X_6_0("use globalPosition().x()")
    inline qreal hiResGlobalX() const { return globalPosition().x(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("use globalPosition().y()")
    inline qreal hiResGlobalY() const { return globalPosition().y(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("use pointingDevice().uniqueId()")
    inline qint64 uniqueId() const { return pointingDevice() ? pointingDevice()->uniqueId().numericId() : -1; }
#endif
    inline qreal pressure() const { Q_ASSERT(!points().isEmpty()); return points().first().pressure(); }
    inline qreal rotation() const { Q_ASSERT(!points().isEmpty()); return points().first().rotation(); }
    inline qreal z() const { return m_z; }
    inline qreal tangentialPressure() const { return m_tangential; }
    inline qreal xTilt() const { return m_xTilt; }
    inline qreal yTilt() const { return m_yTilt; }

protected:
    float m_tangential;
    float m_xTilt;
    float m_yTilt;
    float m_z;
};
#endif // BOBUI_CONFIG(tabletevent)

#if BOBUI_CONFIG(gestures)
class Q_GUI_EXPORT QNativeGestureEvent : public QSinglePointEvent
{
    Q_DECL_EVENT_COMMON(QNativeGestureEvent)
public:
#if BOBUI_DEPRECATED_SINCE(6, 2)
    BOBUI_DEPRECATED_VERSION_X_6_2("Use the other constructor")
    QNativeGestureEvent(BobUI::NativeGestureType type, const QPointingDevice *dev, const QPointF &localPos, const QPointF &scenePos,
                        const QPointF &globalPos, qreal value, quint64 sequenceId, quint64 intArgument);
#endif
    QNativeGestureEvent(BobUI::NativeGestureType type, const QPointingDevice *dev, int fingerCount,
                        const QPointF &localPos, const QPointF &scenePos, const QPointF &globalPos,
                        qreal value, const QPointF &delta, quint64 sequenceId = UINT64_MAX);

    BobUI::NativeGestureType gestureType() const { return m_gestureType; }
    int fingerCount() const { return m_fingerCount; }
    qreal value() const { return m_realValue; }
    QPointF delta() const {
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
        return m_delta.toPointF();
#else
        return m_delta;
#endif
    }

#if BOBUI_DEPRECATED_SINCE(6, 0)
#ifndef BOBUI_NO_INTEGER_EVENT_COORDINATES
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position().toPoint()")
    inline const QPoint pos() const { return position().toPoint(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition().toPoint()")
    inline const QPoint globalPos() const { return globalPosition().toPoint(); }
#endif
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    QPointF localPos() const { return position(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use scenePosition()")
    QPointF windowPos() const { return scenePosition(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use globalPosition()")
    QPointF screenPos() const { return globalPosition(); }
#endif

protected:
    quint64 m_sequenceId;
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    QVector2D m_delta;
#else
    QPointF m_delta;
#endif
    qreal m_realValue;
    BobUI::NativeGestureType m_gestureType;
    quint32 m_fingerCount : 4;
    quint32 m_reserved : 28;
};
#endif // BOBUI_CONFIG(gestures)

class Q_GUI_EXPORT QKeyEvent : public QInputEvent
{
    Q_DECL_EVENT_COMMON(QKeyEvent)
public:
    QKeyEvent(Type type, int key, BobUI::KeyboardModifiers modifiers, const QString& text = QString(),
              bool autorep = false, quint16 count = 1);
    QKeyEvent(Type type, int key, BobUI::KeyboardModifiers modifiers,
              quint32 nativeScanCode, quint32 nativeVirtualKey, quint32 nativeModifiers,
              const QString &text = QString(), bool autorep = false, quint16 count = 1,
              const QInputDevice *device = QInputDevice::primaryKeyboard());

    int key() const { return m_key; }
#if BOBUI_CONFIG(shortcut)
    bool matches(QKeySequence::StandardKey key) const;
#endif
    BobUI::KeyboardModifiers modifiers() const;
    QKeyCombination keyCombination() const
    {
        return QKeyCombination(modifiers(), BobUI::Key(m_key));
    }
    inline QString text() const { return m_text; }
    inline bool isAutoRepeat() const { return m_autoRepeat; }
    inline int count() const { return int(m_count); }

    inline quint32 nativeScanCode() const { return m_scanCode; }
    inline quint32 nativeVirtualKey() const { return m_virtualKey; }
    inline quint32 nativeModifiers() const { return m_nativeModifiers; }

#if BOBUI_CONFIG(shortcut)
    friend inline bool operator==(QKeyEvent *e, QKeySequence::StandardKey key)
    { return (e ? e->matches(key) : false); }
    friend inline bool operator==(QKeySequence::StandardKey key, QKeyEvent *e)
    { return (e ? e->matches(key) : false); }
#endif // BOBUI_CONFIG(shortcut)

protected:
    QString m_text;
    int m_key;
    quint32 m_scanCode;
    quint32 m_virtualKey;
    quint32 m_nativeModifiers;
    quint16 m_count      : 15;
    quint16 m_autoRepeat : 1;
};


class Q_GUI_EXPORT QFocusEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QFocusEvent)
public:
    explicit QFocusEvent(Type type, BobUI::FocusReason reason=BobUI::OtherFocusReason);

    inline bool gotFocus() const { return type() == FocusIn; }
    inline bool lostFocus() const { return type() == FocusOut; }

    BobUI::FocusReason reason() const;

private:
    BobUI::FocusReason m_reason;
};


class Q_GUI_EXPORT QPaintEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QPaintEvent)
public:
    explicit QPaintEvent(const QRegion& paintRegion);
    explicit QPaintEvent(const QRect &paintRect);

    inline const QRect &rect() const { return m_rect; }
    inline const QRegion &region() const { return m_region; }

protected:
    QRect m_rect;
    QRegion m_region;
    bool m_erased;
};

class Q_GUI_EXPORT QMoveEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QMoveEvent)
public:
    QMoveEvent(const QPoint &pos, const QPoint &oldPos);

    inline const QPoint &pos() const { return m_pos; }
    inline const QPoint &oldPos() const { return m_oldPos;}
protected:
    QPoint m_pos, m_oldPos;
    friend class QApplication;
    friend class QApplicationPrivate;
};

class Q_GUI_EXPORT QExposeEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QExposeEvent)
public:
    explicit QExposeEvent(const QRegion &m_region);

#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_X_6_0("Handle QPaintEvent instead")
    inline const QRegion &region() const { return m_region; }
#endif

protected:
    QRegion m_region;
    friend class QWidgetWindow;
};

class Q_GUI_EXPORT QPlatformSurfaceEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QPlatformSurfaceEvent)
public:
    enum SurfaceEventType {
        SurfaceCreated,
        SurfaceAboutToBeDestroyed
    };

    explicit QPlatformSurfaceEvent(SurfaceEventType surfaceEventType);

    inline SurfaceEventType surfaceEventType() const { return m_surfaceEventType; }

protected:
    SurfaceEventType m_surfaceEventType;
};

class Q_GUI_EXPORT QResizeEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QResizeEvent)
public:
    QResizeEvent(const QSize &size, const QSize &oldSize);

    inline const QSize &size() const { return m_size; }
    inline const QSize &oldSize()const { return m_oldSize;}
protected:
    QSize m_size, m_oldSize;
    friend class QApplication;
    friend class QApplicationPrivate;
};


class Q_GUI_EXPORT QCloseEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QCloseEvent)
public:
    QCloseEvent();
};


class Q_GUI_EXPORT QIconDragEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QIconDragEvent)
public:
    QIconDragEvent();
};


class Q_GUI_EXPORT QShowEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QShowEvent)
public:
    QShowEvent();
};


class Q_GUI_EXPORT QHideEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QHideEvent)
public:
    QHideEvent();
};

#ifndef BOBUI_NO_CONTEXTMENU
class Q_GUI_EXPORT QContextMenuEvent : public QInputEvent
{
    Q_DECL_EVENT_COMMON(QContextMenuEvent)
public:
    enum Reason { Mouse, Keyboard, Other };

    QContextMenuEvent(Reason reason, const QPoint &pos, const QPoint &globalPos,
                      BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);
#if BOBUI_DEPRECATED_SINCE(6, 4)
    BOBUI_DEPRECATED_VERSION_X_6_4("Use the other constructor")
    QContextMenuEvent(Reason reason, const QPoint &pos);
#endif

    inline int x() const { return m_pos.x(); }
    inline int y() const { return m_pos.y(); }
    inline int globalX() const { return m_globalPos.x(); }
    inline int globalY() const { return m_globalPos.y(); }

    inline const QPoint& pos() const { return m_pos; }
    inline const QPoint& globalPos() const { return m_globalPos; }

    inline Reason reason() const { return Reason(m_reason); }

protected:
    QPoint m_pos;
    QPoint m_globalPos;
    uint m_reason : 8;
};
#endif // BOBUI_NO_CONTEXTMENU

#ifndef BOBUI_NO_INPUTMETHOD
class Q_GUI_EXPORT QInputMethodEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QInputMethodEvent)
public:
    enum AttributeType {
       TextFormat,
       Cursor,
       Language,
       Ruby,
       Selection,
       MimeData
    };
    class Attribute {
    public:
        Attribute(AttributeType typ, int s, int l, QVariant val) : type(typ), start(s), length(l), value(std::move(val)) {}
        Attribute(AttributeType typ, int s, int l) : type(typ), start(s), length(l), value() {}

        AttributeType type;
        int start;
        int length;
        QVariant value;
    };
    QInputMethodEvent();
    QInputMethodEvent(const QString &preeditText, const QList<Attribute> &attributes);

    void setCommitString(const QString &commitString, int replaceFrom = 0, int replaceLength = 0);
    inline const QList<Attribute> &attributes() const { return m_attributes; }
    inline const QString &preeditString() const { return m_preedit; }

    inline const QString &commitString() const { return m_commit; }
    inline int replacementStart() const { return m_replacementStart; }
    inline int replacementLength() const { return m_replacementLength; }

    inline friend bool operator==(const QInputMethodEvent::Attribute &lhs,
                                  const QInputMethodEvent::Attribute &rhs)
    {
        return lhs.type == rhs.type && lhs.start == rhs.start
                && lhs.length == rhs.length && lhs.value == rhs.value;
    }

    inline friend bool operator!=(const QInputMethodEvent::Attribute &lhs,
                                  const QInputMethodEvent::Attribute &rhs)
    {
        return !(lhs == rhs);
    }

private:
    QString m_preedit;
    QString m_commit;
    QList<Attribute> m_attributes;
    int m_replacementStart;
    int m_replacementLength;
};
Q_DECLARE_TYPEINFO(QInputMethodEvent::Attribute, Q_RELOCATABLE_TYPE);

class Q_GUI_EXPORT QInputMethodQueryEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QInputMethodQueryEvent)
public:
    explicit QInputMethodQueryEvent(BobUI::InputMethodQueries queries);

    BobUI::InputMethodQueries queries() const { return m_queries; }

    void setValue(BobUI::InputMethodQuery query, const QVariant &value);
    QVariant value(BobUI::InputMethodQuery query) const;
private:
    BobUI::InputMethodQueries m_queries;
    struct QueryPair {
        BobUI::InputMethodQuery query;
        QVariant value;
    };
    friend BOBUIypeInfo<QueryPair>;
    QList<QueryPair> m_values;
};
Q_DECLARE_TYPEINFO(QInputMethodQueryEvent::QueryPair, Q_RELOCATABLE_TYPE);

#endif // BOBUI_NO_INPUTMETHOD

#if BOBUI_CONFIG(draganddrop)

class QMimeData;

class Q_GUI_EXPORT QDropEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QDropEvent)
public:
    QDropEvent(const QPointF& pos, BobUI::DropActions actions, const QMimeData *data,
               BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers, Type type = Drop);

#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position().toPoint()")
    inline QPoint pos() const { return position().toPoint(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use position()")
    inline QPointF posF() const { return position(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use buttons()")
    inline BobUI::MouseButtons mouseButtons() const { return buttons(); }
    BOBUI_DEPRECATED_VERSION_X_6_0("Use modifiers()")
    inline BobUI::KeyboardModifiers keyboardModifiers() const { return modifiers(); }
#endif // BOBUI_DEPRECATED_SINCE(6, 0)

    QPointF position() const { return m_pos; }
    inline BobUI::MouseButtons buttons() const { return m_mouseState; }
    inline BobUI::KeyboardModifiers modifiers() const { return m_modState; }

    inline BobUI::DropActions possibleActions() const { return m_actions; }
    inline BobUI::DropAction proposedAction() const { return m_defaultAction; }
    inline void acceptProposedAction() { m_dropAction = m_defaultAction; accept(); }

    inline BobUI::DropAction dropAction() const { return m_dropAction; }
    void setDropAction(BobUI::DropAction action);

    QObject* source() const;
    inline const QMimeData *mimeData() const { return m_data; }

protected:
    friend class QApplication;
    QPointF m_pos;
    BobUI::MouseButtons m_mouseState;
    BobUI::KeyboardModifiers m_modState;
    BobUI::DropActions m_actions;
    BobUI::DropAction m_dropAction;
    BobUI::DropAction m_defaultAction;
    const QMimeData *m_data;
};


class Q_GUI_EXPORT QDragMoveEvent : public QDropEvent
{
    Q_DECL_EVENT_COMMON(QDragMoveEvent)
public:
    QDragMoveEvent(const QPoint &pos, BobUI::DropActions actions, const QMimeData *data,
                   BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers, Type type = DragMove);

    inline QRect answerRect() const { return m_rect; }

    inline void accept() { QDropEvent::accept(); }
    inline void ignore() { QDropEvent::ignore(); }

    inline void accept(const QRect & r) { accept(); m_rect = r; }
    inline void ignore(const QRect & r) { ignore(); m_rect = r; }

protected:
    QRect m_rect;
};


class Q_GUI_EXPORT QDragEnterEvent : public QDragMoveEvent
{
    Q_DECL_EVENT_COMMON(QDragEnterEvent)
public:
    QDragEnterEvent(const QPoint &pos, BobUI::DropActions actions, const QMimeData *data,
                    BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers);
};


class Q_GUI_EXPORT QDragLeaveEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QDragLeaveEvent)
public:
    QDragLeaveEvent();
};
#endif // BOBUI_CONFIG(draganddrop)


class Q_GUI_EXPORT QHelpEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QHelpEvent)
public:
    QHelpEvent(Type type, const QPoint &pos, const QPoint &globalPos);

    inline int x() const { return m_pos.x(); }
    inline int y() const { return m_pos.y(); }
    inline int globalX() const { return m_globalPos.x(); }
    inline int globalY() const { return m_globalPos.y(); }

    inline const QPoint& pos()  const { return m_pos; }
    inline const QPoint& globalPos() const { return m_globalPos; }

private:
    QPoint m_pos;
    QPoint m_globalPos;
};

#ifndef BOBUI_NO_STATUSTIP
class Q_GUI_EXPORT QStatusTipEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QStatusTipEvent)
public:
    explicit QStatusTipEvent(const QString &tip);

    inline QString tip() const { return m_tip; }
private:
    QString m_tip;
};
#endif

#if BOBUI_CONFIG(whatsthis)
class Q_GUI_EXPORT QWhatsThisClickedEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QWhatsThisClickedEvent)
public:
    explicit QWhatsThisClickedEvent(const QString &href);

    inline QString href() const { return m_href; }
private:
    QString m_href;
};
#endif

#if BOBUI_CONFIG(action)
class Q_GUI_EXPORT QActionEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QActionEvent)
public:
    QActionEvent(int type, QAction *action, QAction *before = nullptr);

    inline QAction *action() const { return m_action; }
    inline QAction *before() const { return m_before; }
private:
    QAction *m_action;
    QAction *m_before;
};
#endif // BOBUI_CONFIG(action)

class Q_GUI_EXPORT QFileOpenEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QFileOpenEvent)
public:
    explicit QFileOpenEvent(const QString &file);
    explicit QFileOpenEvent(const QUrl &url);

    inline QString file() const { return m_file; }
    QUrl url() const { return m_url; }
#if BOBUI_DEPRECATED_SINCE(6, 6)
    BOBUI_DEPRECATED_VERSION_X_6_6("Interpret the string returned by file()")
    bool openFile(QFile &file, QIODevice::OpenMode flags) const;
#endif
private:
    QString m_file;
    QUrl m_url;
};

#ifndef BOBUI_NO_TOOLBAR
class Q_GUI_EXPORT BOBUIoolBarChangeEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(BOBUIoolBarChangeEvent)
public:
    explicit BOBUIoolBarChangeEvent(bool t);

    inline bool toggle() const { return m_toggle; }
private:
    bool m_toggle;
};
#endif

#if BOBUI_CONFIG(shortcut)
class Q_GUI_EXPORT QShortcutEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QShortcutEvent)
public:
    // Note this is publicly deprecated, but should remain as internal constructor:
    QShortcutEvent(const QKeySequence &key, int id, bool ambiguous = false);
    QShortcutEvent(const QKeySequence &key, const QShortcut *shortcut = nullptr, bool ambiguous = false);

    inline const QKeySequence &key() const { return m_sequence; }
    // Note this is publicly deprecated, but should remain as internal getter:
    inline int shortcutId() const { return m_shortcutId; }
    inline bool isAmbiguous() const { return m_ambiguous; }
protected:
    QKeySequence m_sequence;
    int  m_shortcutId;
    bool m_ambiguous;
};
#endif

class Q_GUI_EXPORT QWindowStateChangeEvent: public QEvent
{
    Q_DECL_EVENT_COMMON(QWindowStateChangeEvent)
public:
    explicit QWindowStateChangeEvent(BobUI::WindowStates oldState, bool isOverride = false);

    inline BobUI::WindowStates oldState() const { return m_oldStates; }
    bool isOverride() const;

private:
    BobUI::WindowStates m_oldStates;
    bool m_override;
};

#ifndef BOBUI_NO_DEBUG_STREAM
// ### BobUI 7: move this to BobUI Core and add a hook to allow GUI events to be printed: BOBUIBUG-127680
Q_GUI_EXPORT QDebug operator<<(QDebug, const QEvent *);
#endif

class Q_GUI_EXPORT BOBUIouchEvent : public QPointerEvent
{
    Q_DECL_EVENT_COMMON(BOBUIouchEvent)
public:
    using TouchPoint = QEventPoint; // source compat

    explicit BOBUIouchEvent(QEvent::Type eventType,
                         const QPointingDevice *device = nullptr,
                         BobUI::KeyboardModifiers modifiers = BobUI::NoModifier,
                         const QList<QEventPoint> &touchPoints = {});
#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_X_6_0("Use another constructor")
    explicit BOBUIouchEvent(QEvent::Type eventType,
                         const QPointingDevice *device,
                         BobUI::KeyboardModifiers modifiers,
                         QEventPoint::States touchPointStates,
                         const QList<QEventPoint> &touchPoints = {});
#endif

    inline QObject *target() const { return m_target; }
    inline QEventPoint::States touchPointStates() const { return m_touchPointStates; }
#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_X_6_0("Use points()")
    const QList<QEventPoint> &touchPoints() const { return points(); }
#endif
    bool isBeginEvent() const override;
    bool isUpdateEvent() const override;
    bool isEndEvent() const override;

protected:
    friend class QMutableTouchEvent;
    QObject *m_target = nullptr;
    QEventPoint::States m_touchPointStates = QEventPoint::State::Unknown;
    quint32 m_reserved : 24;
};

class Q_GUI_EXPORT QScrollPrepareEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QScrollPrepareEvent)
public:
    explicit QScrollPrepareEvent(const QPointF &startPos);

    QPointF startPos() const { return m_startPos; }

    QSizeF viewportSize() const { return m_viewportSize; }
    QRectF contentPosRange() const { return m_contentPosRange; }
    QPointF contentPos() const { return m_contentPos; }

    void setViewportSize(const QSizeF &size);
    void setContentPosRange(const QRectF &rect);
    void setContentPos(const QPointF &pos);

private:
    QRectF m_contentPosRange;
    QSizeF m_viewportSize;
    QPointF m_startPos;
    QPointF m_contentPos;
};


class Q_GUI_EXPORT QScrollEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QScrollEvent)
public:
    enum ScrollState
    {
        ScrollStarted,
        ScrollUpdated,
        ScrollFinished
    };

    QScrollEvent(const QPointF &contentPos, const QPointF &overshoot, ScrollState scrollState);

    QPointF contentPos() const { return m_contentPos; }
    QPointF overshootDistance() const { return m_overshoot; }
    ScrollState scrollState() const { return m_state; }

private:
    QPointF m_contentPos;
    QPointF m_overshoot;
    QScrollEvent::ScrollState m_state;
};

class Q_GUI_EXPORT QScreenOrientationChangeEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QScreenOrientationChangeEvent)
public:
    QScreenOrientationChangeEvent(QScreen *screen, BobUI::ScreenOrientation orientation);

    QScreen *screen() const { return m_screen; }
    BobUI::ScreenOrientation orientation() const { return m_orientation; }

private:
    QScreen *m_screen;
    BobUI::ScreenOrientation m_orientation;
};

class Q_GUI_EXPORT QApplicationStateChangeEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QApplicationStateChangeEvent)
public:
    explicit QApplicationStateChangeEvent(BobUI::ApplicationState state);

    BobUI::ApplicationState applicationState() const { return m_applicationState; }

private:
    BobUI::ApplicationState m_applicationState;
};

class Q_GUI_EXPORT QChildWindowEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QChildWindowEvent)
public:
    explicit QChildWindowEvent(Type type, QWindow *childWindow);
    QWindow *child() const { return c; }

private:
    QWindow *c;
};

BOBUI_END_NAMESPACE

#endif // QEVENT_H
