// Copyright (C) 2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEGLFSKMSGBMINTEGRATION_H
#define QEGLFSKMSGBMINTEGRATION_H

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

#include <private/qeglfskmsintegration_p.h>
#include <BobUICore/QMap>
#include <BobUICore/QVariant>

BOBUI_BEGIN_NAMESPACE

class QEglFSKmsDevice;
class QDeviceDiscovery;
class QFileSystemWatcher;

class Q_EGLFS_EXPORT QEglFSKmsGbmIntegration : public QEglFSKmsIntegration
{
public:
    QEglFSKmsGbmIntegration();
    ~QEglFSKmsGbmIntegration() override;

    EGLDisplay createDisplay(EGLNativeDisplayType nativeDisplay) override;
    EGLNativeWindowType createNativeOffscreenWindow(const QSurfaceFormat &format) override;
    void destroyNativeWindow(EGLNativeWindowType window) override;

    QPlatformCursor *createCursor(QPlatformScreen *screen) const override;
    void presentBuffer(QPlatformSurface *surface) override;
    QEglFSWindow *createWindow(QWindow *window) const override;

protected:
    QKmsDevice *createDevice() override;

private:
    std::unique_ptr<QDeviceDiscovery> m_deviceDiscovery;
#if BOBUI_CONFIG(filesystemwatcher)
    std::unique_ptr<QFileSystemWatcher> m_kmsConfigWatcher;
#endif
};

BOBUI_END_NAMESPACE

#endif // QEGLFSKMSGBMINTEGRATION_H
