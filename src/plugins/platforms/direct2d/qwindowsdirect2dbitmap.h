// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSDIRECT2DBITMAP_H
#define QWINDOWSDIRECT2DBITMAP_H

#include <BobUICore/qnamespace.h>
#include <BobUICore/qrect.h>
#include <BobUICore/qscopedpointer.h>

struct ID2D1DeviceContext;
struct ID2D1Bitmap1;

BOBUI_BEGIN_NAMESPACE

class QWindowsDirect2DDeviceContext;
class QWindowsDirect2DBitmapPrivate;

class QImage;
class QSize;
class QColor;

class QWindowsDirect2DBitmap
{
    Q_DECLARE_PRIVATE(QWindowsDirect2DBitmap)
    Q_DISABLE_COPY_MOVE(QWindowsDirect2DBitmap)
public:
    QWindowsDirect2DBitmap();
    QWindowsDirect2DBitmap(ID2D1Bitmap1 *bitmap, ID2D1DeviceContext *dc);
    ~QWindowsDirect2DBitmap();

    bool resize(int width, int height);
    bool fromImage(const QImage &image, BobUI::ImageConversionFlags flags);

    ID2D1Bitmap1* bitmap() const;
    QWindowsDirect2DDeviceContext* deviceContext() const;

    void fill(const QColor &color);
    QImage toImage(const QRect &rect = QRect());

    QSize size() const;

private:
    QScopedPointer<QWindowsDirect2DBitmapPrivate> d_ptr;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSDIRECT2DBITMAP_H
