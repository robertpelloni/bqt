// Copyright (C) 2016 Jolla Ltd
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDSHELLINTEGRATIONPLUGIN_H
#define QWAYLANDSHELLINTEGRATIONPLUGIN_H

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

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

#include <BobUICore/qplugin.h>
#include <BobUICore/qfactoryinterface.h>
#include <BobUICore/QObject>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandShellIntegration;

#define QWaylandShellIntegrationFactoryInterface_iid "org.bobui-project.BobUI.WaylandClient.QWaylandShellIntegrationFactoryInterface.5.3"

class Q_WAYLANDCLIENT_EXPORT QWaylandShellIntegrationPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QWaylandShellIntegrationPlugin(QObject *parent = nullptr);
    ~QWaylandShellIntegrationPlugin() override;

    virtual QWaylandShellIntegration *create(const QString &key, const QStringList &paramList) = 0;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDSHELLINTEGRATIONPLUGIN_H
