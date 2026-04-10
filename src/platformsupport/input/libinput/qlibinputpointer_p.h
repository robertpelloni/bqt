// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QLIBINPUTPOINTER_P_H
#define QLIBINPUTPOINTER_P_H

#include <BobUICore/QPoint>
#include <BobUICore/private/qglobal_p.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

struct libinput_event_pointer;

BOBUI_BEGIN_NAMESPACE

class QLibInputPointer
{
public:
    QLibInputPointer();

    void processButton(libinput_event_pointer *e);
    void processMotion(libinput_event_pointer *e);
    void processAbsMotion(libinput_event_pointer *e);
    void processAxis(libinput_event_pointer *e);

    void setPos(const QPoint &pos);

private:
    QPoint m_pos;
    BobUI::MouseButtons m_buttons;
};

BOBUI_END_NAMESPACE

#endif
