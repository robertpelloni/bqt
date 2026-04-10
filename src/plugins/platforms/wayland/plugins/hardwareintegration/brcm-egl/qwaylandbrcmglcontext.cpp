// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwaylandbrcmglcontext.h"

#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>
#include <BobUIWaylandClient/private/qwaylandwindow_p.h>
#include "qwaylandbrcmeglwindow.h"

#include <BobUIGui/private/qeglconvenience_p.h>

#include <qpa/qplatformopenglcontext.h>
#include <BobUIGui/QSurfaceFormat>

#include <dlfcn.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

extern QSurfaceFormat brcmFixFormat(const QSurfaceFormat &format);

QWaylandBrcmGLContext::QWaylandBrcmGLContext() = default;

QWaylandBrcmGLContext::QWaylandBrcmGLContext(EGLDisplay eglDisplay, const QSurfaceFormat &format, QPlatformOpenGLContext *share)
    : QPlatformOpenGLContext()
    , m_eglDisplay(eglDisplay)
    , m_config(q_configFromGLFormat(m_eglDisplay, brcmFixFormat(format), true))
    , m_format(q_glFormatFromConfig(m_eglDisplay, m_config))
{
    EGLContext shareEGLContext = share ? static_cast<QWaylandBrcmGLContext *>(share)->eglContext() : EGL_NO_CONTEXT;

    eglBindAPI(EGL_OPENGL_ES_API);

    QList<EGLint> eglContextAttrs;
    eglContextAttrs.append(EGL_CONTEXT_CLIENT_VERSION);
    eglContextAttrs.append(format.majorVersion() == 1 ? 1 : 2);
    eglContextAttrs.append(EGL_NONE);

    m_context = eglCreateContext(m_eglDisplay, m_config, shareEGLContext, eglContextAttrs.constData());
}

QWaylandBrcmGLContext::~QWaylandBrcmGLContext()
{
    eglDestroyContext(m_eglDisplay, m_context);
}

bool QWaylandBrcmGLContext::makeCurrent(QPlatformSurface *surface)
{
    return static_cast<QWaylandBrcmEglWindow *>(surface)->makeCurrent(m_context);
}

void QWaylandBrcmGLContext::doneCurrent()
{
    eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

void QWaylandBrcmGLContext::swapBuffers(QPlatformSurface *surface)
{
    static_cast<QWaylandBrcmEglWindow *>(surface)->swapBuffers();
}

QFunctionPointer QWaylandBrcmGLContext::getProcAddress(const char *procName)
{
    QFunctionPointer proc = (QFunctionPointer) eglGetProcAddress(procName);
    if (!proc)
        proc = (QFunctionPointer) dlsym(RTLD_DEFAULT, procName);
    return proc;
}

EGLConfig QWaylandBrcmGLContext::eglConfig() const
{
    return m_config;
}

}

BOBUI_END_NAMESPACE
