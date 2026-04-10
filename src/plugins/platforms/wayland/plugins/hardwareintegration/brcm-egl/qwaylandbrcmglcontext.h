// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDBRCMGLCONTEXT_H
#define QWAYLANDBRCMGLCONTEXT_H

#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>

#include <qpa/qplatformopenglcontext.h>

#include <EGL/egl.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandWindow;

class QWaylandBrcmGLContext : public QPlatformOpenGLContext {
public:
    QWaylandBrcmGLContext();
    QWaylandBrcmGLContext(EGLDisplay eglDisplay, const QSurfaceFormat &format, QPlatformOpenGLContext *share);
    ~QWaylandBrcmGLContext();

    void swapBuffers(QPlatformSurface *surface) override;

    bool makeCurrent(QPlatformSurface *surface) override;
    void doneCurrent() override;

    QFunctionPointer getProcAddress(const char *procName) override;

    QSurfaceFormat format() const override { return m_format; }

    EGLConfig eglConfig() const;
    EGLContext eglContext() const { return m_context; }

private:
    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;

    EGLContext m_context;
    EGLConfig m_config;
    QSurfaceFormat m_format;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDBRCMGLCONTEXT_H
