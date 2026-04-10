// Copyright (C) 2011 - 2012 Research In Motion
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQNXGLCONTEXT_H
#define QQNXGLCONTEXT_H

#include <qpa/qplatformopenglcontext.h>
#include <BobUIGui/QSurfaceFormat>
#include <BobUICore/QAtomicInt>
#include <BobUICore/QSize>

#include <EGL/egl.h>
#include <BobUIGui/private/qeglplatformcontext_p.h>

BOBUI_BEGIN_NAMESPACE

class QQnxWindow;

class QQnxGLContext : public QEGLPlatformContext
{
public:
    QQnxGLContext(const QSurfaceFormat &format, QPlatformOpenGLContext *share);
    virtual ~QQnxGLContext();

    bool makeCurrent(QPlatformSurface *surface) override;
    void swapBuffers(QPlatformSurface *surface) override;
    void doneCurrent() override;

protected:
    EGLSurface eglSurfaceForPlatformSurface(QPlatformSurface *surface) override;
};

BOBUI_END_NAMESPACE

#endif // QQNXGLCONTEXT_H
