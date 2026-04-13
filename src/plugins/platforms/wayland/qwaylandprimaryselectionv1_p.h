// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDPRIMARYSELECTIONV1_P_H
#define QWAYLANDPRIMARYSELECTIONV1_P_H

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

#include <BobUIWaylandClient/private/qwayland-wp-primary-selection-unstable-v1.h>

#include <BobUIWaylandClient/private/bobuiwaylandclientglobal_p.h>
#include <BobUIWaylandClient/private/qwaylanddataoffer_p.h>

#include <BobUICore/QObject>

BOBUI_REQUIRE_CONFIG(wayland_client_primary_selection);

BOBUI_BEGIN_NAMESPACE

class QMimeData;

namespace BobUIWaylandClient {

class QWaylandInputDevice;
class QWaylandPrimarySelectionDeviceV1;

class QWaylandPrimarySelectionDeviceManagerV1 : public BobUIWayland::zwp_primary_selection_device_manager_v1
{
public:
    explicit QWaylandPrimarySelectionDeviceManagerV1(QWaylandDisplay *display, uint id, uint version);
    ~QWaylandPrimarySelectionDeviceManagerV1();
    QWaylandPrimarySelectionDeviceV1 *createDevice(QWaylandInputDevice *seat);
    QWaylandDisplay *display() const { return m_display; }

private:
    QWaylandDisplay *m_display = nullptr;
};

class QWaylandPrimarySelectionOfferV1 : public BobUIWayland::zwp_primary_selection_offer_v1, public QWaylandAbstractDataOffer
{
public:
    explicit QWaylandPrimarySelectionOfferV1(QWaylandDisplay *display, ::zwp_primary_selection_offer_v1 *offer);
    ~QWaylandPrimarySelectionOfferV1() override { destroy(); }
    void startReceiving(const QString &mimeType, int fd) override;
    QMimeData *mimeData() override { return m_mimeData.data(); }

protected:
    void zwp_primary_selection_offer_v1_offer(const QString &mime_type) override;

private:
    QWaylandDisplay *m_display = nullptr;
    QScopedPointer<QWaylandMimeData> m_mimeData;
};

class Q_WAYLANDCLIENT_EXPORT QWaylandPrimarySelectionSourceV1 : public QObject, public BobUIWayland::zwp_primary_selection_source_v1
{
    Q_OBJECT
public:
    explicit QWaylandPrimarySelectionSourceV1(QWaylandPrimarySelectionDeviceManagerV1 *manager, QMimeData *mimeData);
    ~QWaylandPrimarySelectionSourceV1() override;

    QMimeData *mimeData() const { return m_mimeData; }

Q_SIGNALS:
    void cancelled();

protected:
    void zwp_primary_selection_source_v1_send(const QString &mime_type, int32_t fd) override;
    void zwp_primary_selection_source_v1_cancelled() override { emit cancelled(); }

private:
    QMimeData *m_mimeData = nullptr;
};

class QWaylandPrimarySelectionDeviceV1 : public QObject, public BobUIWayland::zwp_primary_selection_device_v1
{
    Q_OBJECT
    QWaylandPrimarySelectionDeviceV1(QWaylandPrimarySelectionDeviceManagerV1 *manager, QWaylandInputDevice *seat);

public:
    ~QWaylandPrimarySelectionDeviceV1() override;
    QWaylandPrimarySelectionOfferV1 *selectionOffer() const { return m_selectionOffer.data(); }
    void invalidateSelectionOffer();
    QWaylandPrimarySelectionSourceV1 *selectionSource() const { return m_selectionSource.data(); }
    void setSelectionSource(QWaylandPrimarySelectionSourceV1 *source);

protected:
    void zwp_primary_selection_device_v1_data_offer(struct ::zwp_primary_selection_offer_v1 *offer) override;
    void zwp_primary_selection_device_v1_selection(struct ::zwp_primary_selection_offer_v1 *id) override;

private:
    QWaylandDisplay *m_display = nullptr;
    QWaylandInputDevice *m_seat = nullptr;
    QScopedPointer<QWaylandPrimarySelectionOfferV1> m_selectionOffer;
    QScopedPointer<QWaylandPrimarySelectionSourceV1> m_selectionSource;
    friend class QWaylandPrimarySelectionDeviceManagerV1;
};

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE

#endif // QWAYLANDPRIMARYSELECTIONV1_P_H
