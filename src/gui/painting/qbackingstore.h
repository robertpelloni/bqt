// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBACKINGSTORE_H
#define QBACKINGSTORE_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qrect.h>

#include <BobUIGui/qwindow.h>
#include <BobUIGui/qregion.h>

BOBUI_BEGIN_NAMESPACE


class QRegion;
class QRect;
class QPoint;
class QImage;
class QBackingStorePrivate;
class QPlatformBackingStore;

class Q_GUI_EXPORT QBackingStore
{
public:
    explicit QBackingStore(QWindow *window);
    ~QBackingStore();

    QWindow *window() const;

    QPaintDevice *paintDevice();

    void flush(const QRegion &region, QWindow *window = nullptr, const QPoint &offset = QPoint());

    void resize(const QSize &size);
    QSize size() const;

    bool scroll(const QRegion &area, int dx, int dy);

    void beginPaint(const QRegion &);
    void endPaint();

    void setStaticContents(const QRegion &region);
    QRegion staticContents() const;
    bool hasStaticContents() const;

    QPlatformBackingStore *handle() const;

private:
    QScopedPointer<QBackingStorePrivate> d_ptr;
};

BOBUI_END_NAMESPACE

#endif // QBACKINGSTORE_H
