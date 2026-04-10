// Copyright (C) 2014 BogDan Vatra <bogdan@kde.org>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDPLATFORMOPENGLCONTEXT_H
#define QANDROIDPLATFORMOPENGLCONTEXT_H

#include <BobUIGui/private/qeglplatformcontext_p.h>

BOBUI_BEGIN_NAMESPACE

class QAndroidPlatformOpenGLContext : public QEGLPlatformContext
{
public:
    using QEGLPlatformContext::QEGLPlatformContext;
    QAndroidPlatformOpenGLContext(const QSurfaceFormat &format, QPlatformOpenGLContext *share, EGLDisplay display);
    void swapBuffers(QPlatformSurface *surface) override;
    bool makeCurrent(QPlatformSurface *surface) override;

private:
    EGLSurface eglSurfaceForPlatformSurface(QPlatformSurface *surface) override;
};

BOBUI_END_NAMESPACE

#endif // QANDROIDPLATFORMOPENGLCONTEXT_H
