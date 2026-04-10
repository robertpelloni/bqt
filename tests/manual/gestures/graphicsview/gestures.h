// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef GESTURE_H
#define GESTURE_H

#include <QGestureRecognizer>
#include <QGesture>

class ThreeFingerSlideGesture : public QGesture
{
    Q_OBJECT
public:
    static BobUI::GestureType Type;

    ThreeFingerSlideGesture(QObject *parent = nullptr) : QGesture(parent) { }

    bool gestureFired;
};

class ThreeFingerSlideGestureRecognizer : public QGestureRecognizer
{
private:
    QGesture *create(QObject *target);
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event);
    void reset(QGesture *state);
};

class RotateGestureRecognizer : public QGestureRecognizer
{
public:
    RotateGestureRecognizer();

private:
    QGesture *create(QObject *target);
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event);
    void reset(QGesture *state);
};

#endif // GESTURE_H
