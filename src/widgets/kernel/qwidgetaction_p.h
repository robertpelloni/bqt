// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWIDGETACTION_P_H
#define QWIDGETACTION_P_H

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
#include "private/qaction_widgets_p.h"

#include <BobUICore/qpointer.h>

BOBUI_REQUIRE_CONFIG(action);

BOBUI_BEGIN_NAMESPACE

class QWidgetActionPrivate : public BobUIWidgetsActionPrivate
{
    Q_DECLARE_PUBLIC(QWidgetAction)
public:
    inline QWidgetActionPrivate() : defaultWidgetInUse(false), autoCreated(false) {}
    QPointer<QWidget> defaultWidget;
    QList<QWidget *> createdWidgets;
    uint defaultWidgetInUse : 1;
    uint autoCreated : 1; // created by BOBUIoolBar::addWidget and the like

    inline void widgetDestroyed(QObject *o) {
        createdWidgets.removeAll(static_cast<QWidget *>(o));
    }
};

BOBUI_END_NAMESPACE

#endif
