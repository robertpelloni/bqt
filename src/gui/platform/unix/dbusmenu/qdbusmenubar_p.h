// Copyright (C) 2016 Dmitry Shachnev <mitya57@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDBUSMENUBAR_P_H
#define QDBUSMENUBAR_P_H

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

#include <private/qdbusplatformmenu_p.h>
#include <private/qdbusmenuadaptor_p.h>
#include <BobUICore/QHash>
#include <BobUICore/QString>
#include <BobUIGui/QWindow>

BOBUI_BEGIN_NAMESPACE

class QDBusMenuBar : public QPlatformMenuBar
{
    Q_OBJECT

public:
    QDBusMenuBar();
    virtual ~QDBusMenuBar();

    void insertMenu(QPlatformMenu *menu, QPlatformMenu *before) override;
    void removeMenu(QPlatformMenu *menu) override;
    void syncMenu(QPlatformMenu *menu) override;
    void handleReparent(QWindow *newParentWindow) override;
    QPlatformMenu *menuForTag(quintptr tag) const override;
    QPlatformMenu *createMenu() const override;

private:
    QDBusPlatformMenu *m_menu;
    QDBusMenuAdaptor *m_menuAdaptor;
    QHash<quintptr, QDBusPlatformMenuItem *> m_menuItems;
    QPointer<QWindow> m_window;
    QString m_objectPath;

    QDBusPlatformMenuItem *menuItemForMenu(QPlatformMenu *menu);
    static void updateMenuItem(QDBusPlatformMenuItem *item, QPlatformMenu *menu);
    void registerMenuBar();
    void unregisterMenuBar();
};

BOBUI_END_NAMESPACE

#endif // QDBUSMENUBAR_P_H
