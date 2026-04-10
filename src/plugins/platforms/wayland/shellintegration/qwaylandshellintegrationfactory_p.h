// Copyright (C) 2016 Jolla Ltd
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDSHELLINTEGRATIONFACTORY_H
#define QWAYLANDSHELLINTEGRATIONFACTORY_H

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

#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

#include <BobUICore/QStringList>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandShellIntegration;

class Q_WAYLANDCLIENT_EXPORT QWaylandShellIntegrationFactory
{
public:
    static QStringList keys();
    static QWaylandShellIntegration *create(const QString &name, QWaylandDisplay *display, const QStringList &args = QStringList());
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDSHELLINTEGRATIONFACTORY_H
