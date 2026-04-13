// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandwindowmanagerintegration_p.h"
#include "qwaylandscreen_p.h"
#include "qwaylandwindow_p.h"
#include "qwaylanddisplay_p.h"
#include "qwaylandshellsurface_p.h"

#include <stdint.h>
#include <BobUICore/QEvent>
#include <BobUICore/QHash>
#include <BobUICore/QUrl>
#include <qpa/qplatformnativeinterface.h>
#include <qpa/qplatformwindow.h>
#include <BobUIGui/BobUIEvents>
#include <BobUIGui/QGuiApplication>

#include <QDebug>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

QWaylandWindowManagerIntegration::QWaylandWindowManagerIntegration(QWaylandDisplay *waylandDisplay,
                                                                   uint id, uint version)
    : BobUIWayland::bobui_windowmanager(waylandDisplay->object(), id, version)
{
}

QWaylandWindowManagerIntegration::~QWaylandWindowManagerIntegration()
{
    bobui_windowmanager_destroy(object());
}

bool QWaylandWindowManagerIntegration::showIsFullScreen() const
{
    return m_showIsFullScreen;
}

void QWaylandWindowManagerIntegration::windowmanager_hints(int32_t showIsFullScreen)
{
    m_showIsFullScreen = showIsFullScreen;
}

void QWaylandWindowManagerIntegration::windowmanager_quit()
{
    QGuiApplication::quit();
}

void QWaylandWindowManagerIntegration::openUrl(const QUrl &url)
{
    QString data = url.toString();
    static const int chunkSize = 128;
    while (!data.isEmpty()) {
        QString chunk = data.left(chunkSize);
        data = data.mid(chunkSize);
        if (chunk.at(chunk.size() - 1).isHighSurrogate() && !data.isEmpty()) {
            chunk.append(data.at(0));
            data = data.mid(1);
        }
        open_url(!data.isEmpty(), chunk);
    }
}
}

BOBUI_END_NAMESPACE
