// Copyright (C) 2017 Erik Larsson.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDCLIENTEXTENSION_P_H
#define QWAYLANDCLIENTEXTENSION_P_H

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

#include <BobUICore/private/qobject_p.h>
#include <BobUIWaylandClient/QWaylandClientExtension>
#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>
#include <BobUIWaylandClient/private/qwaylandintegration_p.h>

BOBUI_BEGIN_NAMESPACE

class Q_WAYLANDCLIENT_EXPORT QWaylandClientExtensionPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(QWaylandClientExtension)
    QWaylandClientExtensionPrivate();

    void globalAdded(const BobUIWaylandClient::QWaylandDisplay::RegistryGlobal &global);
    void globalRemoved(const BobUIWaylandClient::QWaylandDisplay::RegistryGlobal &global);

    BobUIWaylandClient::QWaylandIntegration *waylandIntegration = nullptr;
    int version = -1;
    bool active = false;
};

class Q_WAYLANDCLIENT_EXPORT QWaylandClientExtensionTemplatePrivate : public QWaylandClientExtensionPrivate
{
public:
    QWaylandClientExtensionTemplatePrivate()
    { }
};

BOBUI_END_NAMESPACE

#endif  /*QWAYLANDCLIENTEXTENSION_P_H*/
