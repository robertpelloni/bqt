// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBITMAP_H
#define QBITMAP_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qpixmap.h>

BOBUI_BEGIN_NAMESPACE


class QVariant;

class Q_GUI_EXPORT QBitmap : public QPixmap
{
public:
    QBitmap();
#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_X_6_0("Use fromPixmap instead.") explicit QBitmap(const QPixmap &);
#endif
    QBitmap(int w, int h);
    explicit QBitmap(const QSize &);
    explicit QBitmap(const QString &fileName, const char *format = nullptr);
    ~QBitmap() override;

#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_X_6_0("Use fromPixmap instead.") QBitmap &operator=(const QPixmap &);
#endif
    inline void swap(QBitmap &other) { QPixmap::swap(other); } // prevent QBitmap<->QPixmap swaps
    operator QVariant() const;

    inline void clear() { fill(BobUI::color0); }

    static QBitmap fromImage(const QImage &image, BobUI::ImageConversionFlags flags = BobUI::AutoColor);
    static QBitmap fromImage(QImage &&image, BobUI::ImageConversionFlags flags = BobUI::AutoColor);
    static QBitmap fromData(const QSize &size, const uchar *bits,
                            QImage::Format monoFormat = QImage::Format_MonoLSB);
    static QBitmap fromPixmap(const QPixmap &pixmap);

    QBitmap transformed(const BOBUIransform &matrix) const;

    typedef QExplicitlySharedDataPointer<QPlatformPixmap> DataPtr;
};
Q_DECLARE_SHARED(QBitmap)

BOBUI_END_NAMESPACE

#endif // QBITMAP_H
