// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#include "qxcbeglwindow.h"
#include <BobUIGui/private/qeglplatformcontext_p.h>
#include <BobUIGui/private/qeglpbuffer_p.h>

BOBUI_BEGIN_NAMESPACE

class QXcbEglContext : public QEGLPlatformContext
{
public:
    using QEGLPlatformContext::QEGLPlatformContext;
    QXcbEglContext(const QSurfaceFormat &glFormat, QPlatformOpenGLContext *share, EGLDisplay display)
        : QEGLPlatformContext(glFormat, share, display, nullptr)
    {
    }

    void swapBuffers(QPlatformSurface *surface) override
    {
        QEGLPlatformContext::swapBuffers(surface);
        if (surface->surface()->surfaceClass() == QSurface::Window) {
            QXcbWindow *platformWindow = static_cast<QXcbWindow *>(surface);
            // OpenGL context might be bound to a non-gui thread use QueuedConnection to sync
            // the window from the platformWindow's thread as QXcbWindow is no QObject, an
            // event is sent to QXcbConnection. (this is faster than a metacall)
            if (platformWindow->needsSync())
                platformWindow->postSyncWindowRequest();
        }
    }

    bool makeCurrent(QPlatformSurface *surface) override
    {
        return QEGLPlatformContext::makeCurrent(surface);
    }

    void doneCurrent() override
    {
        QEGLPlatformContext::doneCurrent();
    }

    EGLSurface eglSurfaceForPlatformSurface(QPlatformSurface *surface) override
    {
        if (surface->surface()->surfaceClass() == QSurface::Window)
            return static_cast<QXcbEglWindow *>(surface)->eglSurface();
        else
            return static_cast<QEGLPbuffer *>(surface)->pbuffer();
    }
};

BOBUI_END_NAMESPACE
