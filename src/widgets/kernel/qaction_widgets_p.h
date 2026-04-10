// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QACTION_WIDGETS_P_H
#define QACTION_WIDGETS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/qaction_p.h>
#if BOBUI_CONFIG(menu)
#include <BobUIWidgets/qmenu.h>
#endif

#include <BobUICore/qpointer.h>

BOBUI_REQUIRE_CONFIG(action);

BOBUI_BEGIN_NAMESPACE

class QShortcutMap;

class Q_WIDGETS_EXPORT BobUIWidgetsActionPrivate : public QActionPrivate
{
    Q_DECLARE_PUBLIC(QAction)
public:
    BobUIWidgetsActionPrivate() = default;
    ~BobUIWidgetsActionPrivate();

    void destroy() override;

#if BOBUI_CONFIG(shortcut)
    QShortcutMap::ContextMatcher contextMatcher() const override;
#endif

#if BOBUI_CONFIG(menu)
    QPointer<QMenu> m_menu;

    QObject *menu() const override;
    void setMenu(QObject *menu) override;
#endif
};

BOBUI_END_NAMESPACE

#endif // QACTION_WIDGETS_P_H
