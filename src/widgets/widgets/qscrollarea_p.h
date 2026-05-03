// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSCROLLAREA_P_H
#define QSCROLLAREA_P_H

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

#include "private/qabstractscrollarea_p.h"
#include <BobUIWidgets/qscrollbar.h>

#include <BobUICore/qpointer.h>

BOBUI_REQUIRE_CONFIG(scrollarea);

BOBUI_BEGIN_NAMESPACE

class QScrollAreaPrivate: public QAbstractScrollAreaPrivate
{
    Q_DECLARE_PUBLIC(QScrollArea)

public:
    QScrollAreaPrivate(): resizable(false) {}
    void updateScrollBars();
    void updateWidgetPosition();
    QPointer<QWidget> widget;
    mutable QSize widgetSize;
    bool resizable;
    BobUI::Alignment alignment;
};

BOBUI_END_NAMESPACE

#endif
