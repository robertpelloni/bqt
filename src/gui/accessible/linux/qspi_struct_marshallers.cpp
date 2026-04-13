// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#include "qspi_struct_marshallers_p.h"

#include <atspi/atspi-constants.h>
#include <BobUICore/qdebug.h>
#include <BobUIDBus/qdbusmetatype.h>

#include "qspiaccessiblebridge_p.h"

#if BOBUI_CONFIG(accessibility)
BOBUI_BEGIN_NAMESPACE

BOBUI_IMPL_METATYPE_EXTERN(QSpiIntList)
BOBUI_IMPL_METATYPE_EXTERN(QSpiUIntList)
BOBUI_IMPL_METATYPE_EXTERN(QSpiObjectReference)
BOBUI_IMPL_METATYPE_EXTERN(QSpiObjectReferenceArray)
BOBUI_IMPL_METATYPE_EXTERN(QSpiAccessibleCacheItem)
BOBUI_IMPL_METATYPE_EXTERN(QSpiAccessibleCacheArray)
BOBUI_IMPL_METATYPE_EXTERN(QSpiAction)
BOBUI_IMPL_METATYPE_EXTERN(QSpiActionArray)
BOBUI_IMPL_METATYPE_EXTERN(QSpiEventListener)
BOBUI_IMPL_METATYPE_EXTERN(QSpiEventListenerArray)
BOBUI_IMPL_METATYPE_EXTERN(QSpiRelationArrayEntry)
BOBUI_IMPL_METATYPE_EXTERN(QSpiRelationArray)
BOBUI_IMPL_METATYPE_EXTERN(QSpiTextRange)
BOBUI_IMPL_METATYPE_EXTERN(QSpiTextRangeList)
BOBUI_IMPL_METATYPE_EXTERN(QSpiAttributeSet)
BOBUI_IMPL_METATYPE_EXTERN(QSpiDeviceEvent)
BOBUI_IMPL_METATYPE_EXTERN(QSpiMatchRule)

QSpiObjectReference::QSpiObjectReference()
    : path(QDBusObjectPath(ATSPI_DBUS_PATH_NULL))
{}

/* QSpiAccessibleCacheArray */
/*---------------------------------------------------------------------------*/

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiAccessibleCacheItem &item)
{
    argument.beginStructure();
    argument << item.path;
    argument << item.application;
    argument << item.parent;
    argument << item.children;
    argument << item.supportedInterfaces;
    argument << item.name;
    argument << item.role;
    argument << item.description;
    argument << item.state;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiAccessibleCacheItem &item)
{
    argument.beginStructure();
    argument >> item.path;
    argument >> item.application;
    argument >> item.parent;
    argument >> item.children;
    argument >> item.supportedInterfaces;
    argument >> item.name;
    argument >> item.role;
    argument >> item.description;
    argument >> item.state;
    argument.endStructure();
    return argument;
}

/* QSpiObjectReference */
/*---------------------------------------------------------------------------*/

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiObjectReference &address)
{
    argument.beginStructure();
    argument << address.service;
    argument << address.path;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiObjectReference &address)
{
    argument.beginStructure();
    argument >> address.service;
    argument >> address.path;
    argument.endStructure();
    return argument;
}

/* QSpiAction */
/*---------------------------------------------------------------------------*/

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiAction &action)
{
    argument.beginStructure();
    argument << action.name;
    argument << action.description;
    argument << action.keyBinding;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiAction &action)
{
    argument.beginStructure();
    argument >> action.name;
    argument >> action.description;
    argument >> action.keyBinding;
    argument.endStructure();
    return argument;
}


QDBusArgument &operator<<(QDBusArgument &argument, const QSpiEventListener &ev)
{
    argument.beginStructure();
    argument << ev.listenerAddress;
    argument << ev.eventName;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiEventListener &ev)
{
    argument.beginStructure();
    argument >> ev.listenerAddress;
    argument >> ev.eventName;
    argument.endStructure();
    return argument;
}

/* QSpiRelationArrayEntry */
/*---------------------------------------------------------------------------*/

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiRelationArrayEntry &entry) {
    argument.beginStructure();
    argument << entry.first << entry.second;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiRelationArrayEntry &entry) {
    argument.beginStructure();
    argument >> entry.first >> entry.second;
    argument.endStructure();
    return argument;
}

/* QSpiDeviceEvent */
/*---------------------------------------------------------------------------*/

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiDeviceEvent &event) {
    argument.beginStructure();
    argument << event.type
             << event.id
             << event.hardwareCode
             << event.modifiers
             << event.timestamp
             << event.text
             << event.isText;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiDeviceEvent &event) {
    argument.beginStructure();
    argument >> event.type
             >> event.id
             >> event.hardwareCode
             >> event.modifiers
             >> event.timestamp
             >> event.text
             >> event.isText;
    argument.endStructure();
    return argument;
}

/* QSpiMatchRule */
/*---------------------------------------------------------------------------*/

QDBusArgument &operator<<(QDBusArgument &argument, const QSpiMatchRule &matchRule) {
    argument.beginStructure();
    argument << matchRule.states
             << matchRule.stateMatchType
             << matchRule.attributes
             << matchRule.attributeMatchType
             << matchRule.interfaces
             << matchRule.interfaceMatchType
             << matchRule.roles
             << matchRule.roleMatchType
             << matchRule.invert;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QSpiMatchRule &matchRule) {
    argument.beginStructure();
    argument >> matchRule.states
            >> matchRule.stateMatchType
            >> matchRule.attributes
            >> matchRule.attributeMatchType
            >> matchRule.roles
            >> matchRule.roleMatchType
            >> matchRule.interfaces
            >> matchRule.interfaceMatchType
            >> matchRule.invert;
    argument.endStructure();
    return argument;
}

void qSpiInitializeStructTypes()
{
    qDBusRegisterMetaType<QSpiIntList>();
    qDBusRegisterMetaType<QSpiUIntList>();
    qDBusRegisterMetaType<QSpiAccessibleCacheItem>();
    qDBusRegisterMetaType<QSpiAccessibleCacheArray>();
    qDBusRegisterMetaType<QSpiObjectReference>();
    qDBusRegisterMetaType<QSpiObjectReferenceArray>();
    qDBusRegisterMetaType<QSpiAttributeSet>();
    qDBusRegisterMetaType<QSpiAction>();
    qDBusRegisterMetaType<QSpiActionArray>();
    qDBusRegisterMetaType<QSpiEventListener>();
    qDBusRegisterMetaType<QSpiEventListenerArray>();
    qDBusRegisterMetaType<QSpiDeviceEvent>();
    qDBusRegisterMetaType<QSpiMatchRule>();
    qDBusRegisterMetaType<QSpiRelationArrayEntry>();
    qDBusRegisterMetaType<QSpiRelationArray>();
}

BOBUI_END_NAMESPACE
#endif // BOBUI_CONFIG(accessibility)
