// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDSERVERBUFFERINTEGRATION_H
#define QWAYLANDSERVERBUFFERINTEGRATION_H

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
#include <BobUIGui/qopengl.h>

#include <BobUIWaylandClient/private/qwayland-server-buffer-extension.h>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QOpenGLTexture;

namespace BobUIWaylandClient {

class QWaylandDisplay;

class Q_WAYLANDCLIENT_EXPORT QWaylandServerBuffer
{
public:
    enum Format {
        RGBA32,
        A8,
        Custom
    };

    QWaylandServerBuffer();
    virtual ~QWaylandServerBuffer();

    virtual QOpenGLTexture *toOpenGlTexture() = 0;

    Format format() const;
    QSize size() const;

    void setUserData(void *userData);
    void *userData() const;

protected:
    Format m_format = RGBA32;
    QSize m_size;

private:
    void *m_user_data = nullptr;
};

class Q_WAYLANDCLIENT_EXPORT QWaylandServerBufferIntegration
{
public:
    QWaylandServerBufferIntegration();
    virtual ~QWaylandServerBufferIntegration();

    virtual void initialize(QWaylandDisplay *display) = 0;

    virtual QWaylandServerBuffer *serverBuffer(struct bobui_server_buffer *buffer) = 0;
};

}

BOBUI_END_NAMESPACE

#endif
