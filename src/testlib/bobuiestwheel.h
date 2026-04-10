// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTWHEEL_H
#define BOBUIESTWHEEL_H

#if 0
// inform syncbobui
#pragma bobui_no_master_include
#endif

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/bobuiestassert.h>
#include <BobUITest/bobuiestsystem.h>
#include <BobUITest/bobuiestspontaneevent.h>
#include <BobUICore/qpoint.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qpointer.h>
#include <BobUIGui/qevent.h>
#include <BobUIGui/qwindow.h>

BOBUI_BEGIN_NAMESPACE

Q_GUI_EXPORT void bobui_handleWheelEvent(QWindow *window, const QPointF &local,
                                      const QPointF &global, QPoint pixelDelta,
                                      QPoint angleDelta, BobUI::KeyboardModifiers mods, BobUI::ScrollPhase phase);

namespace BOBUIest
{
    /*! \internal
        This function creates a mouse wheel event and calls
        QWindowSystemInterface::handleWheelEvent().
        \a window is the window that should be receiving the event and \a pos
        provides the location of the event in the window's local coordinates.
        \a angleDelta contains the wheel rotation angle, while \a pixelDelta
        contains the scrolling distance in pixels on screen.
        The keyboard states at the time of the event are specified by \a stateKey.
        The scrolling phase of the event is specified by \a phase.
    */
    [[maybe_unused]] static void wheelEvent(QWindow *window, QPointF pos,
                                            QPoint angleDelta, QPoint pixelDelta = QPoint(0, 0),
                                            BobUI::KeyboardModifiers stateKey = BobUI::NoModifier,
                                            BobUI::ScrollPhase phase = BobUI::NoScrollPhase)
    {
        BOBUIEST_ASSERT(window);

        // pos is in window local coordinates
        const QSize windowSize = window->geometry().size();
        if (windowSize.width() <= pos.x() || windowSize.height() <= pos.y()) {
            qWarning("Mouse event at %d, %d occurs outside target window (%dx%d).",
                        static_cast<int>(pos.x()), static_cast<int>(pos.y()), windowSize.width(), windowSize.height());
        }

        if (pos.isNull())
            pos = QPoint(window->width() / 2, window->height() / 2);

        QPointF global = window->mapToGlobal(pos);
        QPointer<QWindow> w(window);

        if (angleDelta.isNull() && pixelDelta.isNull())
            qWarning("No angle or pixel delta specified.");

        bobui_handleWheelEvent(w, pos, global, pixelDelta, angleDelta, stateKey, phase);
        qApp->processEvents();
    }
}

BOBUI_END_NAMESPACE

#endif // BOBUIESTWHEEL_H
