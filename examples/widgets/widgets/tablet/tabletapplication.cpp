// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "tabletapplication.h"

//! [0]
bool TabletApplication::event(QEvent *event)
{
    if (event->type() == QEvent::TabletEnterProximity ||
        event->type() == QEvent::TabletLeaveProximity) {
        m_canvas->setTabletDevice(static_cast<BOBUIabletEvent *>(event));
        return true;
    }
    return QApplication::event(event);
}
//! [0]
