// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGUIACTIONGROUP_P_H
#define QGUIACTIONGROUP_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists for the convenience
// of other BobUI classes. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUIGui/qactiongroup.h>
#include <BobUIGui/qfont.h>
#if BOBUI_CONFIG(shortcut)
#  include <BobUIGui/private/qshortcutmap_p.h>
#endif

#include <BobUICore/qpointer.h>
#include "private/qobject_p.h"

BOBUI_REQUIRE_CONFIG(action);

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QActionGroupPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QActionGroup)
public:
    enum Signal { Triggered, Hovered };

    QActionGroupPrivate();
    ~QActionGroupPrivate();

    virtual void emitSignal(Signal, QAction *) {}

    QList<QAction *> actions;
    QPointer<QAction> current;
    uint enabled : 1;
    uint visible : 1;
    QActionGroup::ExclusionPolicy exclusionPolicy = QActionGroup::ExclusionPolicy::Exclusive;

private:
    void _q_actionTriggered();  //private slot
    void _q_actionChanged();    //private slot
    void _q_actionHovered();    //private slot
};

BOBUI_END_NAMESPACE

#endif // QACTIONGROUP_P_H
