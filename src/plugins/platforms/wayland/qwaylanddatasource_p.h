// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDDATASOURCE_H
#define QWAYLANDDATASOURCE_H

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

#include <QObject>

#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include <BobUIWaylandClient/private/bobuiwaylandclientglobal_p.h>

BOBUI_REQUIRE_CONFIG(wayland_datadevice);

BOBUI_BEGIN_NAMESPACE

class QMimeData;

namespace BobUIWaylandClient {

class QWaylandDataDeviceManager;
class QWaylandDisplay;

class Q_WAYLANDCLIENT_EXPORT QWaylandDataSource : public QObject, public BobUIWayland::wl_data_source
{
    Q_OBJECT
public:
    QWaylandDataSource(QWaylandDataDeviceManager *dataDeviceManager, QMimeData *mimeData);
    ~QWaylandDataSource() override;

Q_SIGNALS:
    void cancelled();
    void finished();

    void dndResponseUpdated(bool accepted, BobUI::DropAction action);
    void dndDropped(bool accepted, BobUI::DropAction action);

protected:
    void data_source_cancelled() override;
    void data_source_send(const QString &mime_type, int32_t fd) override;
    void data_source_target(const QString &mime_type) override;
    void data_source_dnd_drop_performed() override;
    void data_source_dnd_finished() override;
    void data_source_action(uint32_t action) override;

private:
    QMimeData *m_mime_data = nullptr;
    bool m_accepted = false;
    BobUI::DropAction m_dropAction = BobUI::IgnoreAction;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDDATASOURCE_H
