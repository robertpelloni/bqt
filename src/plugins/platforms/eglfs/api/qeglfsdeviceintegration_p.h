// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEGLFSDEVICEINTEGRATION_H
#define QEGLFSDEVICEINTEGRATION_H

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
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformscreen.h>
#include <BobUICore/QString>
#include <BobUICore/QLoggingCategory>
#include <BobUIGui/QSurfaceFormat>
#include <BobUIGui/QImage>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcEglDevDebug)

class QPlatformSurface;
class QEglFSWindow;

#define QEglFSDeviceIntegrationFactoryInterface_iid "org.bobui-project.bobui.qpa.egl.QEglFSDeviceIntegrationFactoryInterface.5.5"

class Q_EGLFS_EXPORT QEglFSDeviceIntegration
{
public:
    virtual ~QEglFSDeviceIntegration() { }

    virtual void platformInit();
    virtual void platformDestroy();
    virtual EGLNativeDisplayType platformDisplay() const;
    virtual EGLDisplay createDisplay(EGLNativeDisplayType nativeDisplay);
    virtual bool usesDefaultScreen();
    virtual void screenInit();
    virtual void screenDestroy();
    virtual QSizeF physicalScreenSize() const;
    virtual QSize screenSize() const;
    virtual QDpi logicalDpi() const;
    virtual QDpi logicalBaseDpi() const;
    virtual BobUI::ScreenOrientation nativeOrientation() const;
    virtual BobUI::ScreenOrientation orientation() const;
    virtual int screenDepth() const;
    virtual QImage::Format screenFormat() const;
    virtual qreal refreshRate() const;
    virtual QSurfaceFormat surfaceFormatFor(const QSurfaceFormat &inputFormat) const;
    virtual EGLint surfaceType() const;
    virtual QEglFSWindow *createWindow(QWindow *window) const;
    virtual EGLNativeWindowType createNativeWindow(QPlatformWindow *platformWindow,
                                                   const QSize &size,
                                                   const QSurfaceFormat &format);
    virtual EGLNativeWindowType createNativeOffscreenWindow(const QSurfaceFormat &format);
    virtual void destroyNativeWindow(EGLNativeWindowType window);
    virtual bool hasCapability(QPlatformIntegration::Capability cap) const;
    virtual QPlatformCursor *createCursor(QPlatformScreen *screen) const;
    virtual bool filterConfig(EGLDisplay display, EGLConfig config) const;
    virtual void waitForVSync(QPlatformSurface *surface) const;
    virtual void presentBuffer(QPlatformSurface *surface);
    virtual QByteArray fbDeviceName() const;
    virtual int framebufferIndex() const;
    virtual bool supportsPBuffers() const;
    virtual bool supportsSurfacelessContexts() const;
    virtual QFunctionPointer platformFunction(const QByteArray &function) const;
    virtual void *nativeResourceForIntegration(const QByteArray &name);
    virtual void *nativeResourceForScreen(const QByteArray &resource, QScreen *screen);
    virtual void *wlDisplay() const;

    static EGLConfig chooseConfig(EGLDisplay display, const QSurfaceFormat &format);
};

class Q_EGLFS_EXPORT QEglFSDeviceIntegrationPlugin : public QObject
{
    Q_OBJECT

public:
    virtual QEglFSDeviceIntegration *create() = 0;

    // the pattern expected by qLoadPlugin calls for a QString argument.
    // we don't need it, so don't bother subclasses with it:
    QEglFSDeviceIntegration *create(const QString &) { return create(); }
};

class Q_EGLFS_EXPORT QEglFSDeviceIntegrationFactory
{
public:
    static QStringList keys();
    static QEglFSDeviceIntegration *create(const QString &name);
};

BOBUI_END_NAMESPACE

#endif // QEGLDEVICEINTEGRATION_H
