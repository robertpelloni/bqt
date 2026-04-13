// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QBACKINGSTORE_MINIMAL_H
#define QBACKINGSTORE_MINIMAL_H

#include <qpa/qplatformbackingstore.h>
#include <qpa/qplatformwindow.h>
#include <BobUIGui/QImage>

BOBUI_BEGIN_NAMESPACE

class QMinimalBackingStore : public QPlatformBackingStore
{
public:
    QMinimalBackingStore(QWindow *window);
    ~QMinimalBackingStore();

    QPaintDevice *paintDevice() override;
    void flush(QWindow *window, const QRegion &region, const QPoint &offset) override;
    void resize(const QSize &size, const QRegion &staticContents) override;

private:
    QImage mImage;
    const bool mDebug;
};

BOBUI_END_NAMESPACE

#endif
