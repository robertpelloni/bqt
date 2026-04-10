// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#ifndef Q_SPI_STRUCT_MARSHALLERS_H
#define Q_SPI_STRUCT_MARSHALLERS_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUICore/qlist.h>
#include <BobUIDBus/QDBusArgument>
#include <BobUIDBus/QDBusConnection>
#include <BobUIDBus/QDBusObjectPath>

BOBUI_REQUIRE_CONFIG(accessibility);

BOBUI_BEGIN_NAMESPACE

using QSpiIntList = QList<int>;
using QSpiUIntList = QList<uint>;

// FIXME: make this copy on write
struct QSpiObjectReference
{
    QString service;
    QDBusObjectPath path;

    QSpiObjectReference();
    QSpiObjectReference(const QDBusConnection& connection, const QDBusObjectPath& path)
        : service(connection.baseService()), path(path) {}
};
Q_DECLARE_TYPEINFO(QSpiObjectReference, Q_RELOCATABLE_TYPE); // QDBusObjectPath is movable, even though it
                                                         // cannot be marked that way until BobUI 6

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiObjectReference &address);
const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiObjectReference &address);

typedef QList<QSpiObjectReference> QSpiObjectReferenceArray;

struct QSpiAccessibleCacheItem
{
    QSpiObjectReference         path;
    QSpiObjectReference         application;
    QSpiObjectReference         parent;
    QSpiObjectReferenceArray children;
    QStringList                 supportedInterfaces;
    QString                     name;
    uint                        role;
    QString                     description;
    QSpiUIntList                state;
};
Q_DECLARE_TYPEINFO(QSpiAccessibleCacheItem, Q_RELOCATABLE_TYPE);

typedef QList<QSpiAccessibleCacheItem> QSpiAccessibleCacheArray;

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiAccessibleCacheItem &item);
const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiAccessibleCacheItem &item);

struct QSpiAction
{
    QString name;
    QString description;
    QString keyBinding;
};
Q_DECLARE_TYPEINFO(QSpiAction, Q_RELOCATABLE_TYPE);

typedef QList<QSpiAction> QSpiActionArray;

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiAction &action);
const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiAction &action);

struct QSpiEventListener
{
    QString listenerAddress;
    QString eventName;
};
Q_DECLARE_TYPEINFO(QSpiEventListener, Q_RELOCATABLE_TYPE);

typedef QList<QSpiEventListener> QSpiEventListenerArray;

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiEventListener &action);
const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiEventListener &action);

typedef std::pair<unsigned int, QSpiObjectReferenceArray> QSpiRelationArrayEntry;
typedef QList<QSpiRelationArrayEntry> QSpiRelationArray;

//a(iisv)
struct QSpiTextRange {
    int startOffset;
    int endOffset;
    QString contents;
    QVariant v;
};
Q_DECLARE_TYPEINFO(QSpiTextRange, Q_RELOCATABLE_TYPE);

typedef QList<QSpiTextRange> QSpiTextRangeList;
typedef QMap <QString, QString> QSpiAttributeSet;

struct QSpiDeviceEvent {
    unsigned int type;
    int id;
    int hardwareCode;
    int modifiers;
    int timestamp;
    QString text;
    bool isText;
};
Q_DECLARE_TYPEINFO(QSpiDeviceEvent, Q_RELOCATABLE_TYPE);

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiDeviceEvent &event);
const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiDeviceEvent &event);

struct QSpiMatchRule
{
    QSpiUIntList states;
    int stateMatchType;
    QSpiAttributeSet attributes;
    int attributeMatchType;
    QSpiIntList roles;
    int roleMatchType;
    QStringList interfaces;
    int interfaceMatchType;
    bool invert;
};
Q_DECLARE_TYPEINFO(QSpiMatchRule, Q_RELOCATABLE_TYPE);

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiMatchRule &matchRule);
const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiMatchRule &matchRule);

void qSpiInitializeStructTypes();

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(QSpiIntList, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiUIntList, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiObjectReference, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiObjectReferenceArray, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiAccessibleCacheItem, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiAccessibleCacheArray, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiAction, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiActionArray, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiEventListener, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiEventListenerArray, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiRelationArrayEntry, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiRelationArray, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiTextRange, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiTextRangeList, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiAttributeSet, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiDeviceEvent, /* not exported */)
BOBUI_DECL_METATYPE_EXTERN(QSpiMatchRule, /* not exported */)

// For qdbusxml2cpp-generated code
BOBUI_USE_NAMESPACE

#endif /* Q_SPI_STRUCT_MARSHALLERS_H */
