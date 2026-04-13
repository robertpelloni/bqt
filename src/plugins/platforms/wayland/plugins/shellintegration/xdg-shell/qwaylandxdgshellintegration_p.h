// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "qwayland-xdg-shell.h"

#include <BobUIWaylandClient/private/qwaylandshellintegration_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandXdgShell;

class Q_WAYLANDCLIENT_EXPORT QWaylandXdgShellIntegration
    : public QWaylandShellIntegrationTemplate<QWaylandXdgShellIntegration>,
      public BobUIWayland::xdg_wm_base
{
public:
    QWaylandXdgShellIntegration();
    ~QWaylandXdgShellIntegration() override;
    QWaylandShellSurface *createShellSurface(QWaylandWindow *window) override;
    void *nativeResourceForWindow(const QByteArray &resource, QWindow *window) override;
    bool initialize(QWaylandDisplay *display) override;

protected:
    void xdg_wm_base_ping(uint32_t serial) override;

private:
    QWaylandDisplay *mDisplay;
    QScopedPointer<QWaylandXdgShell> mXdgShell;
};

}

BOBUI_END_NAMESPACE
