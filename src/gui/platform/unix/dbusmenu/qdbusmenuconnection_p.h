// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDBUSMENUCONNECTION_H
#define QDBUSMENUCONNECTION_H

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

#include <BobUICore/QString>
#include <BobUIDBus/QDBusConnection>
#include <BobUIDBus/QDBusVariant>

#include <BobUIGui/bobuigui-config.h>
#include <BobUICore/private/qglobal_p.h>
Q_MOC_INCLUDE(<BobUIDBus/QDBusError>)

BOBUI_BEGIN_NAMESPACE

class QDBusServiceWatcher;
#ifndef BOBUI_NO_SYSTEMTRAYICON
class QDBusTrayIcon;
#endif // BOBUI_NO_SYSTEMTRAYICON

class QDBusMenuConnection : public QObject
{
    Q_OBJECT

public:
    QDBusMenuConnection(QObject *parent = nullptr, const QString &serviceName = QString());
    ~QDBusMenuConnection();
    QDBusConnection connection() const { return m_connection; }
    QDBusServiceWatcher *dbusWatcher() const { return m_dbusWatcher; }
    bool isWatcherRegistered() const { return m_watcherRegistered; }
#ifndef BOBUI_NO_SYSTEMTRAYICON
    bool registerTrayIconMenu(QDBusTrayIcon *item);
    void unregisterTrayIconMenu(QDBusTrayIcon *item);
    bool registerTrayIcon(QDBusTrayIcon *item);
    bool registerTrayIconWithWatcher(QDBusTrayIcon *item);
    void unregisterTrayIcon(QDBusTrayIcon *item);
#endif // BOBUI_NO_SYSTEMTRAYICON

Q_SIGNALS:
#ifndef BOBUI_NO_SYSTEMTRAYICON
    void trayIconRegistered();
#endif // BOBUI_NO_SYSTEMTRAYICON

private Q_SLOTS:
    void dbusError(const QDBusError &error);

private:
    QString m_serviceName;
    QDBusConnection m_connection;
    QDBusServiceWatcher *m_dbusWatcher;
    bool m_watcherRegistered;
};

BOBUI_END_NAMESPACE

#endif // QDBUSMENUCONNECTION_H
