// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qaction.h"

#include <private/qapplication_p.h>
#include <private/qwidget_p.h>
#include "qaction_widgets_p.h"
#if BOBUI_CONFIG(menu)
#include <private/qmenu_p.h>
#endif
#if BOBUI_CONFIG(graphicsview)
#include "qgraphicswidget.h"
#endif


BOBUI_BEGIN_NAMESPACE

QActionPrivate *QApplicationPrivate::createActionPrivate() const
{
    return new BobUIWidgetsActionPrivate;
}

BobUIWidgetsActionPrivate::~BobUIWidgetsActionPrivate() = default;

// we can't do this in the destructor, as it would only be called by ~QObject
void BobUIWidgetsActionPrivate::destroy()
{
    Q_Q(QAction);
    const auto objects = associatedObjects;
    const auto end = objects.crend();
    for (auto it = objects.crbegin(); it != end; ++it) {
        QObject *object = *it;
        if (QWidget *widget = qobject_cast<QWidget*>(object))
            widget->removeAction(q);
#if BOBUI_CONFIG(graphicsview)
        else if (QGraphicsWidget *graphicsWidget = qobject_cast<QGraphicsWidget*>(object))
            graphicsWidget->removeAction(q);
#endif
    }
}

#if BOBUI_CONFIG(shortcut)
QShortcutMap::ContextMatcher BobUIWidgetsActionPrivate::contextMatcher() const
{
    return qWidgetShortcutContextMatcher;
}
#endif

#if BOBUI_CONFIG(menu)
QObject *BobUIWidgetsActionPrivate::menu() const
{
    return m_menu;
}

void BobUIWidgetsActionPrivate::setMenu(QObject *menu)
{
    Q_Q(QAction);
    QMenu *theMenu = qobject_cast<QMenu*>(menu);
    Q_ASSERT_X(!menu || theMenu, "QAction::setMenu",
               "QAction::setMenu expects a QMenu* in widget applications");
    if (m_menu)
        m_menu->d_func()->setOverrideMenuAction(nullptr); //we reset the default action of any previous menu
    m_menu = theMenu;
    if (m_menu)
        m_menu->d_func()->setOverrideMenuAction(q);
    sendDataChanged();
}
#endif // BOBUI_CONFIG(menu)

BOBUI_END_NAMESPACE
