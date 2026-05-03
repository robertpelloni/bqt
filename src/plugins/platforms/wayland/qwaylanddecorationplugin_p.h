// Copyright (C) 2016 Robin Burchell <robin.burchell@viroteck.net>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDDECORATIONPLUGIN_H
#define QWAYLANDDECORATIONPLUGIN_H

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

class QWaylandAbstractDecoration;

#define QWaylandDecorationFactoryInterface_iid "org.bobui-project.BobUI.WaylandClient.QWaylandDecorationFactoryInterface.5.4"

class Q_WAYLANDCLIENT_EXPORT QWaylandDecorationPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QWaylandDecorationPlugin(QObject *parent = nullptr);
    ~QWaylandDecorationPlugin() override;

    virtual QWaylandAbstractDecoration *create(const QString &key, const QStringList &paramList) = 0;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDDECORATIONPLUGIN_H
