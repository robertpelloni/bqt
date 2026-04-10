// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qandroidplatformoffscreensurface.h"

#include <BobUIGui/private/qeglconvenience_p.h>

BOBUI_BEGIN_NAMESPACE

QAndroidPlatformOffscreenSurface::QAndroidPlatformOffscreenSurface(
        ANativeWindow *nativeSurface, EGLDisplay display, QOffscreenSurface *offscreenSurface)
    : QPlatformOffscreenSurface(offscreenSurface), m_display(display), m_surface(EGL_NO_SURFACE)
{
    // FIXME: Read surface format properties from native surface using ANativeWindow_getFormat
    m_format.setAlphaBufferSize(8);
    m_format.setRedBufferSize(8);
    m_format.setGreenBufferSize(8);
    m_format.setBlueBufferSize(8);

    if (EGLConfig config = q_configFromGLFormat(m_display, m_format, false)) {
        const EGLint attributes[] = { EGL_NONE };
        m_surface = eglCreateWindowSurface(m_display, config, nativeSurface, attributes);
    }
}

QAndroidPlatformOffscreenSurface::~QAndroidPlatformOffscreenSurface()
{
    eglDestroySurface(m_display, m_surface);
}

BOBUI_END_NAMESPACE

