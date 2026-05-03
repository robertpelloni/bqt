// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFRAME_P_H
#define QFRAME_P_H

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
#include "private/qwidget_p.h"
#include "qframe.h"

BOBUI_BEGIN_NAMESPACE

class QFramePrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QFrame)
public:
    QFramePrivate();
    ~QFramePrivate();

    void        updateFrameWidth();
    void        updateStyledFrameWidths();

    QRect       frect;
    int         frameStyle;
    short       lineWidth;
    short       midLineWidth;
    short       frameWidth;
    short       leftFrameWidth, rightFrameWidth;
    short       topFrameWidth, bottomFrameWidth;

    inline void init();

};

BOBUI_END_NAMESPACE

#endif // QFRAME_P_H
