// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qrasterbackingstore_p.h"

#include <BobUIGui/qbackingstore.h>
#include <BobUIGui/qpainter.h>

#include <private/qhighdpiscaling_p.h>
#include <qpa/qplatformwindow.h>

BOBUI_BEGIN_NAMESPACE

QRasterBackingStore::QRasterBackingStore(QWindow *window)
    : QPlatformBackingStore(window)
{
}

QRasterBackingStore::~QRasterBackingStore()
{
}

void QRasterBackingStore::resize(const QSize &size, const QRegion &staticContents)
{
    Q_UNUSED(staticContents);
    m_requestedSize = size;
}

QImage::Format QRasterBackingStore::format() const
{
    if (window()->format().hasAlpha())
        return QImage::Format_ARGB32_Premultiplied;
    else
        return QImage::Format_RGB32;
}

QPaintDevice *QRasterBackingStore::paintDevice()
{
    return &m_image;
}

QImage QRasterBackingStore::toImage() const
{
    return m_image;
}

bool QRasterBackingStore::scroll(const QRegion &region, int dx, int dy)
{
    if (window()->surfaceType() != QSurface::RasterSurface)
        return false;

    extern void bobui_scrollRectInImage(QImage &, const QRect &, const QPoint &);

    const qreal devicePixelRatio = m_image.devicePixelRatio();
    const QPoint delta(dx * devicePixelRatio, dy * devicePixelRatio);

    const QRect rect = region.boundingRect();
    bobui_scrollRectInImage(m_image, QRect(rect.topLeft() * devicePixelRatio, rect.size() * devicePixelRatio), delta);

    return true;
}

void QRasterBackingStore::beginPaint(const QRegion &region)
{
    qreal nativeWindowDevicePixelRatio = window()->handle()->devicePixelRatio();
    QSize effectiveBufferSize = m_requestedSize * nativeWindowDevicePixelRatio;
    if (m_image.devicePixelRatio() != nativeWindowDevicePixelRatio || m_image.size() != effectiveBufferSize) {
        m_image = QImage(effectiveBufferSize, format());
        m_image.setDevicePixelRatio(nativeWindowDevicePixelRatio);
        if (m_image.hasAlphaChannel())
            m_image.fill(BobUI::transparent);
    }

    if (!m_image.hasAlphaChannel())
        return;

    QPainter painter(&m_image);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    for (const QRect &rect : region)
        painter.fillRect(rect, BobUI::transparent);
}

BOBUI_END_NAMESPACE
