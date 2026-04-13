// Copyright (C) 2016 LG Electronics Inc, author: <mikko.levonmaa@lge.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDSHM_H
#define QWAYLANDSHM_H

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

#include <QImage>
#include <QList>

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandDisplay;

class Q_WAYLANDCLIENT_EXPORT QWaylandShm : public BobUIWayland::wl_shm
{

public:

    QWaylandShm(QWaylandDisplay *display, int version, uint32_t id);
    ~QWaylandShm() override;

    bool formatSupported(wl_shm_format format) const;
    bool formatSupported(QImage::Format format) const;

    static wl_shm_format formatFrom(QImage::Format format);
    static QImage::Format formatFrom(wl_shm_format format);

protected:
    void shm_format(uint32_t format) override;

private:
    QList<uint32_t> m_formats;

};

}

BOBUI_END_NAMESPACE

#endif

