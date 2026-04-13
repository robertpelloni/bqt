// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <BobUIWaylandClient/private/qwaylandclientbufferintegration_p.h>

#include "qwaylandeglinclude_p.h"

BOBUI_BEGIN_NAMESPACE

class QWindow;

namespace BobUIWaylandClient {

class QWaylandWindow;

class Q_WAYLANDCLIENT_EXPORT QWaylandEglClientBufferIntegration : public QWaylandClientBufferIntegration
{
public:
    QWaylandEglClientBufferIntegration();
    ~QWaylandEglClientBufferIntegration() override;

    void initialize(QWaylandDisplay *display) override;
    bool isValid() const override;
    bool supportsThreadedOpenGL() const override;
    bool supportsWindowDecoration() const override;

    QWaylandWindow *createEglWindow(QWindow *window) override;
    QPlatformOpenGLContext *createPlatformOpenGLContext(const QSurfaceFormat &glFormat, QPlatformOpenGLContext *share) const override;
    bool canCreatePlatformOffscreenSurface() const override;
    QPlatformOffscreenSurface *createPlatformOffscreenSurface(QOffscreenSurface *surface) const override;
    QOpenGLContext *createOpenGLContext(EGLContext context, EGLDisplay contextDisplay, QOpenGLContext *shareContext) const override;

    void *nativeResource(NativeResource resource) override;
    void *nativeResourceForContext(NativeResource resource, QPlatformOpenGLContext *context) override;

    EGLDisplay eglDisplay() const;

private:
    QWaylandDisplay *m_display = nullptr;

    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
    bool m_supportsThreading = false;
};

BOBUI_END_NAMESPACE

}
