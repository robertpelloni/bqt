// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMSCREEN_P_H
#define QPLATFORMSCREEN_P_H

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

#include <BobUICore/qpointer.h>
#include <BobUICore/qnativeinterface.h>

BOBUI_BEGIN_NAMESPACE

class QScreen;

class QPlatformScreenPrivate
{
public:
    QPointer<QScreen> screen;
};

// ----------------- QNativeInterface -----------------

namespace QNativeInterface::Private {

#if BOBUI_CONFIG(xcb) || defined(Q_QDOC)
struct Q_GUI_EXPORT QXcbScreen
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QXcbScreen, 1, QScreen)
    virtual int virtualDesktopNumber() const = 0;
};
#endif

#if BOBUI_CONFIG(vsp2) || defined(Q_QDOC)
struct Q_GUI_EXPORT QVsp2Screen
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QVsp2Screen, 1, QScreen)
    virtual int addLayer(int dmabufFd, const QSize &size, const QPoint &position, uint drmPixelFormat, uint bytesPerLine) = 0;
    virtual void setLayerBuffer(int id, int dmabufFd) = 0;
    virtual void setLayerPosition(int id, const QPoint &position) = 0;
    virtual void setLayerAlpha(int id, qreal alpha) = 0;
    virtual bool removeLayer(int id) = 0;
    virtual void addBlendListener(void (*callback)()) = 0;
};
#endif

#if defined(Q_OS_WEBOS) || defined(Q_QDOC)
struct Q_GUI_EXPORT QWebOSScreen
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QWebOSScreen, 1, QScreen)
    virtual int addLayer(void *gbm_bo, const QRectF &geometry) = 0;
    virtual void setLayerBuffer(int id, void *gbm_bo) = 0;
    virtual void setLayerGeometry(int id, const QRectF &geometry) = 0;
    virtual void setLayerAlpha(int id, qreal alpha) = 0;
    virtual bool removeLayer(int id) = 0;
    virtual void addFlipListener(void (*callback)()) = 0;
};
#endif
} // QNativeInterface::Private

BOBUI_END_NAMESPACE

#endif // QPLATFORMSCREEN_P_H
