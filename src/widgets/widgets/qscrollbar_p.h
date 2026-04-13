// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSCROLLBAR_P_H
#define QSCROLLBAR_P_H

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

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qscrollbar.h"
#include "private/qabstractslider_p.h"
#include "qstyle.h"

#include <BobUICore/qbasictimer.h>

BOBUI_REQUIRE_CONFIG(scrollbar);

BOBUI_BEGIN_NAMESPACE

class QScrollBarPrivate : public QAbstractSliderPrivate
{
    Q_DECLARE_PUBLIC(QScrollBar)
public:
    QStyle::SubControl pressedControl;
    bool pointerOutsidePressedControl;

    int clickOffset, snapBackPosition;

    void activateControl(uint control, int threshold = 500);
    void stopRepeatAction();
    int pixelPosToRangeValue(int pos) const;
    void init();
    bool updateHoverControl(const QPoint &pos);
    QStyle::SubControl newHoverControl(const QPoint &pos);

    QStyle::SubControl hoverControl;
    QRect hoverRect;

    bool transient;
    void setTransient(bool value);

    bool flashed;
    QBasicTimer flashTimer;
    void flash();
};

BOBUI_END_NAMESPACE

#endif // QSCROLLBAR_P_H
