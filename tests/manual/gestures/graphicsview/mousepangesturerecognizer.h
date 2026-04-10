// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef MOUSEPANGESTURERECOGNIZER_H
#define MOUSEPANGESTURERECOGNIZER_H

#include <QGestureRecognizer>

class MousePanGestureRecognizer : public QGestureRecognizer
{
public:
    MousePanGestureRecognizer();

    QGesture* create(QObject *target);
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event);
    void reset(QGesture *state);
};

#endif // MOUSEPANGESTURERECOGNIZER_H
