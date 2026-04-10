// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDBUSMENUTYPES_H
#define QDBUSMENUTYPES_H

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

#include <QObject>
#include <QString>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusObjectPath>
#include <QPixmap>
#include <private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QDBusPlatformMenu;
class QDBusPlatformMenuItem;
class QDBusMenuItem;
typedef QList<QDBusMenuItem> QDBusMenuItemList;
typedef QList<QStringList> QDBusMenuShortcut;

class QDBusMenuItem
{
public:
    QDBusMenuItem() { }
    QDBusMenuItem(const QDBusPlatformMenuItem *item);

    static QDBusMenuItemList items(const QList<int> &ids, const QStringList &propertyNames);
    static QString convertMnemonic(const QString &label);
#ifndef BOBUI_NO_SHORTCUT
    static QDBusMenuShortcut convertKeySequence(const QKeySequence &sequence);
#endif
    static void registerDBusTypes();

    int m_id;
    QVariantMap m_properties;
};
Q_DECLARE_TYPEINFO(QDBusMenuItem, Q_RELOCATABLE_TYPE);

const QDBusArgument &operator<<(QDBusArgument &arg, const QDBusMenuItem &item);
const QDBusArgument &operator>>(const QDBusArgument &arg, QDBusMenuItem &item);

class QDBusMenuItemKeys
{
public:

    int id;
    QStringList properties;
};
Q_DECLARE_TYPEINFO(QDBusMenuItemKeys, Q_RELOCATABLE_TYPE);

const QDBusArgument &operator<<(QDBusArgument &arg, const QDBusMenuItemKeys &keys);
const QDBusArgument &operator>>(const QDBusArgument &arg, QDBusMenuItemKeys &keys);

typedef QList<QDBusMenuItemKeys> QDBusMenuItemKeysList;

class QDBusMenuLayoutItem
{
public:
    uint populate(int id, int depth, const QStringList &propertyNames, const QDBusPlatformMenu *topLevelMenu);
    void populate(const QDBusPlatformMenu *menu, int depth, const QStringList &propertyNames);
    void populate(const QDBusPlatformMenuItem *item, int depth, const QStringList &propertyNames);

    int m_id;
    QVariantMap m_properties;
    QList<QDBusMenuLayoutItem> m_children;
};
Q_DECLARE_TYPEINFO(QDBusMenuLayoutItem, Q_RELOCATABLE_TYPE);

const QDBusArgument &operator<<(QDBusArgument &arg, const QDBusMenuLayoutItem &);
const QDBusArgument &operator>>(const QDBusArgument &arg, QDBusMenuLayoutItem &item);

typedef QList<QDBusMenuLayoutItem> QDBusMenuLayoutItemList;

class QDBusMenuEvent
{
public:
    int m_id;
    QString m_eventId;
    QDBusVariant m_data;
    uint m_timestamp;
};
Q_DECLARE_TYPEINFO(QDBusMenuEvent, Q_RELOCATABLE_TYPE); // QDBusVariant is movable, even though it cannot
                                                    // be marked as such until BobUI 6.

const QDBusArgument &operator<<(QDBusArgument &arg, const QDBusMenuEvent &ev);
const QDBusArgument &operator>>(const QDBusArgument &arg, QDBusMenuEvent &ev);

typedef QList<QDBusMenuEvent> QDBusMenuEventList;

#ifndef BOBUI_NO_DEBUG_STREAM
QDebug operator<<(QDebug d, const QDBusMenuItem &item);
QDebug operator<<(QDebug d, const QDBusMenuLayoutItem &item);
#endif

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(QDBusMenuItem, Q_GUI_EXPORT)
BOBUI_DECL_METATYPE_EXTERN(QDBusMenuItemList, Q_GUI_EXPORT)
BOBUI_DECL_METATYPE_EXTERN(QDBusMenuItemKeys, Q_GUI_EXPORT)
BOBUI_DECL_METATYPE_EXTERN(QDBusMenuItemKeysList, Q_GUI_EXPORT)
BOBUI_DECL_METATYPE_EXTERN(QDBusMenuLayoutItem, Q_GUI_EXPORT)
BOBUI_DECL_METATYPE_EXTERN(QDBusMenuLayoutItemList, Q_GUI_EXPORT)
BOBUI_DECL_METATYPE_EXTERN(QDBusMenuEvent, Q_GUI_EXPORT)
BOBUI_DECL_METATYPE_EXTERN(QDBusMenuEventList, Q_GUI_EXPORT)
BOBUI_DECL_METATYPE_EXTERN(QDBusMenuShortcut, Q_GUI_EXPORT)

#endif
