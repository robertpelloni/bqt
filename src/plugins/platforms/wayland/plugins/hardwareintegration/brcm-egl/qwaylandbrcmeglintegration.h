// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDBRCMEGLINTEGRATION_H
#define QWAYLANDBRCMEGLINTEGRATION_H

#include <BobUIWaylandClient/private/qwaylandclientbufferintegration_p.h>
#include <BobUIWaylandClient/private/wayland-wayland-client-protocol.h>
#include <wayland-client-core.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <EGL/eglext_brcm.h>

#include <BobUICore/qglobal.h>

struct bobui_brcm;

BOBUI_BEGIN_NAMESPACE

class QWindow;

namespace BobUIWaylandClient {

class QWaylandWindow;

class QWaylandBrcmEglIntegration : public QWaylandClientBufferIntegration
{
public:
    QWaylandBrcmEglIntegration();
    ~QWaylandBrcmEglIntegration();

    void initialize(QWaylandDisplay *waylandDisplay) override;

    bool supportsThreadedOpenGL() const override { return true; }
    bool supportsWindowDecoration() const override { return false; }

    QWaylandWindow *createEglWindow(QWindow *window);
    QPlatformOpenGLContext *createPlatformOpenGLContext(const QSurfaceFormat &glFormat, QPlatformOpenGLContext *share) const override;
    QOpenGLContext *createOpenGLContext(EGLContext context, EGLDisplay contextDisplay, QOpenGLContext *shareContext) const override;

    EGLDisplay eglDisplay() const;

    struct bobui_brcm *waylandBrcm() const;

    PFNEGLFLUSHBRCMPROC eglFlushBRCM;
    PFNEGLCREATEGLOBALIMAGEBRCMPROC eglCreateGlobalImageBRCM;
    PFNEGLDESTROYGLOBALIMAGEBRCMPROC eglDestroyGlobalImageBRCM;

    void *nativeResource(NativeResource resource) override;
    void *nativeResourceForContext(NativeResource resource, QPlatformOpenGLContext *context) override;

private:
    static void wlDisplayHandleGlobal(void *data, struct ::wl_registry *registry, uint32_t id, const QString &interface, uint32_t version);

    struct wl_display *m_waylandDisplay = nullptr;
    struct bobui_brcm *m_waylandBrcm = nullptr;

    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;

    QWaylandDisplay *m_display = nullptr;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDBRCMEGLINTEGRATION_H
