// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "qwaylandeglinclude_p.h" //must be first

#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>
#include <BobUIGui/private/qeglplatformcontext_p.h>
#include <qpa/qplatformopenglcontext.h>

BOBUI_BEGIN_NAMESPACE

class QOpenGLShaderProgram;
class QOpenGLTextureCache;

namespace BobUIWaylandClient {

class QWaylandEglWindow;
class DecorationsBlitter;

class Q_WAYLANDCLIENT_EXPORT QWaylandGLContext : public QEGLPlatformContext
{
public:
    QWaylandGLContext();
    QWaylandGLContext(EGLDisplay eglDisplay, QWaylandDisplay *display, const QSurfaceFormat &format, QPlatformOpenGLContext *share);
    ~QWaylandGLContext();

    void initialize() override;
    void swapBuffers(QPlatformSurface *surface) override;

    bool makeCurrent(QPlatformSurface *surface) override;
    void doneCurrent() override;

    void beginFrame() override;
    void endFrame() override;

    GLuint defaultFramebufferObject(QPlatformSurface *surface) const override;

protected:
    EGLSurface eglSurfaceForPlatformSurface(QPlatformSurface *surface) override;
    EGLSurface createTemporaryOffscreenSurface() override;
    void destroyTemporaryOffscreenSurface(EGLSurface surface) override;
    void runGLChecks() override;

private:
    QWaylandDisplay *m_display = nullptr;
    EGLContext m_decorationsContext = EGL_NO_CONTEXT;
    DecorationsBlitter *m_blitter = nullptr;
    bool m_supportNonBlockingSwap = true;
    EGLenum m_api;
    wl_surface *m_wlSurface = nullptr;
    wl_egl_window *m_eglWindow = nullptr;
    QWaylandEglWindow *m_currentWindow = nullptr;
    QMetaObject::Connection m_reconnectionWatcher;
    bool m_doneCurrentWorkAround = false;
};

}

BOBUI_END_NAMESPACE
