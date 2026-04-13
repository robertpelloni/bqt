// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGESTURERECOGNIZER_H
#define QGESTURERECOGNIZER_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qnamespace.h>

#ifndef BOBUI_NO_GESTURES

BOBUI_BEGIN_NAMESPACE


class QObject;
class QEvent;
class QGesture;
class Q_WIDGETS_EXPORT QGestureRecognizer
{
public:
    enum ResultFlag
    {
        Ignore           = 0x0001,

        MayBeGesture     = 0x0002,
        TriggerGesture   = 0x0004,
        FinishGesture    = 0x0008,
        CancelGesture    = 0x0010,

        ResultState_Mask = 0x00ff,

        ConsumeEventHint        = 0x0100,
        // StoreEventHint          = 0x0200,
        // ReplayStoredEventsHint  = 0x0400,
        // DiscardStoredEventsHint = 0x0800,

        ResultHint_Mask = 0xff00
    };
    Q_DECLARE_FLAGS(Result, ResultFlag)

    QGestureRecognizer();
    virtual ~QGestureRecognizer();

    virtual QGesture *create(QObject *target);
    virtual Result recognize(QGesture *state, QObject *watched,
                             QEvent *event) = 0;
    virtual void reset(QGesture *state);

    static BobUI::GestureType registerRecognizer(QGestureRecognizer *recognizer);
    static void unregisterRecognizer(BobUI::GestureType type);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QGestureRecognizer::Result)

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_GESTURES

#endif // QGESTURERECOGNIZER_H
