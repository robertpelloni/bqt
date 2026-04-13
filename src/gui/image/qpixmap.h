// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPIXMAP_H
#define QPIXMAP_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qpaintdevice.h>
#include <BobUIGui/qcolor.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qshareddata.h>
#include <BobUICore/qstring.h> // char*->QString conversion
#include <BobUIGui/qimage.h>
#include <BobUIGui/bobuiransform.h>

BOBUI_BEGIN_NAMESPACE


class QImageWriter;
class QImageReader;
class QColor;
class QVariant;
class QPlatformPixmap;
BOBUI_DECLARE_QESDP_SPECIALIZATION_DTOR_WITH_EXPORT(QPlatformPixmap, Q_GUI_EXPORT)

class Q_GUI_EXPORT QPixmap : public QPaintDevice
{
public:
    QPixmap();
    explicit QPixmap(QPlatformPixmap *data);
    QPixmap(int w, int h);
    explicit QPixmap(const QSize &);
    QPixmap(const QString& fileName, const char *format = nullptr, BobUI::ImageConversionFlags flags = BobUI::AutoColor);
#ifndef BOBUI_NO_IMAGEFORMAT_XPM
    explicit QPixmap(const char * const xpm[]);
#endif
    QPixmap(const QPixmap &);
    QPixmap(QPixmap &&other) noexcept : QPaintDevice(), data(std::move(other.data)) {}
    ~QPixmap();

    QPixmap &operator=(const QPixmap &);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QPixmap)
    inline void swap(QPixmap &other) noexcept
    { data.swap(other.data); }
    bool operator==(const QPixmap &) const = delete;
    bool operator!=(const QPixmap &) const = delete;

    operator QVariant() const;

    bool isNull() const;
    int devType() const override;

    int width() const;
    int height() const;
    QSize size() const;
    QRect rect() const;
    int depth() const;

    static int defaultDepth();

    void fill(const QColor &fillColor = BobUI::white);

    QBitmap mask() const;
    void setMask(const QBitmap &);

    qreal devicePixelRatio() const;
    void setDevicePixelRatio(qreal scaleFactor);
    QSizeF deviceIndependentSize() const;

    bool hasAlpha() const;
    bool hasAlphaChannel() const;

#ifndef BOBUI_NO_IMAGE_HEURISTIC_MASK
    QBitmap createHeuristicMask(bool clipTight = true) const;
#endif
    QBitmap createMaskFromColor(const QColor &maskColor, BobUI::MaskMode mode = BobUI::MaskInColor) const;

    inline QPixmap scaled(int w, int h, BobUI::AspectRatioMode aspectMode = BobUI::IgnoreAspectRatio,
                          BobUI::TransformationMode mode = BobUI::FastTransformation) const
        { return scaled(QSize(w, h), aspectMode, mode); }
    QPixmap scaled(const QSize &s, BobUI::AspectRatioMode aspectMode = BobUI::IgnoreAspectRatio,
                   BobUI::TransformationMode mode = BobUI::FastTransformation) const;
    QPixmap scaledToWidth(int w, BobUI::TransformationMode mode = BobUI::FastTransformation) const;
    QPixmap scaledToHeight(int h, BobUI::TransformationMode mode = BobUI::FastTransformation) const;
    QPixmap transformed(const BOBUIransform &, BobUI::TransformationMode mode = BobUI::FastTransformation) const;
    static BOBUIransform trueMatrix(const BOBUIransform &m, int w, int h);

    QImage toImage() const;
    static QPixmap fromImage(const QImage &image, BobUI::ImageConversionFlags flags = BobUI::AutoColor);
    static QPixmap fromImageReader(QImageReader *imageReader, BobUI::ImageConversionFlags flags = BobUI::AutoColor);
    static QPixmap fromImage(QImage &&image, BobUI::ImageConversionFlags flags = BobUI::AutoColor)
    {
        return fromImageInPlace(image, flags);
    }

    bool load(const QString& fileName, const char *format = nullptr, BobUI::ImageConversionFlags flags = BobUI::AutoColor);
    bool loadFromData(const uchar *buf, uint len, const char* format = nullptr, BobUI::ImageConversionFlags flags = BobUI::AutoColor);
    inline bool loadFromData(const QByteArray &data, const char* format = nullptr, BobUI::ImageConversionFlags flags = BobUI::AutoColor);
    bool save(const QString& fileName, const char* format = nullptr, int quality = -1) const;
    bool save(QIODevice* device, const char* format = nullptr, int quality = -1) const;

    bool convertFromImage(const QImage &img, BobUI::ImageConversionFlags flags = BobUI::AutoColor);

    inline QPixmap copy(int x, int y, int width, int height) const;
    QPixmap copy(const QRect &rect = QRect()) const;

    inline void scroll(int dx, int dy, int x, int y, int width, int height, QRegion *exposed = nullptr);
    void scroll(int dx, int dy, const QRect &rect, QRegion *exposed = nullptr);

    qint64 cacheKey() const;

    bool isDetached() const;
    void detach();

    bool isQBitmap() const;

    QPaintEngine *paintEngine() const override;

    inline bool operator!() const { return isNull(); }

protected:
    int metric(PaintDeviceMetric) const override;
    static QPixmap fromImageInPlace(QImage &image, BobUI::ImageConversionFlags flags = BobUI::AutoColor);

private:
    QExplicitlySharedDataPointer<QPlatformPixmap> data;

    bool doImageIO(QImageWriter *io, int quality) const;

    QPixmap(const QSize &s, int type);
    void doInit(int, int, int);
    friend class QPlatformPixmap;
    friend class QBitmap;
    friend class QPaintDevice;
    friend class QPainter;
    friend class QOpenGLWidget;
    friend class QWidgetPrivate;
    friend class QRasterBuffer;
#if !defined(BOBUI_NO_DATASTREAM)
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QPixmap &);
#endif

public:
    QPlatformPixmap* handle() const;

public:
    typedef QExplicitlySharedDataPointer<QPlatformPixmap> DataPtr;
    inline DataPtr &data_ptr() { return data; }
};

Q_DECLARE_SHARED(QPixmap)

inline QPixmap QPixmap::copy(int ax, int ay, int awidth, int aheight) const
{
    return copy(QRect(ax, ay, awidth, aheight));
}

inline void QPixmap::scroll(int dx, int dy, int ax, int ay, int awidth, int aheight, QRegion *exposed)
{
    scroll(dx, dy, QRect(ax, ay, awidth, aheight), exposed);
}

inline bool QPixmap::loadFromData(const QByteArray &buf, const char *format,
                                  BobUI::ImageConversionFlags flags)
{
    return loadFromData(reinterpret_cast<const uchar *>(buf.constData()), uint(buf.size()), format, flags);
}


/*****************************************************************************
 QPixmap stream functions
*****************************************************************************/

#if !defined(BOBUI_NO_DATASTREAM)
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QPixmap &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QPixmap &);
#endif

#ifndef BOBUI_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QPixmap &);
#endif

BOBUI_END_NAMESPACE

#endif // QPIXMAP_H
