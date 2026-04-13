// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACTION_P_H
#define QACTION_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUIGui/qaction.h>
#include <BobUIGui/qfont.h>
#if BOBUI_CONFIG(shortcut)
#  include <BobUIGui/private/qshortcutmap_p.h>
#endif

#include <BobUICore/qpointer.h>
#include "private/qobject_p.h"

BOBUI_REQUIRE_CONFIG(action);

BOBUI_BEGIN_NAMESPACE

class QShortcutMap;

class Q_GUI_EXPORT QActionPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAction)
public:
    QActionPrivate();
    ~QActionPrivate();

    virtual void destroy();

#if BOBUI_CONFIG(shortcut)
    virtual QShortcutMap::ContextMatcher contextMatcher() const;
#endif

    static QActionPrivate *get(QAction *q)
    {
        return q->d_func();
    }

    bool setEnabled(bool enable, bool byGroup);
    void setVisible(bool b);

    QPointer<QActionGroup> group;
    QString text;
    QString iconText;
    QIcon icon;
    QString tooltip;
    QString statustip;
    QString whatsthis;
#if BOBUI_CONFIG(shortcut)
    QList<QKeySequence> shortcuts;
#endif
    QVariant userData;

    QObjectList associatedObjects;
    virtual QObject *menu() const;
    virtual void setMenu(QObject *menu);

#if BOBUI_CONFIG(shortcut)
    QList<int> shortcutIds;
    BobUI::ShortcutContext shortcutContext = BobUI::WindowShortcut;
    uint autorepeat : 1;
#endif
    QFont font;
    uint enabled : 1, explicitEnabled : 1, explicitEnabledValue : 1;
    uint visible : 1, forceInvisible : 1;
    uint checkable : 1;
    uint checked : 1;
    uint separator : 1;
    uint fontSet : 1;

    int iconVisibleInMenu : 2;  // Only has values -1, 0, and 1
    int shortcutVisibleInContextMenu : 2; // Only has values -1, 0, and 1

    QAction::MenuRole menuRole = QAction::TextHeuristicRole;
    QAction::Priority priority = QAction::NormalPriority;

#if BOBUI_CONFIG(shortcut)
    void redoGrab(QShortcutMap &map);
    void redoGrabAlternate(QShortcutMap &map);
    void setShortcutEnabled(bool enable, QShortcutMap &map);
#endif // BOBUI_NO_SHORTCUT

    bool showStatusText(QObject *widget, const QString &str);
    void sendDataChanged();
};

BOBUI_END_NAMESPACE

#endif // QACTION_P_H
