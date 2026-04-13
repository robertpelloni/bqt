// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDPLATFORMSERVICES_H
#define QWAYLANDPLATFORMSERVICES_H

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

#include <BobUICore/QMap>
#include <BobUIGui/private/qdesktopunixservices_p.h>

#include <BobUIWaylandClient/private/qwayland-bobui-windowmanager.h>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandAppMenu;
class QWaylandDisplay;
class QWaylandWindow;

class Q_WAYLANDCLIENT_EXPORT QWaylandPlatformServices : public QDesktopUnixServices
{
public:
    explicit QWaylandPlatformServices(QWaylandDisplay *waylandDisplay);
    ~QWaylandPlatformServices();

    bool openUrl(const QUrl &url) override;
    bool openDocument(const QUrl &url) override;
    QString portalWindowIdentifier(QWindow *window) override;
    void registerDBusMenuForWindow(QWindow *window, const QString &service,
                                   const QString &path) override;
    void unregisterDBusMenuForWindow(QWindow *window) override;

private:
    QWaylandDisplay *m_display;
    QMap<QWindow *, QWaylandAppMenu *> m_appMenus;
};

BOBUI_END_NAMESPACE

} // namespace BobUIWaylandClient

#endif // QWAYLANDPLATFORMSERVICES_H
