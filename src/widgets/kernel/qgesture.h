// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGESTURE_H
#define QGESTURE_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qmap.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qdatetime.h>
#include <BobUICore/qpoint.h>
#include <BobUICore/qrect.h>
#include <BobUICore/qmetatype.h>
#include <BobUIGui/qevent.h>

#ifndef BOBUI_NO_GESTURES

// ### move to qnamespace.h
BOBUI_DECL_METATYPE_EXTERN_TAGGED(BobUI::GestureState, BobUI__GestureState, Q_WIDGETS_EXPORT)
// ### move to qnamespace.h
BOBUI_DECL_METATYPE_EXTERN_TAGGED(BobUI::GestureType, BobUI__GestureType, Q_WIDGETS_EXPORT)

BOBUI_BEGIN_NAMESPACE


class QGesturePrivate;
class Q_WIDGETS_EXPORT QGesture : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QGesture)

    Q_PROPERTY(BobUI::GestureState state READ state)
    Q_PROPERTY(BobUI::GestureType gestureType READ gestureType)
    Q_PROPERTY(QGesture::GestureCancelPolicy gestureCancelPolicy READ gestureCancelPolicy
               WRITE setGestureCancelPolicy)
    Q_PROPERTY(QPointF hotSpot READ hotSpot WRITE setHotSpot RESET unsetHotSpot)
    Q_PROPERTY(bool hasHotSpot READ hasHotSpot)

public:
    explicit QGesture(QObject *parent = nullptr);
    ~QGesture();

    BobUI::GestureType gestureType() const;

    BobUI::GestureState state() const;

    QPointF hotSpot() const;
    void setHotSpot(const QPointF &value);
    bool hasHotSpot() const;
    void unsetHotSpot();

    enum GestureCancelPolicy {
        CancelNone = 0,
        CancelAllInContext
    };

    void setGestureCancelPolicy(GestureCancelPolicy policy);
    GestureCancelPolicy gestureCancelPolicy() const;

protected:
    QGesture(QGesturePrivate &dd, QObject *parent);

private:
    friend class QGestureEvent;
    friend class QGestureRecognizer;
    friend class QGestureManager;
    friend class QGraphicsScenePrivate;
};

class QPanGesturePrivate;
class Q_WIDGETS_EXPORT QPanGesture : public QGesture
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPanGesture)

    Q_PROPERTY(QPointF lastOffset READ lastOffset WRITE setLastOffset)
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset)
    Q_PROPERTY(QPointF delta READ delta STORED false)
    Q_PROPERTY(qreal acceleration READ acceleration WRITE setAcceleration)
    Q_PRIVATE_PROPERTY(QPanGesture::d_func(), qreal horizontalVelocity READ horizontalVelocity WRITE setHorizontalVelocity)
    Q_PRIVATE_PROPERTY(QPanGesture::d_func(), qreal verticalVelocity READ verticalVelocity WRITE setVerticalVelocity)

public:
    explicit QPanGesture(QObject *parent = nullptr);
    ~QPanGesture();

    QPointF lastOffset() const;
    QPointF offset() const;
    QPointF delta() const;
    qreal acceleration() const;

    void setLastOffset(const QPointF &value);
    void setOffset(const QPointF &value);
    void setAcceleration(qreal value);

    friend class QPanGestureRecognizer;
    friend class QWinNativePanGestureRecognizer;
};

class QPinchGesturePrivate;
class Q_WIDGETS_EXPORT QPinchGesture : public QGesture
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPinchGesture)

public:
    enum ChangeFlag {
        ScaleFactorChanged = 0x1,
        RotationAngleChanged = 0x2,
        CenterPointChanged = 0x4
    };
    Q_ENUM(ChangeFlag)
    Q_DECLARE_FLAGS(ChangeFlags, ChangeFlag)
    Q_FLAG(ChangeFlags)

    Q_PROPERTY(ChangeFlags totalChangeFlags READ totalChangeFlags WRITE setTotalChangeFlags)
    Q_PROPERTY(ChangeFlags changeFlags READ changeFlags WRITE setChangeFlags)

    Q_PROPERTY(qreal totalScaleFactor READ totalScaleFactor WRITE setTotalScaleFactor)
    Q_PROPERTY(qreal lastScaleFactor READ lastScaleFactor WRITE setLastScaleFactor)
    Q_PROPERTY(qreal scaleFactor READ scaleFactor WRITE setScaleFactor)

    Q_PROPERTY(qreal totalRotationAngle READ totalRotationAngle WRITE setTotalRotationAngle)
    Q_PROPERTY(qreal lastRotationAngle READ lastRotationAngle WRITE setLastRotationAngle)
    Q_PROPERTY(qreal rotationAngle READ rotationAngle WRITE setRotationAngle)

    Q_PROPERTY(QPointF startCenterPoint READ startCenterPoint WRITE setStartCenterPoint)
    Q_PROPERTY(QPointF lastCenterPoint READ lastCenterPoint WRITE setLastCenterPoint)
    Q_PROPERTY(QPointF centerPoint READ centerPoint WRITE setCenterPoint)

public:
    explicit QPinchGesture(QObject *parent = nullptr);
    ~QPinchGesture();

    ChangeFlags totalChangeFlags() const;
    void setTotalChangeFlags(ChangeFlags value);

    ChangeFlags changeFlags() const;
    void setChangeFlags(ChangeFlags value);

    QPointF startCenterPoint() const;
    QPointF lastCenterPoint() const;
    QPointF centerPoint() const;
    void setStartCenterPoint(const QPointF &value);
    void setLastCenterPoint(const QPointF &value);
    void setCenterPoint(const QPointF &value);

    qreal totalScaleFactor() const;
    qreal lastScaleFactor() const;
    qreal scaleFactor() const;
    void setTotalScaleFactor(qreal value);
    void setLastScaleFactor(qreal value);
    void setScaleFactor(qreal value);

    qreal totalRotationAngle() const;
    qreal lastRotationAngle() const;
    qreal rotationAngle() const;
    void setTotalRotationAngle(qreal value);
    void setLastRotationAngle(qreal value);
    void setRotationAngle(qreal value);

    friend class QPinchGestureRecognizer;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QPinchGesture::ChangeFlags)

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN_TAGGED(QPinchGesture::ChangeFlags,
                               QPinchGesture__ChangeFlags, Q_WIDGETS_EXPORT)

BOBUI_BEGIN_NAMESPACE

class QSwipeGesturePrivate;
class Q_WIDGETS_EXPORT QSwipeGesture : public QGesture
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSwipeGesture)

    Q_PROPERTY(SwipeDirection horizontalDirection READ horizontalDirection STORED false)
    Q_PROPERTY(SwipeDirection verticalDirection READ verticalDirection STORED false)
    Q_PROPERTY(qreal swipeAngle READ swipeAngle WRITE setSwipeAngle)
    Q_PRIVATE_PROPERTY(QSwipeGesture::d_func(), qreal velocity READ velocity WRITE setVelocity)

public:
    enum SwipeDirection { NoDirection, Left, Right, Up, Down };
    Q_ENUM(SwipeDirection)

    explicit QSwipeGesture(QObject *parent = nullptr);
    ~QSwipeGesture();

    SwipeDirection horizontalDirection() const;
    SwipeDirection verticalDirection() const;

    qreal swipeAngle() const;
    void setSwipeAngle(qreal value);

    friend class QSwipeGestureRecognizer;
};

class BOBUIapGesturePrivate;
class Q_WIDGETS_EXPORT BOBUIapGesture : public QGesture
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(BOBUIapGesture)

    Q_PROPERTY(QPointF position READ position WRITE setPosition)

public:
    explicit BOBUIapGesture(QObject *parent = nullptr);
    ~BOBUIapGesture();

    QPointF position() const;
    void setPosition(const QPointF &pos);

    friend class BOBUIapGestureRecognizer;
};

class BOBUIapAndHoldGesturePrivate;
class Q_WIDGETS_EXPORT BOBUIapAndHoldGesture : public QGesture
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(BOBUIapAndHoldGesture)

    Q_PROPERTY(QPointF position READ position WRITE setPosition)

public:
    explicit BOBUIapAndHoldGesture(QObject *parent = nullptr);
    ~BOBUIapAndHoldGesture();

    QPointF position() const;
    void setPosition(const QPointF &pos);

    static void setTimeout(int msecs);
    static int timeout();

    friend class BOBUIapAndHoldGestureRecognizer;
};

class QGesture;
class QGestureEventPrivate;
class Q_WIDGETS_EXPORT QGestureEvent : public QEvent
{
public:
    explicit QGestureEvent(const QList<QGesture *> &gestures);
    ~QGestureEvent();

    QList<QGesture *> gestures() const;
    QGesture *gesture(BobUI::GestureType type) const;

    QList<QGesture *> activeGestures() const;
    QList<QGesture *> canceledGestures() const;

    using QEvent::setAccepted;
    using QEvent::isAccepted;
    using QEvent::accept;
    using QEvent::ignore;

    void setAccepted(QGesture *, bool);
    void accept(QGesture *);
    void ignore(QGesture *);
    bool isAccepted(QGesture *) const;

    void setAccepted(BobUI::GestureType, bool);
    void accept(BobUI::GestureType);
    void ignore(BobUI::GestureType);
    bool isAccepted(BobUI::GestureType) const;

    void setWidget(QWidget *widget);
    QWidget *widget() const;

#if BOBUI_CONFIG(graphicsview)
    QPointF mapToGraphicsScene(const QPointF &gesturePoint) const;
#endif

private:
    QList<QGesture *> m_gestures;
    QWidget *m_widget;
    QMap<BobUI::GestureType, bool> m_accepted;
    QMap<BobUI::GestureType, QWidget *> m_targetWidgets;

    friend class QApplication;
    friend class QGestureManager;
};

#  ifndef BOBUI_NO_DEBUG_STREAM
Q_WIDGETS_EXPORT QDebug operator<<(QDebug, const QGesture *);
Q_WIDGETS_EXPORT QDebug operator<<(QDebug, const QGestureEvent *);
#  endif

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN_TAGGED(QGesture::GestureCancelPolicy,
                               QGesture__GestureCancelPolicy, Q_WIDGETS_EXPORT)

#endif // BOBUI_NO_GESTURES

#endif // QGESTURE_H
