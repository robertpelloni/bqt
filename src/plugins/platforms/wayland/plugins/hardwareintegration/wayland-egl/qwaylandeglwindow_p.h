// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <BobUIWaylandClient/private/qwaylandwindow_p.h>
#include "qwaylandeglinclude_p.h"
#include "qwaylandeglclientbufferintegration_p.h"

BOBUI_BEGIN_NAMESPACE

class QOpenGLFramebufferObject;

namespace BobUIWaylandClient {

class QWaylandGLContext;

class Q_WAYLANDCLIENT_EXPORT QWaylandEglWindow : public QWaylandWindow
{
    Q_OBJECT
public:
    QWaylandEglWindow(QWindow *window, QWaylandDisplay *display);
    ~QWaylandEglWindow();
    WindowType windowType() const override;
    void ensureSize() override;

    void updateSurface(bool create);
    QRect contentsRect() const;

    EGLSurface eglSurface() const;
    GLuint contentFBO() const;
    GLuint contentTexture() const;
    bool needToUpdateContentFBO() const { return decoration() && (m_resize || !m_contentFBO); }

    void bindContentFBO();

    void invalidateSurface() override;

    QMutex* eglSurfaceLock();

private:
    QWaylandEglClientBufferIntegration *m_clientBufferIntegration = nullptr;
    struct wl_egl_window *m_waylandEglWindow = nullptr;

    // Locks any manipulation of the eglSurface size
    QMutex m_eglSurfaceLock;
    EGLSurface m_eglSurface = EGL_NO_SURFACE;
    mutable bool m_resize = false;
    mutable QOpenGLFramebufferObject *m_contentFBO = nullptr;

    // Size used in the last call to wl_egl_window_resize
    QSize m_requestedSize;

    // Size of the buffer used by QWaylandWindow
    // This is always written to from the main thread, potentially read from the rendering thread
    QReadWriteLock m_bufferSizeLock;
    QSize m_bufferSize;
};

}

BOBUI_END_NAMESPACE
