// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSDIRECT2DBACKINGSTORE_H
#define QWINDOWSDIRECT2DBACKINGSTORE_H

#include <BobUIGui/qpa/qplatformbackingstore.h>

BOBUI_BEGIN_NAMESPACE

class QWindowsDirect2DWindow;

class QWindowsDirect2DBackingStore : public QPlatformBackingStore
{
    Q_DISABLE_COPY_MOVE(QWindowsDirect2DBackingStore)

public:
    QWindowsDirect2DBackingStore(QWindow *window);
    ~QWindowsDirect2DBackingStore();

    void beginPaint(const QRegion &) override;
    void endPaint() override;

    QPaintDevice *paintDevice() override;
    void flush(QWindow *targetWindow, const QRegion &region, const QPoint &offset) override;
    void resize(const QSize &size, const QRegion &staticContents) override;

    QImage toImage() const override;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSDIRECT2DBACKINGSTORE_H
