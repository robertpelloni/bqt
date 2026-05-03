// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>
#include <QBitmap>
#include <QDir>
#include <QImage>
#include <QImageReader>
#include <QPainter>
#include <QPixmap>
#include <private/qpixmap_raster_p.h>

class tst_QPixmap : public QObject
{
    Q_OBJECT

public:
    tst_QPixmap();

private slots:
    void fill_data();
    void fill();

    void scaled_data();
    void scaled();
    void transformed_data();
    void transformed();
    void mask_data();
    void mask();

    void fromImageReader_data();
    void fromImageReader();
};

Q_DECLARE_METATYPE(QImage::Format)
Q_DECLARE_METATYPE(BobUI::AspectRatioMode)
Q_DECLARE_METATYPE(BobUI::TransformationMode)

QPixmap rasterPixmap(int width, int height)
{
    QPlatformPixmap *data =
        new QRasterPlatformPixmap(QPlatformPixmap::PixmapType);

    data->resize(width, height);

    return QPixmap(data);
}

QPixmap rasterPixmap(const QSize &size)
{
    return rasterPixmap(size.width(), size.height());
}

QPixmap rasterPixmap(const QImage &image)
{
    QPlatformPixmap *data =
        new QRasterPlatformPixmap(QPlatformPixmap::PixmapType);

    data->fromImage(image, BobUI::AutoColor);

    return QPixmap(data);
}

tst_QPixmap::tst_QPixmap()
{
}

void tst_QPixmap::fill_data()
{
    BOBUIest::addColumn<bool>("opaque");
    BOBUIest::addColumn<int>("width");
    BOBUIest::addColumn<int>("height");

    BOBUIest::newRow("opaque 16x16") << true << 16 << 16;
    BOBUIest::newRow("!opaque 16x16") << false << 16 << 16;
    BOBUIest::newRow("opaque 587x128") << true << 587 << 128;
    BOBUIest::newRow("!opaque 587x128") << false << 587 << 128;
}

void tst_QPixmap::fill()
{
    QFETCH(bool, opaque);
    QFETCH(int, width);
    QFETCH(int, height);

    const QColor color = opaque ? QColor(255, 0, 0) : QColor(255, 0, 0, 200);
    QPixmap pixmap = rasterPixmap(width, height);

    QBENCHMARK {
        pixmap.fill(color);
    }
}

void tst_QPixmap::scaled_data()
{
    BOBUIest::addColumn<QSize>("size");
    BOBUIest::addColumn<QSize>("scale");
    BOBUIest::addColumn<BobUI::AspectRatioMode>("ratioMode");
    BOBUIest::addColumn<BobUI::TransformationMode>("transformMode");

    BOBUIest::newRow("16x16 => 32x32") << QSize(16, 16) << QSize(32, 32)
                                    << BobUI::IgnoreAspectRatio
                                    << BobUI::FastTransformation;
    BOBUIest::newRow("100x100 => 200x200") << QSize(100, 100) << QSize(200, 200)
                                        << BobUI::IgnoreAspectRatio
                                        << BobUI::FastTransformation;
    BOBUIest::newRow("100x100 => 200x200") << QSize(100, 100) << QSize(200, 200)
                                        << BobUI::IgnoreAspectRatio
                                        << BobUI::FastTransformation;
    BOBUIest::newRow("80x80 => 200x200") << QSize(137, 137) << QSize(200, 200)
                                      << BobUI::IgnoreAspectRatio
                                      << BobUI::FastTransformation;

}

void tst_QPixmap::scaled()
{
    QFETCH(QSize, size);
    QFETCH(QSize, scale);
    QFETCH(BobUI::AspectRatioMode, ratioMode);
    QFETCH(BobUI::TransformationMode, transformMode);

    QPixmap opaque = rasterPixmap(size);
    QPixmap transparent = rasterPixmap(size);
    opaque.fill(QColor(255, 0, 0));
    transparent.fill(QColor(255, 0, 0, 200));

    QPixmap scaled1;
    QPixmap scaled2;
    QBENCHMARK {
        scaled1 = opaque.scaled(scale, ratioMode, transformMode);
        scaled2 = transparent.scaled(scale, ratioMode, transformMode);
    }
}

void tst_QPixmap::transformed_data()
{
    BOBUIest::addColumn<QSize>("size");
    BOBUIest::addColumn<BOBUIransform>("transform");
    BOBUIest::addColumn<BobUI::TransformationMode>("transformMode");

    BOBUIest::newRow("16x16 rotate(90)") << QSize(16, 16)
                                      << BOBUIransform().rotate(90)
                                      << BobUI::FastTransformation;
    BOBUIest::newRow("16x16 rotate(199)") << QSize(16, 16)
                                       << BOBUIransform().rotate(199)
                                       << BobUI::FastTransformation;
    BOBUIest::newRow("16x16 shear(2,1)") << QSize(16, 16)
                                      << BOBUIransform().shear(2, 1)
                                      << BobUI::FastTransformation;
    BOBUIest::newRow("16x16 rotate(199).shear(2,1)") << QSize(16, 16)
                                                  << BOBUIransform().rotate(199).shear(2, 1)
                                                  << BobUI::FastTransformation;
    BOBUIest::newRow("100x100 rotate(90)") << QSize(100, 100)
                                        << BOBUIransform().rotate(90)
                                        << BobUI::FastTransformation;
    BOBUIest::newRow("100x100 rotate(199)") << QSize(100, 100)
                                         << BOBUIransform().rotate(199)
                                         << BobUI::FastTransformation;
    BOBUIest::newRow("100x100 shear(2,1)") << QSize(100, 100)
                                        << BOBUIransform().shear(2, 1)
                                        << BobUI::FastTransformation;
    BOBUIest::newRow("100x100 shear(2,1) smooth") << QSize(100, 100)
                                               << BOBUIransform().shear(2, 1)
                                               << BobUI::SmoothTransformation;
    BOBUIest::newRow("100x100 rotate(199).shear(2,1)") << QSize(100, 100)
                                                    << BOBUIransform().rotate(199).shear(2, 1)
                                                    << BobUI::FastTransformation;
}

void tst_QPixmap::transformed()
{
    QFETCH(QSize, size);
    QFETCH(BOBUIransform, transform);
    QFETCH(BobUI::TransformationMode, transformMode);

    QPixmap opaque = rasterPixmap(size);
    QPixmap transparent = rasterPixmap(size);
    opaque.fill(QColor(255, 0, 0));
    transparent.fill(QColor(255, 0, 0, 200));

    QPixmap transformed1;
    QPixmap transformed2;
    QBENCHMARK {
        transformed1 = opaque.transformed(transform, transformMode);
        transformed2 = transparent.transformed(transform, transformMode);
    }
}

void tst_QPixmap::mask_data()
{
    BOBUIest::addColumn<QSize>("size");

    BOBUIest::newRow("1x1") << QSize(1, 1);
    BOBUIest::newRow("9x9") << QSize(9, 9);
    BOBUIest::newRow("16x16") << QSize(16, 16);
    BOBUIest::newRow("128x128") << QSize(128, 128);
    BOBUIest::newRow("333x333") << QSize(333, 333);
    BOBUIest::newRow("2048x128") << QSize(2048, 128);
}

void tst_QPixmap::mask()
{
    QFETCH(QSize, size);

    QPixmap src = rasterPixmap(size);
    src.fill(BobUI::transparent);
    {
        QPainter p(&src);
        p.drawLine(QPoint(0, 0), QPoint(src.width(), src.height()));
    }

    QBENCHMARK {
        QBitmap bitmap = src.mask();
        QVERIFY(bitmap.size() == src.size());
    }
}

void tst_QPixmap::fromImageReader_data()
{
    const QString tempDir = QDir::tempPath();
    BOBUIest::addColumn<QString>("filename");

    QImage image(2000, 2000, QImage::Format_ARGB32);
    image.fill(0);
    {
        // Generate an image with opaque and transparent pixels
        // with an interesting distribution.
        QPainter painter(&image);

        QRadialGradient radialGrad(QPointF(1000, 1000), 1000);
        radialGrad.setColorAt(0, QColor(255, 0, 0, 255));
        radialGrad.setColorAt(0.5, QColor(0, 255, 0, 255));
        radialGrad.setColorAt(0.9, QColor(0, 0, 255, 100));
        radialGrad.setColorAt(1, QColor(0, 0, 0, 0));

        painter.fillRect(image.rect(), radialGrad);
    }
    image.save("test.png");

    // RGB32
    const QString rgb32Path = tempDir + QString::fromLatin1("/rgb32.jpg");
    image.save(rgb32Path);
    BOBUIest::newRow("gradient RGB32") << rgb32Path;

    // ARGB32
    const QString argb32Path = tempDir + QString::fromLatin1("/argb32.png");
    image.save(argb32Path);
    BOBUIest::newRow("gradient ARGB32") << argb32Path;

    // Indexed 8
    const QString indexed8Path = tempDir + QString::fromLatin1("/indexed8.gif");
    image.save(indexed8Path);
    BOBUIest::newRow("gradient indexed8") << indexed8Path;

}

void tst_QPixmap::fromImageReader()
{
    QFETCH(QString, filename);
    // warmup
    {
        QImageReader imageReader(filename);
        QPixmap::fromImageReader(&imageReader);
    }

    QBENCHMARK {
        QImageReader imageReader(filename);
        QPixmap::fromImageReader(&imageReader);
    }
    QFile::remove(filename);
}


BOBUIEST_MAIN(tst_QPixmap)

#include "tst_qpixmap.moc"
