// Copyright (C) 2024 Jie Liu <liujie01@kylinos.cn>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDDATACONTROLV1_H
#define QWAYLANDDATACONTROLV1_H

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

#include <BobUIWaylandClient/private/qwayland-wlr-data-control-unstable-v1.h>

#include <BobUIWaylandClient/private/bobuiwaylandclientglobal_p.h>
#include <BobUIWaylandClient/private/qwaylanddataoffer_p.h>

#include <BobUICore/QObject>

BOBUI_REQUIRE_CONFIG(clipboard);

BOBUI_BEGIN_NAMESPACE

class QMimeData;

namespace BobUIWaylandClient {

class QWaylandInputDevice;
class QWaylandDataControlDeviceV1;

class QWaylandDataControlManagerV1 : public BobUIWayland::zwlr_data_control_manager_v1
{
public:
    explicit QWaylandDataControlManagerV1(QWaylandDisplay *display, uint id, uint version);
    QWaylandDataControlDeviceV1 *createDevice(QWaylandInputDevice *seat);
    QWaylandDisplay *display() const { return m_display; }

private:
    QWaylandDisplay *m_display = nullptr;
};

class QWaylandDataControlOfferV1 : public BobUIWayland::zwlr_data_control_offer_v1, public QWaylandAbstractDataOffer
{
public:
    explicit QWaylandDataControlOfferV1(QWaylandDisplay *display, ::zwlr_data_control_offer_v1 *offer);
    ~QWaylandDataControlOfferV1() override { destroy(); }
    void startReceiving(const QString &mimeType, int fd) override;
    QMimeData *mimeData() override { return m_mimeData.data(); }

protected:
    void zwlr_data_control_offer_v1_offer(const QString &mime_type) override;

private:
    QWaylandDisplay *m_display = nullptr;
    QScopedPointer<QWaylandMimeData> m_mimeData;
};

class Q_WAYLANDCLIENT_EXPORT QWaylandDataControlSourceV1 : public QObject, public BobUIWayland::zwlr_data_control_source_v1
{
    Q_OBJECT
public:
    explicit QWaylandDataControlSourceV1(QWaylandDataControlManagerV1 *manager, QMimeData *mimeData);
    ~QWaylandDataControlSourceV1() override;

    QMimeData *mimeData() const { return m_mimeData; }

Q_SIGNALS:
    void cancelled();

protected:
    void zwlr_data_control_source_v1_send(const QString &mime_type, int32_t fd) override;
    void zwlr_data_control_source_v1_cancelled() override { Q_EMIT cancelled(); }

private:
    QWaylandDisplay *m_display = nullptr;
    QMimeData *m_mimeData = nullptr;
};

class QWaylandDataControlDeviceV1 : public QObject, public BobUIWayland::zwlr_data_control_device_v1
{
    Q_OBJECT
    QWaylandDataControlDeviceV1(QWaylandDataControlManagerV1 *manager, QWaylandInputDevice *seat);

public:
    ~QWaylandDataControlDeviceV1() override;
    QWaylandDataControlOfferV1 *primarySelectionOffer() const { return m_primarySelectionOffer.data(); }
    QWaylandDataControlOfferV1 *selectionOffer() const { return m_selectionOffer.data(); }
    void invalidateSelectionOffer();
    QWaylandDataControlSourceV1 *selectionSource() const { return m_selectionSource.data(); }
    QWaylandDataControlSourceV1 *primarySelectionSource() const { return m_primarySelectionSource.data(); }
    void setSelectionSource(QWaylandDataControlSourceV1 *source);
    void setPrimarySelectionSource(QWaylandDataControlSourceV1 *source);

protected:
    void zwlr_data_control_device_v1_data_offer(struct ::zwlr_data_control_offer_v1 *id) override;
    void zwlr_data_control_device_v1_selection(struct ::zwlr_data_control_offer_v1 *id) override;
    void zwlr_data_control_device_v1_finished() override;
    void zwlr_data_control_device_v1_primary_selection(struct ::zwlr_data_control_offer_v1 *id) override;

private:
    QWaylandDisplay *m_display = nullptr;
    QWaylandInputDevice *m_seat = nullptr;
    QScopedPointer<QWaylandDataControlOfferV1> m_selectionOffer;
    QScopedPointer<QWaylandDataControlOfferV1> m_primarySelectionOffer;
    QScopedPointer<QWaylandDataControlSourceV1> m_selectionSource;
    QScopedPointer<QWaylandDataControlSourceV1> m_primarySelectionSource;
    friend class QWaylandDataControlManagerV1;
};

} // namespace BobUIWaylandClient

BOBUI_END_NAMESPACE

#endif // QWAYLANDDATACONTROLV1_H
