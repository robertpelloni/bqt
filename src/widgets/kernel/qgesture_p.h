// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGESTURE_P_H
#define QGESTURE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qrect.h"
#include "qpoint.h"
#include "qgesture.h"
#include "qbasictimer.h"
#include "qelapsedtimer.h"
#include "private/qobject_p.h"

#ifndef BOBUI_NO_GESTURES

BOBUI_BEGIN_NAMESPACE

class QGesturePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGesture)

public:
    QGesturePrivate()
        : gestureType(BobUI::CustomGesture), state(BobUI::NoGesture),
          isHotSpotSet(false), gestureCancelPolicy(0)
    {
    }

    BobUI::GestureType gestureType;
    BobUI::GestureState state;
    QPointF hotSpot;
    QPointF sceneHotSpot;
    uint isHotSpotSet : 1;
    uint gestureCancelPolicy : 2;
};

class QPanGesturePrivate : public QGesturePrivate
{
    Q_DECLARE_PUBLIC(QPanGesture)

public:
    QPanGesturePrivate()
        : acceleration(0), xVelocity(0), yVelocity(0), pointCount(2)
    {
    }

    qreal horizontalVelocity() const { return xVelocity; }
    void setHorizontalVelocity(qreal value) { xVelocity = value; }
    qreal verticalVelocity() const { return yVelocity; }
    void setVerticalVelocity(qreal value) { yVelocity = value; }

    QPointF lastOffset;
    QPointF offset;
    QPoint startPosition;
    qreal acceleration;
    qreal xVelocity;
    qreal yVelocity;
    int pointCount; // ### fixme BobUI 5.5: Add accessor to QPanGesture.
};

class QPinchGesturePrivate : public QGesturePrivate
{
    Q_DECLARE_PUBLIC(QPinchGesture)

public:
    QPinchGesturePrivate()
        : totalScaleFactor(1), lastScaleFactor(1), scaleFactor(1),
          totalRotationAngle(0), lastRotationAngle(0), rotationAngle(0),
          isNewSequence(true)
    {
    }

    QPinchGesture::ChangeFlags totalChangeFlags;
    QPinchGesture::ChangeFlags changeFlags;

    QPointF startCenterPoint;
    QPointF lastCenterPoint;
    QPointF centerPoint;

    qreal totalScaleFactor;
    qreal lastScaleFactor;
    qreal scaleFactor;

    qreal totalRotationAngle;
    qreal lastRotationAngle;
    qreal rotationAngle;

    bool isNewSequence;
    QPointF startPosition[2];
};

class QSwipeGesturePrivate : public QGesturePrivate
{
    Q_DECLARE_PUBLIC(QSwipeGesture)

public:
    enum State {
        NoGesture,
        Started,
        ThreePointsReached
    };

    QSwipeGesturePrivate()
        : horizontalDirection(QSwipeGesture::NoDirection),
          verticalDirection(QSwipeGesture::NoDirection),
          swipeAngle(0),
          state(NoGesture), velocityValue(0)
    {
    }

    qreal velocity() const { return velocityValue; }
    void setVelocity(qreal value) { velocityValue = value; }

    QSwipeGesture::SwipeDirection horizontalDirection;
    QSwipeGesture::SwipeDirection verticalDirection;
    qreal swipeAngle;

    QPoint lastPositions[3];
    State state;
    qreal velocityValue;
    QElapsedTimer time;
};

class BOBUIapGesturePrivate : public QGesturePrivate
{
    Q_DECLARE_PUBLIC(BOBUIapGesture)

public:
    BOBUIapGesturePrivate()
    {
    }

    QPointF position;
};

class BOBUIapAndHoldGesturePrivate : public QGesturePrivate
{
    Q_DECLARE_PUBLIC(BOBUIapAndHoldGesture)

public:
    BOBUIapAndHoldGesturePrivate()
    {
    }

    QPointF position;
    QBasicTimer tapAndHoldTimer;
    static int Timeout;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_GESTURES

#endif // QGESTURE_P_H
