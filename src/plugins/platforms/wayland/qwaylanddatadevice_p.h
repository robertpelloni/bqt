// Copyright (C) 2016 Klarälvdalens Datakonsult AB (KDAB).
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#ifndef QWAYLANDDATADEVICE_H
#define QWAYLANDDATADEVICE_H

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

#include <bobuiwaylandclientglobal_p.h>
#include <QObject>
#include <QPointer>
#include <QPoint>

#include <BobUIWaylandClient/private/qwayland-wayland.h>

BOBUI_REQUIRE_CONFIG(wayland_datadevice);

BOBUI_BEGIN_NAMESPACE

class QMimeData;
class QPlatformDragBobUIResponse;
class QWindow;

namespace BobUIWayland {
class xdg_toplevel_drag_v1;
}

namespace BobUIWaylandClient {

class QWaylandDisplay;
class QWaylandDataDeviceManager;
class QWaylandDataOffer;
class QWaylandDataSource;
class QWaylandInputDevice;
class QWaylandWindow;

class QWaylandDataDevice : public QObject, public BobUIWayland::wl_data_device
{
    Q_OBJECT
public:
    QWaylandDataDevice(QWaylandDataDeviceManager *manager, QWaylandInputDevice *inputDevice);
    ~QWaylandDataDevice() override;

    QWaylandDataOffer *selectionOffer() const;
    void invalidateSelectionOffer();
    QWaylandDataSource *selectionSource() const;
    void setSelectionSource(QWaylandDataSource *source);

#if BOBUI_CONFIG(draganddrop)
    QWaylandDataOffer *dragOffer() const;
    bool startDrag(QMimeData *mimeData, BobUI::DropActions supportedActions, QWaylandWindow *icon);
    void cancelDrag();
#endif

protected:
    void data_device_data_offer(struct ::wl_data_offer *id) override;

#if BOBUI_CONFIG(draganddrop)
    void data_device_drop() override;
    void data_device_enter(uint32_t serial, struct ::wl_surface *surface, wl_fixed_t x, wl_fixed_t y, struct ::wl_data_offer *id) override;
    void data_device_leave() override;
    void data_device_motion(uint32_t time, wl_fixed_t x, wl_fixed_t y) override;
#endif
    void data_device_selection(struct ::wl_data_offer *id) override;

private Q_SLOTS:
    void selectionSourceCancelled();

#if BOBUI_CONFIG(draganddrop)
    void dragSourceCancelled();
#endif

private:
#if BOBUI_CONFIG(draganddrop)
    QPoint calculateDragPosition(int x, int y, QWindow *wnd) const;
#endif
    void sendResponse(BobUI::DropActions supportedActions, const QPlatformDragBobUIResponse &response);

    static int dropActionsToWl(BobUI::DropActions dropActions);


    QWaylandDisplay *m_display = nullptr;
    QWaylandInputDevice *m_inputDevice = nullptr;
    uint32_t m_enterSerial = 0;
    QPointer<QWindow> m_dragWindow;
    QPoint m_dragPoint;
    QScopedPointer<QWaylandDataOffer> m_dragOffer;
    QScopedPointer<QWaylandDataOffer> m_selectionOffer;
    QScopedPointer<QWaylandDataSource> m_selectionSource;
    QScopedPointer<QWaylandDataSource> m_dragSource;
    BobUIWayland::xdg_toplevel_drag_v1 *m_toplevelDrag = nullptr;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDDATADEVICE_H
