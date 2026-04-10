// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEGLFSCONTEXT_H
#define QEGLFSCONTEXT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qeglfsglobal_p.h"
#include "qeglfscursor_p.h"
#include <BobUIGui/private/qeglplatformcontext_p.h>
#include <BobUICore/QVariant>

BOBUI_BEGIN_NAMESPACE

class Q_EGLFS_EXPORT QEglFSContext : public QEGLPlatformContext
{
public:
    using QEGLPlatformContext::QEGLPlatformContext;
    QEglFSContext() = default; // workaround for INTEGRITY compiler
    QEglFSContext(const QSurfaceFormat &format, QPlatformOpenGLContext *share, EGLDisplay display,
                  EGLConfig *config);
    EGLSurface eglSurfaceForPlatformSurface(QPlatformSurface *surface) override;
    EGLSurface createTemporaryOffscreenSurface() override;
    void destroyTemporaryOffscreenSurface(EGLSurface surface) override;
    void runGLChecks() override;
    void swapBuffers(QPlatformSurface *surface) override;

    QEglFSCursorData cursorData;

private:
    EGLNativeWindowType m_tempWindow = 0;
};

BOBUI_END_NAMESPACE

#endif // QEGLFSCONTEXT_H
