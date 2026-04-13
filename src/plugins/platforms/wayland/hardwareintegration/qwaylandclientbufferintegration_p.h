// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDCLIENTBUFFERINTEGRATION_H
#define QWAYLANDCLIENTBUFFERINTEGRATION_H

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

#include <BobUICore/private/qglobal_p.h>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#if BOBUI_CONFIG(opengl)
#include <BobUIGui/private/qeglplatformcontext_p.h>
#endif

BOBUI_BEGIN_NAMESPACE

class QWindow;
#if BOBUI_CONFIG(opengl)
class QOpenGLContext;
class QPlatformOpenGLContext;
#endif
class QPlatformOffscreenSurface;
class QSurfaceFormat;
class QOffscreenSurface;

namespace BobUIWaylandClient {

class QWaylandWindow;
class QWaylandDisplay;

class Q_WAYLANDCLIENT_EXPORT QWaylandClientBufferIntegration
{
public:
    QWaylandClientBufferIntegration();
    virtual ~QWaylandClientBufferIntegration();

    virtual void initialize(QWaylandDisplay *display) = 0;

    virtual bool isValid() const { return true; }

    virtual bool supportsThreadedOpenGL() const { return false; }
    virtual bool supportsWindowDecoration() const { return false; }

    virtual QWaylandWindow *createEglWindow(QWindow *window) = 0;
    virtual QPlatformOpenGLContext *createPlatformOpenGLContext(const QSurfaceFormat &glFormat, QPlatformOpenGLContext *share) const = 0;
    virtual bool canCreatePlatformOffscreenSurface() const { return false; }
#if BOBUI_CONFIG(opengl)
    virtual QOpenGLContext *createOpenGLContext(EGLContext context, EGLDisplay contextDisplay, QOpenGLContext *shareContext) const = 0;
    virtual QPlatformOffscreenSurface *createPlatformOffscreenSurface(QOffscreenSurface *surface) const { Q_UNUSED(surface); return nullptr; }
#endif

    enum NativeResource {
        EglDisplay,
        EglConfig,
        EglContext
    };
    virtual void *nativeResource(NativeResource /*resource*/) { return nullptr; }
    virtual void *nativeResourceForContext(NativeResource /*resource*/, QPlatformOpenGLContext */*context*/) { return nullptr; }
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDCLIENTBUFFERINTEGRATION_H
