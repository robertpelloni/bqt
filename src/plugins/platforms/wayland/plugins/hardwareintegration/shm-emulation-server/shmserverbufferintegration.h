// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include "qwayland-shm-emulation-server-buffer.h"
#include <BobUIWaylandClient/private/qwaylandserverbufferintegration_p.h>

#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>
#include <BobUICore/BOBUIextStream>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class ShmServerBufferIntegration;

class ShmServerBuffer : public QWaylandServerBuffer
{
public:
    ShmServerBuffer(const QString &key, const QSize &size, int bytesPerLine, QWaylandServerBuffer::Format format);
    ~ShmServerBuffer() override;
    QOpenGLTexture* toOpenGlTexture() override;
private:
    QOpenGLTexture *m_texture = nullptr;
    QString m_key;
    int m_bpl;
};

class ShmServerBufferIntegration
    : public QWaylandServerBufferIntegration
    , public BobUIWayland::bobui_shm_emulation_server_buffer
{
public:
    void initialize(QWaylandDisplay *display) override;

    QWaylandServerBuffer *serverBuffer(struct bobui_server_buffer *buffer) override;

protected:
    void shm_emulation_server_buffer_server_buffer_created(bobui_server_buffer *id, const QString &key, int32_t width, int32_t height, int32_t bytes_per_line, int32_t format) override;

private:
    static void wlDisplayHandleGlobal(void *data, struct ::wl_registry *registry, uint32_t id,
                                      const QString &interface, uint32_t version);
    QWaylandDisplay *m_display = nullptr;
};

}

BOBUI_END_NAMESPACE
