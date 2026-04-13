// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFLICKGESTURE_P_H
#define QFLICKGESTURE_P_H

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
#include "qevent.h"
#include "qgesturerecognizer.h"
#include "private/qgesture_p.h"
#include "qscroller.h"

#include <BobUICore/qpointer.h>
#include "qscopedpointer.h"

#ifndef BOBUI_NO_GESTURES

BOBUI_BEGIN_NAMESPACE

class QFlickGesturePrivate;
class QGraphicsItem;

class Q_WIDGETS_EXPORT QFlickGesture : public QGesture
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QFlickGesture)

public:
    QFlickGesture(QObject *receiver, BobUI::MouseButton button, QObject *parent = nullptr);
    ~QFlickGesture();

    friend class QFlickGestureRecognizer;
};

class QFlickGesturePrivate : public QGesturePrivate
{
    Q_DECLARE_PUBLIC(QFlickGesture)
public:
    QFlickGesturePrivate();

    QPointer<QObject> receiver;
    QScroller *receiverScroller;
    BobUI::MouseButton button; // NoButton == Touch
    bool macIgnoreWheel;
};

class QFlickGestureRecognizer : public QGestureRecognizer
{
public:
    QFlickGestureRecognizer(BobUI::MouseButton button);

    QGesture *create(QObject *target) override;
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event) override;
    void reset(QGesture *state) override;

private:
    BobUI::MouseButton button; // NoButton == Touch
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_GESTURES

#endif // QFLICKGESTURE_P_H
