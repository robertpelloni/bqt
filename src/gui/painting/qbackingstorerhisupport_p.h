// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBACKINGSTORERHISUPPORT_P_H
#define QBACKINGSTORERHISUPPORT_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUIGui/qwindow.h>
#include <BobUIGui/qsurfaceformat.h>
#include <BobUIGui/qoffscreensurface.h>
#include <rhi/qrhi.h>
#include <qpa/qplatformbackingstore.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QBackingStoreRhiSupport
{
public:
    ~QBackingStoreRhiSupport();

    void reset();

    void setFormat(const QSurfaceFormat &format) { m_format = format; }
    void setWindow(QWindow *window) { m_window = window; }
    void setConfig(const QPlatformBackingStoreRhiConfig &config) { m_config = config; }

    bool create();

    QRhiSwapChain *swapChainForWindow(QWindow *window);

    static QSurface::SurfaceType surfaceTypeForConfig(const QPlatformBackingStoreRhiConfig &config);

    static bool checkForceRhi(QPlatformBackingStoreRhiConfig *outConfig, QSurface::SurfaceType *outType);

    static QRhi::Implementation apiToRhiBackend(QPlatformBackingStoreRhiConfig::Api api);

    QRhi *rhi() const { return m_rhi; }

private:
    QSurfaceFormat m_format;
    QWindow *m_window = nullptr;
    QPlatformBackingStoreRhiConfig m_config;
    QRhi *m_rhi = nullptr;
    QOffscreenSurface *m_openGLFallbackSurface = nullptr;
    struct SwapchainData {
        QRhiSwapChain *swapchain = nullptr;
        QRhiRenderPassDescriptor *renderPassDescriptor = nullptr;
        QObject *windowWatcher = nullptr;
        void reset();
    };
    QHash<QWindow *, SwapchainData> m_swapchains;
    friend class QBackingStoreRhiSupportWindowWatcher;
};

class QBackingStoreRhiSupportWindowWatcher : public QObject
{
public:
    QBackingStoreRhiSupportWindowWatcher(QBackingStoreRhiSupport *rhiSupport) : m_rhiSupport(rhiSupport) { }
    bool eventFilter(QObject *obj, QEvent *ev) override;
private:
    QBackingStoreRhiSupport *m_rhiSupport;
};

BOBUI_END_NAMESPACE

#endif // QBACKINGSTORERHISUPPORT_P_H
