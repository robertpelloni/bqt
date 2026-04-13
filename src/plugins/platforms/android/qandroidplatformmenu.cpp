// Copyright (C) 2012 BogDan Vatra <bogdan@kde.org>
// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "androidjnimenu.h"
#include "qandroidplatformmenu.h"
#include "qandroidplatformmenuitem.h"

#include <BobUICore/qjnienvironment.h>
#include <BobUICore/qjniobject.h>

BOBUI_BEGIN_NAMESPACE

QAndroidPlatformMenu::QAndroidPlatformMenu()
{
    m_enabled = true;
    m_isVisible = true;
}

QAndroidPlatformMenu::~QAndroidPlatformMenu()
{
    BobUIAndroidMenu::androidPlatformMenuDestroyed(this);
}

void QAndroidPlatformMenu::insertMenuItem(QPlatformMenuItem *menuItem, QPlatformMenuItem *before)
{
    QMutexLocker lock(&m_menuItemsMutex);
    m_menuItems.insert(std::find(m_menuItems.begin(),
                                 m_menuItems.end(),
                                 static_cast<QAndroidPlatformMenuItem *>(before)),
                       static_cast<QAndroidPlatformMenuItem *>(menuItem));
    m_menuHash.insert(m_nextMenuId++, menuItem);
}

void QAndroidPlatformMenu::removeMenuItem(QPlatformMenuItem *menuItem)
{
    QMutexLocker lock(&m_menuItemsMutex);
    PlatformMenuItemsType::iterator it = std::find(m_menuItems.begin(),
                                                   m_menuItems.end(),
                                                   static_cast<QAndroidPlatformMenuItem *>(menuItem));
    if (it != m_menuItems.end())
        m_menuItems.erase(it);

    {
        int maxId = -1;
        QHash<int, QPlatformMenuItem *>::iterator it = m_menuHash.begin();
        while (it != m_menuHash.end()) {
            if (it.value() == menuItem) {
                it = m_menuHash.erase(it);
            } else {
                maxId = qMax(maxId, it.key());
                ++it;
            }
        }

        m_nextMenuId = maxId + 1;
    }
}

void QAndroidPlatformMenu::syncMenuItem(QPlatformMenuItem *menuItem)
{
    PlatformMenuItemsType::iterator it;
    for (it = m_menuItems.begin(); it != m_menuItems.end(); ++it) {
        if ((*it)->tag() == menuItem->tag())
            break;
    }

    if (it != m_menuItems.end())
        BobUIAndroidMenu::syncMenu(this);
}

void QAndroidPlatformMenu::syncSeparatorsCollapsible(bool enable)
{
    Q_UNUSED(enable);
}

void QAndroidPlatformMenu::setText(const QString &text)
{
    m_text = text;
}

QString QAndroidPlatformMenu::text() const
{
    return m_text;
}

void QAndroidPlatformMenu::setIcon(const QIcon &icon)
{
    m_icon = icon;
}

QIcon QAndroidPlatformMenu::icon() const
{
    return m_icon;
}

void QAndroidPlatformMenu::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool QAndroidPlatformMenu::isEnabled() const
{
    return m_enabled;
}

void QAndroidPlatformMenu::setVisible(bool visible)
{
    m_isVisible = visible;
}

bool QAndroidPlatformMenu::isVisible() const
{
    return m_isVisible;
}

void QAndroidPlatformMenu::showPopup(const QWindow *parentWindow, const QRect &targetRect, const QPlatformMenuItem *item)
{
    Q_UNUSED(parentWindow);
    Q_UNUSED(item);
    setVisible(true);
    BobUIAndroidMenu::showContextMenu(this, targetRect);
}

QPlatformMenuItem *QAndroidPlatformMenu::menuItemForTag(quintptr tag) const
{
    for (QAndroidPlatformMenuItem *menuItem : m_menuItems) {
        if (menuItem->tag() == tag)
            return menuItem;
    }

    return nullptr;
}

QPlatformMenuItem *QAndroidPlatformMenu::menuItemAt(int position) const
{
    if (position < m_menuItems.size())
        return m_menuItems[position];
    return 0;
}

int QAndroidPlatformMenu::menuId(QPlatformMenuItem *menu) const
{
    QHash<int, QPlatformMenuItem *>::const_iterator it;
    for (it = m_menuHash.constBegin(); it != m_menuHash.constEnd(); ++it) {
        if (it.value() == menu)
            return it.key();
    }

    return -1;
}

QPlatformMenuItem *QAndroidPlatformMenu::menuItemForId(int menuId) const
{
    return m_menuHash.value(menuId);
}

QAndroidPlatformMenu::PlatformMenuItemsType QAndroidPlatformMenu::menuItems() const
{
    return m_menuItems;
}

QMutex *QAndroidPlatformMenu::menuItemsMutex()
{
    return &m_menuItemsMutex;
}

BOBUI_END_NAMESPACE
