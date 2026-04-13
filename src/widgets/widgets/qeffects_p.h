// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QEFFECTS_P_H
#define QEFFECTS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qeffects.cpp, qcombobox.cpp, qpopupmenu.cpp and bobuiooltip.cpp.
// This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//

#include "BobUICore/qnamespace.h"
#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>

BOBUI_REQUIRE_CONFIG(effects);

BOBUI_BEGIN_NAMESPACE

class QWidget;

struct QEffects
{
    enum Direction {
        LeftScroll  = 0x0001,
        RightScroll = 0x0002,
        UpScroll    = 0x0004,
        DownScroll  = 0x0008
    };

    typedef uint DirFlags;
};

extern void Q_WIDGETS_EXPORT qScrollEffect(QWidget*, QEffects::DirFlags dir = QEffects::DownScroll, int time = -1);
extern void Q_WIDGETS_EXPORT qFadeEffect(QWidget*, int time = -1);

BOBUI_END_NAMESPACE

#endif // QEFFECTS_P_H
