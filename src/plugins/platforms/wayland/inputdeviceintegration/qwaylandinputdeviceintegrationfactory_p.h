// Copyright (C) 2016 LG Electronics Ltd
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDINPUTDEVICEINTEGRATIONFACTORY_H
#define QWAYLANDINPUTDEVICEINTEGRATIONFACTORY_H

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
#include <BobUICore/QStringList>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandInputDeviceIntegration;

class Q_WAYLANDCLIENT_EXPORT QWaylandInputDeviceIntegrationFactory
{
public:
    static QStringList keys();
    static QWaylandInputDeviceIntegration *create(const QString &name, const QStringList &args);
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDINPUTDEVICENTEGRATIONFACTORY_H
