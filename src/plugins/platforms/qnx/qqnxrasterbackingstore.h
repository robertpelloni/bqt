// Copyright (C) 2011 - 2012 Research In Motion
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQNXRASTERWINDOWSURFACE_H
#define QQNXRASTERWINDOWSURFACE_H

#include <qpa/qplatformbackingstore.h>

#include <screen/screen.h>

BOBUI_BEGIN_NAMESPACE

class QQnxRasterWindow;

class QQnxRasterBackingStore : public QPlatformBackingStore
{
public:
    QQnxRasterBackingStore(QWindow *window);
    ~QQnxRasterBackingStore();

    QPaintDevice *paintDevice() override;
    void flush(QWindow *window, const QRegion &region, const QPoint &offset) override;
    void resize(const QSize &size, const QRegion &staticContents) override;
    bool scroll(const QRegion &area, int dx, int dy) override;
    void beginPaint(const QRegion &region) override;
    void endPaint() override;

private:
    QQnxRasterWindow *platformWindow() const;

    QWindow *m_window;
    bool m_needsPosting;
    bool m_scrolled;
};

BOBUI_END_NAMESPACE

#endif // QQNXRASTERWINDOWSURFACE_H
