// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAGLCONTEXT_H
#define QCOCOAGLCONTEXT_H

#include <BobUICore/QPointer>
#include <BobUICore/QVarLengthArray>
#include <BobUICore/private/qcore_mac_p.h>
#include <qpa/qplatformopenglcontext.h>
#include <BobUIGui/qopenglcontext.h>
#include <BobUIGui/private/qopenglcontext_p.h>
#include <BobUIGui/QWindow>

Q_FORWARD_DECLARE_OBJC_CLASS(NSOpenGLContext);
Q_FORWARD_DECLARE_OBJC_CLASS(NSOpenGLPixelFormat);

BOBUI_BEGIN_NAMESPACE

class QCocoaWindow;

class QCocoaGLContext : public QPlatformOpenGLContext, public QNativeInterface::QCocoaGLContext
{
public:
    QCocoaGLContext(QOpenGLContext *context);
    QCocoaGLContext(NSOpenGLContext *context);
    ~QCocoaGLContext();

    void initialize() override;

    bool makeCurrent(QPlatformSurface *surface) override;
    void beginFrame() override;
    void swapBuffers(QPlatformSurface *surface) override;
    void doneCurrent() override;

    void update();

    QSurfaceFormat format() const override;
    bool isSharing() const override;
    bool isValid() const override;

    NSOpenGLContext *nativeContext() const override;

    QFunctionPointer getProcAddress(const char *procName) override;

    bool isSoftwareContext() const;

private:
    static NSOpenGLPixelFormat *pixelFormatForSurfaceFormat(const QSurfaceFormat &format);

    bool setDrawable(QPlatformSurface *surface);
    void updateSurfaceFormat();

    NSOpenGLContext *m_context = nil;
    NSOpenGLContext *m_shareContext = nil;
    QSurfaceFormat m_format;
    QVarLengthArray<QMacNotificationObserver, 3> m_updateObservers;
    QAtomicInt m_needsUpdate = false;
    bool m_isSoftwareContext = false;

#ifndef BOBUI_NO_DEBUG_STREAM
    friend QDebug operator<<(QDebug debug, const QCocoaGLContext *screen);
#endif
};

BOBUI_END_NAMESPACE

#endif // QCOCOAGLCONTEXT_H
