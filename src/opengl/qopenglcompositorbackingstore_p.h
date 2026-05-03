// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QOPENGLCOMPOSITORBACKINGSTORE_H
#define QOPENGLCOMPOSITORBACKINGSTORE_H

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

#include <BobUIOpenGL/bobuiopenglglobal.h>

#include <qpa/qplatformbackingstore.h>
#include <QImage>
#include <QRegion>
#include <private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QOpenGLContext;
class QPlatformTextureList;
class QRhiTexture;

class Q_OPENGL_EXPORT QOpenGLCompositorBackingStore : public QPlatformBackingStore
{
public:
    QOpenGLCompositorBackingStore(QWindow *window);
    ~QOpenGLCompositorBackingStore();

    QPaintDevice *paintDevice() override;

    void beginPaint(const QRegion &region) override;

    void flush(QWindow *window, const QRegion &region, const QPoint &offset) override;
    void resize(const QSize &size, const QRegion &staticContents) override;

    QImage toImage() const override;

    FlushResult rhiFlush(QWindow *window,
                         qreal sourceDevicePixelRatio,
                         const QRegion &region,
                         const QPoint &offset,
                         QPlatformTextureList *textures,
                         bool translucentBackground,
                         qreal sourceTransformFactor = 0) override;

    const QPlatformTextureList *textures() const { return m_textures; }

    void notifyComposited();

private:
    void updateTexture();

    QWindow *m_window;
    QImage m_image;
    QRegion m_dirty;
    uint m_bsTexture;
    QRhiTexture *m_bsTextureWrapper;
    QOpenGLContext *m_bsTextureContext;
    QPlatformTextureList *m_textures;
    QPlatformTextureList *m_lockedWidgetTextures;
    QRhi *m_rhi;
};

BOBUI_END_NAMESPACE

#endif // QOPENGLCOMPOSITORBACKINGSTORE_H
