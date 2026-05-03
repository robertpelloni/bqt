// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSTANDARDGESTURES_P_H
#define QSTANDARDGESTURES_P_H

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
#include "qgesturerecognizer.h"
#include "private/qgesture_p.h"

#ifndef BOBUI_NO_GESTURES

BOBUI_BEGIN_NAMESPACE

class QPanGestureRecognizer : public QGestureRecognizer
{
public:
    explicit QPanGestureRecognizer(int pointCount = 2) : m_pointCount(pointCount) {}

    QGesture *create(QObject *target) override;
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event) override;
    void reset(QGesture *state) override;

private:
    const int m_pointCount;
};

class QPinchGestureRecognizer : public QGestureRecognizer
{
public:
    QPinchGestureRecognizer();

    QGesture *create(QObject *target) override;
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event) override;
    void reset(QGesture *state) override;
};

class QSwipeGestureRecognizer : public QGestureRecognizer
{
public:
    QSwipeGestureRecognizer();

    QGesture *create(QObject *target) override;
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event) override;
    void reset(QGesture *state) override;
};

class BOBUIapGestureRecognizer : public QGestureRecognizer
{
public:
    BOBUIapGestureRecognizer();

    QGesture *create(QObject *target) override;
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event) override;
    void reset(QGesture *state) override;
};

class BOBUIapAndHoldGestureRecognizer : public QGestureRecognizer
{
public:
    BOBUIapAndHoldGestureRecognizer();

    QGesture *create(QObject *target) override;
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event) override;
    void reset(QGesture *state) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_GESTURES

#endif // QSTANDARDGESTURES_P_H
