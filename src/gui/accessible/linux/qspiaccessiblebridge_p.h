// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#ifndef QSPIACCESSIBLEBRIDGE_H
#define QSPIACCESSIBLEBRIDGE_H

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
#include <BobUIDBus/qdbusconnection.h>
#include <qpa/qplatformaccessibility.h>
#include <BobUICore/qhash.h>

namespace BobUIGuiPrivate {
class DeviceEventControllerAdaptor;
} // namespace BobUIGuiPrivate

BOBUI_REQUIRE_CONFIG(accessibility);

BOBUI_BEGIN_NAMESPACE

class QAtSpiDBusConnection;
class QSpiDBusCache;
class AtSpiAdaptor;
struct RoleNames;

class Q_GUI_EXPORT QSpiAccessibleBridge: public QObject, public QPlatformAccessibility
{
    Q_OBJECT
public:
    using SpiRoleMapping = QHash <QAccessible::Role, RoleNames>;

    QSpiAccessibleBridge();

    virtual ~QSpiAccessibleBridge();

    void notifyAccessibilityUpdate(QAccessibleEvent *event) override;
    QDBusConnection dBusConnection() const;

    const SpiRoleMapping &spiRoleNames() const { return m_spiRoleMapping; }

    static QSpiAccessibleBridge *instance();
    static RoleNames namesForRole(QAccessible::Role role);

public Q_SLOTS:
    void enabledChanged(bool enabled);

private:
    void initializeConstantMappings();
    void updateStatus();

    QSpiDBusCache *cache;
    BobUIGuiPrivate::DeviceEventControllerAdaptor *dec;
    AtSpiAdaptor *dbusAdaptor;
    QAtSpiDBusConnection* dbusConnection;
    SpiRoleMapping m_spiRoleMapping;
};

BOBUI_END_NAMESPACE

#endif
