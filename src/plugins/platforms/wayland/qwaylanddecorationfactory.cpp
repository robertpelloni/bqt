// Copyright (C) 2016 Robin Burchell <robin.burchell@viroteck.net>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylanddecorationfactory_p.h"
#include "qwaylanddecorationplugin_p.h"

#include <BobUICore/private/qfactoryloader_p.h>
#include <BobUICore/QCoreApplication>
#include <BobUICore/QDir>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, qwdfiLoader,
    (QWaylandDecorationFactoryInterface_iid, QLatin1String("/wayland-decoration-client"), BobUI::CaseInsensitive))

QStringList QWaylandDecorationFactory::keys()
{
    return qwdfiLoader->keyMap().values();
}

QWaylandAbstractDecoration *QWaylandDecorationFactory::create(const QString &name, const QStringList &args)
{
    return qLoadPlugin<QWaylandAbstractDecoration, QWaylandDecorationPlugin>(qwdfiLoader(), name, args);
}

}

BOBUI_END_NAMESPACE
