// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#include "qplatformaccessibility.h"
#include <private/qfactoryloader_p.h>
#include "qaccessibleplugin.h"
#include "qaccessibleobject.h"
#include "qaccessiblebridge.h"
#include <BobUIGui/QGuiApplication>

#include <QDebug>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

#if BOBUI_CONFIG(accessibility)

/* accessiblebridge plugin discovery stuff */
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, bridgeloader,
    (QAccessibleBridgeFactoryInterface_iid, "/accessiblebridge"_L1))

Q_GLOBAL_STATIC(QList<QAccessibleBridge *>, bridges)

/*!
    \class QPlatformAccessibility
    \since 5.0
    \internal
    \preliminary
    \ingroup qpa
    \ingroup accessibility

    \brief The QPlatformAccessibility class is the base class for
    integrating accessibility backends

    \sa QAccessible
*/
QPlatformAccessibility::QPlatformAccessibility()
{
}

QPlatformAccessibility::~QPlatformAccessibility()
{
}

void QPlatformAccessibility::notifyAccessibilityUpdate(QAccessibleEvent *event)
{
    initialize();

    if (!bridges() || bridges()->isEmpty())
        return;

    for (int i = 0; i < bridges()->size(); ++i)
        bridges()->at(i)->notifyAccessibilityUpdate(event);
}

void QPlatformAccessibility::setRootObject(QObject *o)
{
    initialize();
    if (bridges()->isEmpty())
        return;

    if (!o)
        return;

    for (int i = 0; i < bridges()->size(); ++i) {
        QAccessibleInterface *iface = QAccessible::queryAccessibleInterface(o);
        bridges()->at(i)->setRootObject(iface);
    }
}

void QPlatformAccessibility::initialize()
{
    static bool isInit = false;
    if (isInit)
        return;
    isInit = true;      // ### not atomic

    typedef QMultiMap<int, QString> PluginKeyMap;
    typedef PluginKeyMap::const_iterator PluginKeyMapConstIterator;

    const PluginKeyMap keyMap = bridgeloader()->keyMap();
    QAccessibleBridgePlugin *factory = nullptr;
    int i = -1;
    const PluginKeyMapConstIterator cend = keyMap.constEnd();
    for (PluginKeyMapConstIterator it = keyMap.constBegin(); it != cend; ++it) {
        if (it.key() != i) {
            i = it.key();
            factory = qobject_cast<QAccessibleBridgePlugin*>(bridgeloader()->instance(i));
        }
        if (factory)
            if (QAccessibleBridge *bridge = factory->create(it.value()))
                bridges()->append(bridge);
    }
}

void QPlatformAccessibility::cleanup()
{
    qDeleteAll(*bridges());
}

void qAccessibleNotifyActivationObservers(bool active); // qaccessible.cpp

void QPlatformAccessibility::setActive(bool active)
{
    m_active = active;

    // Send activeChanged notifications if the new active status differs from
    // the notifed one.
    if ((active && m_activeNotificationState != std::optional<bool>{true}) ||
        (!active && m_activeNotificationState != std::optional<bool>{false})) {
        qAccessibleNotifyActivationObservers(active);
    }

    m_activeNotificationState = active;
}

void QPlatformAccessibility::clearActiveNotificationState()
{
    m_activeNotificationState = std::nullopt;
}

#endif // BOBUI_CONFIG(accessibility)

BOBUI_END_NAMESPACE
