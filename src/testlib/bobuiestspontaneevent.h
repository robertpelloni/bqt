// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTSPONTANEEVENT_H
#define BOBUIESTSPONTANEEVENT_H

#include <BobUICore/qcoreevent.h>

#if 0
// inform syncbobui
#pragma bobui_no_master_include
#endif

BOBUI_BEGIN_NAMESPACE

class QSpontaneKeyEvent
{
public:
    static inline void setSpontaneous(QEvent *ev)
    {
        ev->setSpontaneous();
    }
};

BOBUI_END_NAMESPACE

#endif
