// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDWLSHELLSURFACE_H
#define QWAYLANDWLSHELLSURFACE_H

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

#include <BobUICore/QSize>

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include <BobUIWaylandClient/private/qwaylandshellsurface_p.h>

BOBUI_BEGIN_NAMESPACE

class QWindow;

namespace BobUIWaylandClient {

class QWaylandWindow;
class QWaylandInputDevice;
class QWaylandExtendedSurface;

class Q_WAYLANDCLIENT_EXPORT QWaylandWlShellSurface : public QWaylandShellSurface
    , public BobUIWayland::wl_shell_surface
{
    Q_OBJECT
public:
    QWaylandWlShellSurface(struct ::wl_shell_surface *shell_surface, QWaylandWindow *window);
    ~QWaylandWlShellSurface() override;

    using BobUIWayland::wl_shell_surface::resize;
    bool resize(QWaylandInputDevice *inputDevice, BobUI::Edges edges) override;

    using BobUIWayland::wl_shell_surface::move;
    bool move(QWaylandInputDevice *inputDevice) override;

    void setTitle(const QString & title) override;
    void setAppId(const QString &appId) override;

    void applyConfigure() override;
    bool wantsDecorations() const override;

    std::any surfaceRole() const override { return object(); };

protected:
    void requestWindowStates(BobUI::WindowStates states) override;

private:
    static enum resize convertToResizeEdges(BobUI::Edges edges);
    void setTopLevel();
    void updateTransientParent(QWindow *parent);
    void setPopup(QWaylandWindow *parent, QWaylandInputDevice *device, uint serial);

    QWaylandWindow *m_window = nullptr;
    struct {
        BobUI::WindowStates states = BobUI::WindowNoState;
        QSize size;
        enum resize edges = resize_none;
    } m_applied, m_pending;
    QSize m_normalSize;
    // There's really no need to have pending and applied state on wl-shell, but we do it just to
    // keep the different shell implementations more similar.
    QWaylandExtendedSurface *m_extendedWindow = nullptr;

    void shell_surface_ping(uint32_t serial) override;
    void shell_surface_configure(uint32_t edges,
                                 int32_t width,
                                 int32_t height) override;
    void shell_surface_popup_done() override;

    friend class QWaylandWindow;
};

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE

#endif // QWAYLANDSHELLSURFACE_H
