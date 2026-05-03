// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QFBBACKINGSTORE_P_H
#define QFBBACKINGSTORE_P_H

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

#include <qpa/qplatformbackingstore.h>
#include <BobUICore/QMutex>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QFbScreen;
class QFbWindow;
class QWindow;

class QFbBackingStore : public QPlatformBackingStore
{
public:
    QFbBackingStore(QWindow *window);
    ~QFbBackingStore();

    QPaintDevice *paintDevice() override { return &mImage; }
    void flush(QWindow *window, const QRegion &region, const QPoint &offset) override;

    void resize(const QSize &size, const QRegion &region) override;

    const QImage image();
    QImage toImage() const override;

    void lock();
    void unlock();

    void beginPaint(const QRegion &) override;
    void endPaint() override;

protected:
    friend class QFbWindow;

    QImage mImage;
    QMutex mImageMutex;
};

BOBUI_END_NAMESPACE

#endif // QFBBACKINGSTORE_P_H

