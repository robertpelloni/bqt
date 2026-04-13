// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDBUFFER_H
#define QWAYLANDBUFFER_H

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

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

#include <BobUICore/QSize>
#include <BobUICore/QRect>

#include <BobUIWaylandClient/private/wayland-wayland-client-protocol.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class Q_WAYLANDCLIENT_EXPORT QWaylandBuffer {
public:
    QWaylandBuffer();
    virtual ~QWaylandBuffer();
    void init(wl_buffer *buf);

    wl_buffer *buffer() {return mBuffer;}
    virtual QSize size() const = 0;
    virtual int scale() const { return 1; }

    void setBusy(bool busy) { mBusy = busy; }
    bool busy() const { return mBusy; }

    void setCommitted() { mCommitted = true; }
    bool committed() const { return mCommitted; }

    void setDeleteOnRelease(bool deleteOnRelease);

protected:
    struct wl_buffer *mBuffer = nullptr;

private:
    bool mBusy = false;
    bool mCommitted = false;
    bool mDeleteOnRelease = false;

    static void release(void *data, wl_buffer *);
    static const wl_buffer_listener listener;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDBUFFER_H
