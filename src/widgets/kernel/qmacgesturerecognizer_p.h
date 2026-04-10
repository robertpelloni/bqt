// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QMACSWIPEGESTURERECOGNIZER_MAC_P_H
#define QMACSWIPEGESTURERECOGNIZER_MAC_P_H

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
#include "qpoint.h"
#include "qgesturerecognizer.h"
#include <BobUICore/qbasictimer.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qpointer.h>

#ifndef BOBUI_NO_GESTURES

BOBUI_BEGIN_NAMESPACE

class QMacSwipeGestureRecognizer : public QGestureRecognizer
{
public:
    QMacSwipeGestureRecognizer();

    QGesture *create(QObject *target) override;
    QGestureRecognizer::Result recognize(QGesture *gesture, QObject *watched, QEvent *event) override;
    void reset(QGesture *gesture) override;
};

class QMacPinchGestureRecognizer : public QGestureRecognizer
{
public:
    QMacPinchGestureRecognizer();

    QGesture *create(QObject *target) override;
    QGestureRecognizer::Result recognize(QGesture *gesture, QObject *watched, QEvent *event) override;
    void reset(QGesture *gesture) override;
};

class QMacPanGestureRecognizer : public QObject, public QGestureRecognizer
{
    Q_OBJECT
public:
    QMacPanGestureRecognizer();

    QGesture *create(QObject *target) override;
    QGestureRecognizer::Result recognize(QGesture *gesture, QObject *watched, QEvent *event) override;
    void reset(QGesture *gesture) override;
protected:
    void timerEvent(BOBUIimerEvent *ev) override;
private:
    QPointF _startPos;
    QBasicTimer _panTimer;
    bool _panCanceled;
    QPointer<QObject> _target;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_GESTURES

#endif // QMACSWIPEGESTURERECOGNIZER_MAC_P_H
