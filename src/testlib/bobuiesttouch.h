// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTTOUCH_H
#define BOBUIESTTOUCH_H

#if 0
// inform syncbobui
#pragma bobui_no_master_include
#endif

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/bobuiestassert.h>
#include <BobUITest/bobuiestsystem.h>
#include <BobUITest/bobuiestspontaneevent.h>
#include <BobUICore/qmap.h>
#include <BobUIGui/qevent.h>
#include <BobUIGui/qpointingdevice.h>
#include <BobUIGui/qwindow.h>
#include <BobUIGui/qpointingdevice.h>
#ifdef BOBUI_WIDGETS_LIB
#include <BobUIWidgets/qwidget.h>
#include <BobUIWidgets/bobuiestsupport_widgets.h>
#else
#include <BobUIGui/bobuiestsupport_gui.h>
#endif

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest
{
#if defined(BOBUI_WIDGETS_LIB) || defined(Q_QDOC)
    inline
    BOBUIouchEventWidgetSequence touchEvent(QWidget *widget,
                                   QPointingDevice *device,
                                   bool autoCommit = true)
    {
        return BOBUIouchEventWidgetSequence(widget, device, autoCommit);
    }
#endif
    inline
    BOBUIouchEventSequence touchEvent(QWindow *window,
                                   QPointingDevice *device,
                                   bool autoCommit = true)
    {
        return BOBUIouchEventSequence(window, device, autoCommit);
    }

}

BOBUI_END_NAMESPACE

#endif // BOBUIESTTOUCH_H
