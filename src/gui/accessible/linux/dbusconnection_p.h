// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#ifndef DBUSCONNECTION_H
#define DBUSCONNECTION_H

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

#include <BobUICore/QObject>
#include <BobUICore/QString>
#include <BobUIDBus/QDBusConnection>
#include <BobUIDBus/QDBusVariant>
#include <BobUICore/private/qglobal_p.h>
Q_MOC_INCLUDE(<BobUIDBus/QDBusError>)

#include "bus_interface.h"
#include "properties_interface.h"

BOBUI_BEGIN_NAMESPACE

class QDBusServiceWatcher;

class QAtSpiDBusConnection : public QObject
{
    Q_OBJECT

public:
    QAtSpiDBusConnection(QObject *parent = nullptr);
    QDBusConnection connection() const;
    bool isEnabled() const { return m_enabled; }

Q_SIGNALS:
    // Emitted when the global accessibility status changes to enabled
    void enabledChanged(bool enabled);

private Q_SLOTS:
    QString getAddressFromXCB();
    void checkEnabledState();
    void serviceUnregistered();
    void connectA11yBus(const QString &address);

    void dbusError(const QDBusError &error);

private:
    QString getAccessibilityBusAddress() const;

    QDBusServiceWatcher *dbusWatcher;
    BobUIGuiPrivate::OrgFreedesktopDBusPropertiesInterface *m_dbusProperties;
    BobUIGuiPrivate::OrgA11yStatusInterface *m_a11yStatus;
    QDBusConnection m_a11yConnection;
    bool m_enabled;
};

BOBUI_END_NAMESPACE

#endif // DBUSCONNECTION_H
