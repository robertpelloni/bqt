// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <QBuffer>
#include <QColorSpace>
#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QPixmap>
#include <QScopeGuard>
#include <BOBUIcpSocket>
#include <BOBUIcpServer>
#include <BOBUIimer>
#include <BOBUIemporaryDir>
#include <BOBUIemporaryFile>

#include <algorithm>

// #define DEBUG_WRITE_OUTPUT

typedef QMap<QString, QString> QStringMap;
typedef QList<int> QIntList;
Q_DECLARE_METATYPE(QImage::Format)

static QByteArray msgFileOpenWriteFailed(const QFile &file)
{
    const QString result = QLatin1String("Cannot open \"")
        + QDir::toNativeSeparators(file.fileName())
        + QLatin1String("\" for writing: ") + file.errorString();
    return result.toLocal8Bit();
}

static QByteArray msgFileOpenReadFailed(const QFile &file)
{
    const QString result = QLatin1String("Cannot open \"")
        + QDir::toNativeSeparators(file.fileName())
        + QLatin1String("\" for reading: ") + file.errorString();
    return result.toLocal8Bit();
}

class tst_QImageReader : public QObject
{
    Q_OBJECT

public:
    tst_QImageReader();
    virtual ~tst_QImageReader();

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void getSetCheck();
    void readImage_data();
    void readImage();
    void jpegRgbCmyk();

    void setScaledSize_data();
    void setScaledSize();

    void setScaledSizeOneDimension_data();
    void setScaledSizeOneDimension();

    void setClipRect_data();
    void setClipRect();

    void setScaledClipRect_data();
    void setScaledClipRect();

    void setFormat();

    void imageFormat_data();
    void imageFormat();

    void blackXPM();
    void transparentXPM();
    void multiWordNamedColorXPM();

    void supportedFormats();
    void supportedMimeTypes();

    void readFromDevice_data();
    void readFromDevice();

    void readFromFileAfterJunk_data();
    void readFromFileAfterJunk();

    void devicePosition_data();
    void devicePosition();

    void setBackgroundColor_data();
    void setBackgroundColor();

    void supportsAnimation_data();
    void supportsAnimation();

    void readFromResources_data();
    void readFromResources();

    void sizeBeforeRead_data();
    void sizeBeforeRead();

    void sizeBeforeFormat_data();
    void sizeBeforeFormat();

    void imageFormatBeforeRead_data();
    void imageFormatBeforeRead();

    void gifHandlerBugs();
    void animatedGif();
    void gifImageCount();
    void gifLoopCount();

    void ppmMaxval_data();
    void ppmMaxval();

    void readCorruptImage_data();
    void readCorruptImage();
    void readCorruptBmp();

    void supportsOption_data();
    void supportsOption();

    void autoDetectImageFormat();
    void fileNameProbing();

    void pixelCompareWithBaseline_data();
    void pixelCompareWithBaseline();

    void task255627_setNullScaledSize_data();
    void task255627_setNullScaledSize();

    void testIgnoresFormatAndExtension_data();
    void testIgnoresFormatAndExtension();

    void saveFormat_data();
    void saveFormat();

    void saveColorSpace_data();
    void saveColorSpace();

    void readText_data();
    void readText();

    void preserveTexts_data();
    void preserveTexts();

    void devicePixelRatio_data();
    void devicePixelRatio();

    void xpmBufferOverflow();

    void xbmBufferHandling();

private:
    QString prefix;
    BOBUIemporaryDir m_temporaryDir;
};

// helper to skip an autotest when the given image format is not supported
#define SKIP_IF_UNSUPPORTED(format) do {                                                          \
    if (!QByteArray(format).isEmpty() && !QImageReader::supportedImageFormats().contains(format)) \
        QSKIP('"' + QByteArray(format) + "\" images are not supported");             \
} while (0)

// Testing get/set functions
void tst_QImageReader::getSetCheck()
{
    QImageReader obj1;
    // QIODevice * QImageReader::device()
    // void QImageReader::setDevice(QIODevice *)
    QFile *var1 = new QFile;
    obj1.setDevice(var1);

    //A bit strange but that's the only way to compile under windows.
    QCOMPARE((QIODevice *) var1, obj1.device());
    obj1.setDevice((QIODevice *)0);
    QCOMPARE((QIODevice *) 0,
             obj1.device());
    delete var1;
}

tst_QImageReader::tst_QImageReader() :
    m_temporaryDir(QDir::tempPath() + QStringLiteral("/tst_qimagereaderXXXXXX"))
{
    m_temporaryDir.setAutoRemove(true);
}

tst_QImageReader::~tst_QImageReader()
{

}

void tst_QImageReader::initTestCase()
{
    prefix = QFINDTESTDATA("images/");
    if (prefix.isEmpty())
        QFAIL("Can't find images directory!");
   QVERIFY2(m_temporaryDir.isValid(), qPrintable(m_temporaryDir.errorString()));
}

void tst_QImageReader::cleanupTestCase()
{
}

void tst_QImageReader::readImage_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<bool>("success");
    BOBUIest::addColumn<QByteArray>("format");

    BOBUIest::newRow("empty") << QString() << false << QByteArray();
    BOBUIest::newRow("BMP: colorful") << QString("colorful.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("BMP: test32bfv4") << QString("test32bfv4.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("BMP: test32v5") << QString("test32v5.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("BMP: font") << QString("font.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("BMP: signed char") << QString("crash-signed-char.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("BMP: 4bpp RLE") << QString("4bpp-rle.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("BMP: 4bpp uncompressed") << QString("tst7.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("BMP: 16bpp") << QString("16bpp.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("BMP: negative height") << QString("negativeheight.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("BMP: high mask bit set") << QString("rgb32bf.bmp") << true << QByteArray("bmp");
    BOBUIest::newRow("XPM: marble") << QString("marble.xpm") << true << QByteArray("xpm");
    BOBUIest::newRow("PNG: kollada") << QString("kollada.png") << true << QByteArray("png");
    BOBUIest::newRow("PNG: kollada 16bpc") << QString("kollada-16bpc.png") << true << QByteArray("png");
    BOBUIest::newRow("PPM: teapot") << QString("teapot.ppm") << true << QByteArray("ppm");
    BOBUIest::newRow("PPM: runners") << QString("runners.ppm") << true << QByteArray("ppm");
    BOBUIest::newRow("PPM: test") << QString("test.ppm") << true << QByteArray("ppm");
    BOBUIest::newRow("XBM: gnus") << QString("gnus.xbm") << true << QByteArray("xbm");
    BOBUIest::newRow("PGM: longcomment") << QString("longcomment.pgm") << true << QByteArray("pgm");

    BOBUIest::newRow("JPEG: beavis") << QString("beavis.jpg") << true << QByteArray("jpeg");
    BOBUIest::newRow("JPEG: bobuibug13653") << QString("bobuibug13653-no_eoi.jpg") << true << QByteArray("jpeg");

    BOBUIest::newRow("GIF: earth") << QString("earth.gif") << true << QByteArray("gif");
    BOBUIest::newRow("GIF: trolltech") << QString("trolltech.gif") << true << QByteArray("gif");

    BOBUIest::newRow("SVG: rect") << QString("rect.svg") << true << QByteArray("svg");
    BOBUIest::newRow("SVGZ: rect") << QString("rect.svgz") << true << QByteArray("svgz");
}

void tst_QImageReader::readImage()
{
    QFETCH(QString, fileName);
    QFETCH(bool, success);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    for (int i = 0; i < 2; ++i) {
        QImageReader io(prefix + fileName, i ? QByteArray() : format);
        if (success) {
            if (!io.supportsAnimation())
                QVERIFY(io.imageCount() > 0);
        } else {
            QCOMPARE(io.imageCount(), -1);
        }
        QImage image = io.read();
        if (!success) {
            QVERIFY(image.isNull());
            return;
        }

        QVERIFY2(!image.isNull(), io.errorString().toLatin1().constData());

        // No format
        QImageReader io2(prefix + fileName);
        QVERIFY2(!io2.read().isNull(), io.errorString().toLatin1().constData());

        // No extension, no format
        QImageReader io3(prefix + fileName.left(fileName.lastIndexOf(QLatin1Char('.'))));
        QVERIFY2(!io3.read().isNull(), io.errorString().toLatin1().constData());

        // Read into \a image2
        QImage image2;
        QImageReader image2Reader(prefix + fileName, i ? QByteArray() : format);
        QCOMPARE(image2Reader.format(), format);
        QVERIFY(image2Reader.read(&image2));
        if (image2Reader.canRead()) {
            if (i)
                QVERIFY(!image2Reader.format().isEmpty());
            else
                QCOMPARE(image2Reader.format(), format);
        } else {
            if (i)
                QVERIFY(image2Reader.format().isEmpty());
            else
                QVERIFY(!image2Reader.format().isEmpty());
        }
        QCOMPARE(image, image2);
        do {
            QVERIFY2(!image.isNull(), io.errorString().toLatin1().constData());
        } while (!(image = io.read()).isNull());
    }
}

void tst_QImageReader::jpegRgbCmyk()
{
    QImage image1(prefix + QLatin1String("YCbCr_cmyk.jpg"));
    QImage image2(prefix + QLatin1String("YCbCr_cmyk.png"));

    QVERIFY(!image1.isNull());
    QVERIFY(!image2.isNull());

    QCOMPARE(image1.height(), image2.height());
    QCOMPARE(image1.width(), image2.width());

    QCOMPARE(image1.format(), QImage::Format_CMYK8888);
    QCOMPARE(image2.format(), QImage::Format_RGB32);

    // compare all the pixels with a slack of 3. This ignores rounding errors
    // in libjpeg/libpng, where some versions sacrifice accuracy for speed.
    const auto fuzzyCompareColors = [](const QColor &c1, const QColor &c2) {
        int c1rgba[4];
        int c2rgba[4];

        c1.getRgb(c1rgba + 0,
                  c1rgba + 1,
                  c1rgba + 2,
                  c1rgba + 3);

        c2.getRgb(c2rgba + 0,
                  c2rgba + 1,
                  c2rgba + 2,
                  c2rgba + 3);

        const auto fuzzyCompare = [](int a, int b) {
            return qAbs(a - b) <= 3;
        };

        return fuzzyCompare(c1rgba[0], c2rgba[0]) &&
               fuzzyCompare(c1rgba[1], c2rgba[1]) &&
               fuzzyCompare(c1rgba[2], c2rgba[2]) &&
               fuzzyCompare(c1rgba[3], c2rgba[3]);
    };

    for (int h = 0; h < image1.height(); ++h) {
        const uchar *sl1 = image1.constScanLine(h);
        const uchar *sl2 = image2.constScanLine(h);
        for (int w = 0; w < image1.width(); ++w) {
            const uchar *s1 = sl1 + w * 4;
            const uchar *s2 = sl2 + w * 4;

            QColor c1 = QColor::fromCmyk(s1[0], s1[1], s1[2], s1[3]);
            QColor c2 = QColor::fromRgb(s2[2], s2[1], s2[0]);
            QVERIFY2(fuzzyCompareColors(c1, c2),
                     qPrintable(QString("images differ in line %1, col %2").arg(h).arg(w)));
        }
    }
}

void tst_QImageReader::setScaledSize_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QSize>("newSize");
    BOBUIest::addColumn<QByteArray>("format");

    BOBUIest::newRow("BMP: colorful") << "colorful" << QSize(200, 200) << QByteArray("bmp");
    BOBUIest::newRow("BMP: font") << "font" << QSize(200, 200) << QByteArray("bmp");
    BOBUIest::newRow("XPM: marble") << "marble" << QSize(200, 200) << QByteArray("xpm");
    BOBUIest::newRow("PNG: kollada") << "kollada" << QSize(200, 200) << QByteArray("png");
    BOBUIest::newRow("PPM: teapot") << "teapot" << QSize(200, 200) << QByteArray("ppm");
    BOBUIest::newRow("PPM: runners") << "runners.ppm" << QSize(400, 400) << QByteArray("ppm");
    BOBUIest::newRow("PPM: test") << "test.ppm" << QSize(10, 10) << QByteArray("ppm");
    BOBUIest::newRow("XBM: gnus") << "gnus" << QSize(200, 200) << QByteArray("xbm");

    BOBUIest::newRow("JPEG: beavis A") << "beavis" << QSize(200, 200) << QByteArray("jpeg");
    BOBUIest::newRow("JPEG: beavis B") << "beavis" << QSize(175, 175) << QByteArray("jpeg");
    BOBUIest::newRow("JPEG: beavis C") << "beavis" << QSize(100, 100) << QByteArray("jpeg");
    BOBUIest::newRow("JPEG: beavis D") << "beavis" << QSize(100, 200) << QByteArray("jpeg");
    BOBUIest::newRow("JPEG: beavis E") << "beavis" << QSize(200, 100) << QByteArray("jpeg");
    BOBUIest::newRow("JPEG: beavis F") << "beavis" << QSize(87, 87) << QByteArray("jpeg");
    BOBUIest::newRow("JPEG: beavis G") << "beavis" << QSize(50, 45) << QByteArray("jpeg");
    BOBUIest::newRow("JPEG: beavis H") << "beavis" << QSize(43, 43) << QByteArray("jpeg");
    BOBUIest::newRow("JPEG: beavis I") << "beavis" << QSize(25, 25) << QByteArray("jpeg");

    BOBUIest::newRow("GIF: earth") << "earth" << QSize(200, 200) << QByteArray("gif");
    BOBUIest::newRow("GIF: trolltech") << "trolltech" << QSize(200, 200) << QByteArray("gif");

    BOBUIest::newRow("SVG: rect") << "rect" << QSize(200, 200) << QByteArray("svg");
    BOBUIest::newRow("SVGZ: rect") << "rect" << QSize(200, 200) << QByteArray("svgz");
}

void tst_QImageReader::setScaledSize()
{
    QFETCH(QString, fileName);
    QFETCH(QSize, newSize);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    QImageReader reader(prefix + fileName);
    reader.setScaledSize(newSize);
    QImage image = reader.read();
    QVERIFY(!image.isNull());

    QCOMPARE(image.size(), newSize);
}

void tst_QImageReader::setScaledSizeOneDimension_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QByteArray>("format");

    BOBUIest::newRow("PNG: kollada") << QString("kollada") << QByteArray("png");
    BOBUIest::newRow("JPEG: beavis") << QString("beavis") << QByteArray("jpeg");
    BOBUIest::newRow("GIF: earth") << QString("earth") << QByteArray("gif");
    BOBUIest::newRow("SVG: rect") << QString("rect") << QByteArray("svg");
    BOBUIest::newRow("BMP: colorful") << QString("colorful") << QByteArray("bmp");
    BOBUIest::newRow("XPM: marble") << QString("marble") << QByteArray("xpm");
    BOBUIest::newRow("PPM: teapot") << QString("teapot") << QByteArray("ppm");
    BOBUIest::newRow("XBM: gnus") << QString("gnus") << QByteArray("xbm");
}

void tst_QImageReader::setScaledSizeOneDimension()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    const QSize originalSize = QImageReader(prefix + fileName).size();
    QVERIFY(!originalSize.isEmpty());

    auto testScaledSize = [&] (const QSize &scaledSize) {
        QSize expectedSize = scaledSize;
        if (scaledSize.width() <= 0)
            expectedSize.setWidth(qRound(originalSize.width() *
                                         (qreal(scaledSize.height()) / originalSize.height())));
        else if (scaledSize.height() <= 0)
            expectedSize.setHeight(qRound(originalSize.height() *
                                          (qreal(scaledSize.width()) / originalSize.width())));

        QImageReader reader(prefix + fileName);
        reader.setScaledSize(scaledSize);
        QImage image = reader.read();
        QVERIFY(!image.isNull());
        QCOMPARE(image.size(), expectedSize);
    };

    // downscale
    testScaledSize(QSize(originalSize.width() / 2, 0));
    testScaledSize(QSize(originalSize.width() / 2, -1));
    testScaledSize(QSize(0, originalSize.height() / 2));
    testScaledSize(QSize(-1, originalSize.height() / 2));

    // upscale
    testScaledSize(QSize(originalSize.width() * 2, 0));
    testScaledSize(QSize(originalSize.width() * 2, -1));
    testScaledSize(QSize(0, originalSize.height() * 2));
    testScaledSize(QSize(-1, originalSize.height() * 2));
}

void tst_QImageReader::task255627_setNullScaledSize_data()
{
    setScaledSize_data();
}

void tst_QImageReader::task255627_setNullScaledSize()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    QImageReader reader(prefix + fileName);

    // set a null size
    reader.setScaledSize(QSize(0, 0));
    reader.setQuality(0);
    QImage image = reader.read();
    QVERIFY(image.isNull());
    QCOMPARE(image.size(), QSize(0, 0));
}

void tst_QImageReader::setClipRect_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QRect>("newRect");
    BOBUIest::addColumn<QByteArray>("format");
    BOBUIest::newRow("BMP: colorful") << "colorful" << QRect(0, 0, 50, 50) << QByteArray("bmp");
    BOBUIest::newRow("BMP: test32bfv4") << "test32bfv4" << QRect(0, 0, 50, 50) << QByteArray("bmp");
    BOBUIest::newRow("BMP: test32v5") << "test32v5" << QRect(0, 0, 50, 50) << QByteArray("bmp");
    BOBUIest::newRow("BMP: font") << "font" << QRect(0, 0, 50, 50) << QByteArray("bmp");
    BOBUIest::newRow("BMP: 4bpp uncompressed") << "tst7.bmp" << QRect(0, 0, 31, 31) << QByteArray("bmp");
    BOBUIest::newRow("XPM: marble") << "marble" << QRect(0, 0, 50, 50) << QByteArray("xpm");
    BOBUIest::newRow("PNG: kollada") << "kollada" << QRect(0, 0, 50, 50) << QByteArray("png");
    BOBUIest::newRow("PPM: teapot") << "teapot" << QRect(0, 0, 50, 50) << QByteArray("ppm");
    BOBUIest::newRow("PPM: runners") << "runners.ppm" << QRect(0, 0, 50, 50) << QByteArray("ppm");
    BOBUIest::newRow("PPM: test") << "test.ppm" << QRect(0, 0, 50, 50) << QByteArray("ppm");
    BOBUIest::newRow("XBM: gnus") << "gnus" << QRect(0, 0, 50, 50) << QByteArray("xbm");

    BOBUIest::newRow("JPEG: beavis") << "beavis" << QRect(0, 0, 50, 50) << QByteArray("jpeg");

    BOBUIest::newRow("GIF: earth") << "earth" << QRect(0, 0, 50, 50) << QByteArray("gif");
    BOBUIest::newRow("GIF: trolltech") << "trolltech" << QRect(0, 0, 50, 50) << QByteArray("gif");

    BOBUIest::newRow("SVG: rect") << "rect" << QRect(0, 0, 50, 50) << QByteArray("svg");
    BOBUIest::newRow("SVGZ: rect") << "rect" << QRect(0, 0, 50, 50) << QByteArray("svgz");
}

void tst_QImageReader::setClipRect()
{
    QFETCH(QString, fileName);
    QFETCH(QRect, newRect);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    QImageReader reader(prefix + fileName);
    reader.setClipRect(newRect);
    QImage image = reader.read();
    QVERIFY(!image.isNull());
    QCOMPARE(image.rect(), newRect);

    QImageReader originalReader(prefix + fileName);
    QImage originalImage = originalReader.read();
    QCOMPARE(originalImage.copy(newRect), image);
}

void tst_QImageReader::setScaledClipRect_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QRect>("newRect");
    BOBUIest::addColumn<QByteArray>("format");

    BOBUIest::newRow("BMP: colorful") << "colorful" << QRect(50, 20, 50, 50) << QByteArray("bmp");
    BOBUIest::newRow("BMP: test32bfv4") << "test32bfv4" << QRect(50, 20, 50, 50) << QByteArray("bmp");
    BOBUIest::newRow("BMP: test32v5") << "test32v5" << QRect(50, 20, 50, 50) << QByteArray("bmp");
    BOBUIest::newRow("BMP: font") << "font" << QRect(50, 20, 50, 50) << QByteArray("bmp");
    BOBUIest::newRow("XPM: marble") << "marble" << QRect(50, 20, 50, 50) << QByteArray("xpm");
    BOBUIest::newRow("PNG: kollada") << "kollada" << QRect(50, 20, 50, 50) << QByteArray("png");
    BOBUIest::newRow("PPM: teapot") << "teapot" << QRect(50, 20, 50, 50) << QByteArray("ppm");
    BOBUIest::newRow("PPM: runners") << "runners.ppm" << QRect(50, 20, 50, 50) << QByteArray("ppm");
    BOBUIest::newRow("PPM: test") << "test.ppm" << QRect(50, 20, 50, 50) << QByteArray("ppm");
    BOBUIest::newRow("XBM: gnus") << "gnus" << QRect(50, 20, 50, 50) << QByteArray("xbm");

    BOBUIest::newRow("JPEG: beavis") << "beavis" << QRect(50, 20, 50, 50) << QByteArray("jpeg");

    BOBUIest::newRow("GIF: earth") << "earth" << QRect(50, 20, 50, 50) << QByteArray("gif");
    BOBUIest::newRow("GIF: trolltech") << "trolltech" << QRect(50, 20, 50, 50) << QByteArray("gif");

    BOBUIest::newRow("SVG: rect") << "rect" << QRect(50, 20, 50, 50) << QByteArray("svg");
    BOBUIest::newRow("SVGZ: rect") << "rect" << QRect(50, 20, 50, 50) << QByteArray("svgz");
}

void tst_QImageReader::setScaledClipRect()
{
    QFETCH(QString, fileName);
    QFETCH(QRect, newRect);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    QImageReader reader(prefix + fileName);
    reader.setScaledSize(QSize(300, 300));
    reader.setScaledClipRect(newRect);
    QImage image = reader.read();
    QVERIFY(!image.isNull());
    QCOMPARE(image.rect().translated(50, 20), newRect);
#ifdef DEBUG_WRITE_OUTPUT
    QString tempPath = QDir::temp().filePath(fileName) + QLatin1String(".png");
    image.save(tempPath);
#endif

    QImageReader originalReader(prefix + fileName);
    originalReader.setScaledSize(QSize(300, 300));
    QImage originalImage = originalReader.read();
    if (format.contains("svg")) {
        // rendering of subrect may yield slight rounding differences, truncate them away
        image.convertTo(QImage::Format_RGB444);
        originalImage.convertTo(QImage::Format_RGB444);
    }
    QCOMPARE(originalImage.copy(newRect), image);
}

void tst_QImageReader::setFormat()
{
    QByteArray ppmImage = "P1 2 2\n1 0\n0 1";
    QBuffer buf(&ppmImage);
    QImageReader reader(&buf);

    // read image in autodetected format
    QCOMPARE(reader.size(), QSize(2,2));
    buf.close();

    // try reading with non-matching format, must not succeed
    reader.setDecideFormatFromContent(false);
    reader.setFormat("bmp");
    reader.setDevice(&buf);
    QCOMPARE(reader.size(), QSize());
    buf.close();

    // read with manually set matching format
    reader.setFormat("ppm");
    reader.setDevice(&buf);
    QCOMPARE(reader.size(), QSize(2,2));
    buf.close();
}

void tst_QImageReader::imageFormat_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QByteArray>("format");
    BOBUIest::addColumn<QImage::Format>("imageFormat");

    BOBUIest::newRow("pbm") << QString("image.pbm") << QByteArray("pbm") << QImage::Format_Mono;
    BOBUIest::newRow("pgm") << QString("image.pgm") << QByteArray("pgm") << QImage::Format_Grayscale8;
    BOBUIest::newRow("pgm") << QString("image16.pgm") << QByteArray("pgm") << QImage::Format_Grayscale16;
    BOBUIest::newRow("ppm-1") << QString("image.ppm") << QByteArray("ppm") << QImage::Format_RGB32;
    BOBUIest::newRow("ppm-2") << QString("teapot.ppm") << QByteArray("ppm") << QImage::Format_RGB32;
    BOBUIest::newRow("ppm-3") << QString("runners.ppm") << QByteArray("ppm") << QImage::Format_RGB32;
    BOBUIest::newRow("ppm-4") << QString("test.ppm") << QByteArray("ppm") << QImage::Format_RGB32;

    BOBUIest::newRow("jpeg-1") << QString("beavis.jpg") << QByteArray("jpeg") << QImage::Format_Grayscale8;
    BOBUIest::newRow("jpeg-2") << QString("YCbCr_cmyk.jpg") << QByteArray("jpeg") << QImage::Format_CMYK8888;
    BOBUIest::newRow("jpeg-3") << QString("YCbCr_rgb.jpg") << QByteArray("jpeg") << QImage::Format_RGB32;

    BOBUIest::newRow("gif-1") << QString("earth.gif") << QByteArray("gif") << QImage::Format_Invalid;
    BOBUIest::newRow("gif-2") << QString("trolltech.gif") << QByteArray("gif") << QImage::Format_Invalid;

    BOBUIest::newRow("xbm") << QString("gnus.xbm") << QByteArray("xbm") << QImage::Format_MonoLSB;
    BOBUIest::newRow("xpm") << QString("marble.xpm") << QByteArray("xpm") << QImage::Format_Indexed8;
    BOBUIest::newRow("bmp-1") << QString("colorful.bmp") << QByteArray("bmp") << QImage::Format_Indexed8;
    BOBUIest::newRow("bmp-2") << QString("font.bmp") << QByteArray("bmp") << QImage::Format_Indexed8;
    BOBUIest::newRow("bmp-3") << QString("test32bfv4.bmp") << QByteArray("bmp") << QImage::Format_ARGB32;
    BOBUIest::newRow("bmp-4") << QString("test32v5.bmp") << QByteArray("bmp") << QImage::Format_RGB32;
    BOBUIest::newRow("png") << QString("kollada.png") << QByteArray("png") << QImage::Format_ARGB32;
    BOBUIest::newRow("png-2") << QString("YCbCr_cmyk.png") << QByteArray("png") << QImage::Format_RGB32;
    BOBUIest::newRow("png-3") << QString("kollada-16bpc.png") << QByteArray("png") << QImage::Format_RGBA64;
    BOBUIest::newRow("png-4") << QString("basn0g16.png") << QByteArray("png") << QImage::Format_Grayscale16;
    BOBUIest::newRow("png-5") << QString("basn2c16.png") << QByteArray("png") << QImage::Format_RGBX64;
    BOBUIest::newRow("png-6") << QString("basn4a16.png") << QByteArray("png") << QImage::Format_RGBA64; // Grayscale16Alpha16
    BOBUIest::newRow("png-7") << QString("basn6a16.png") << QByteArray("png") << QImage::Format_RGBA64;
    BOBUIest::newRow("png-8") << QString("tbwn0g16.png") << QByteArray("png") << QImage::Format_RGBA64; // Grayscale16+tRNS
    BOBUIest::newRow("svg") << QString("rect.svg") << QByteArray("svg") << QImage::Format_ARGB32_Premultiplied;
    BOBUIest::newRow("svgz") << QString("rect.svgz") << QByteArray("svgz") << QImage::Format_ARGB32_Premultiplied;
}

void tst_QImageReader::imageFormat()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);
    QFETCH(QImage::Format, imageFormat);

    SKIP_IF_UNSUPPORTED(format);

    QCOMPARE(QImageReader::imageFormat(prefix + fileName), format);
    QImageReader reader(prefix + fileName);
    QCOMPARE(reader.imageFormat(), imageFormat);
}

void tst_QImageReader::blackXPM()
{
    QImage image(prefix + QLatin1String("black.xpm"));
    QImage image2(prefix + QLatin1String("black.png"));
    QCOMPARE(image.pixel(25, 25), qRgb(190, 190, 190));
    QCOMPARE(image.pixel(25, 25), image2.pixel(25, 25));
}

void tst_QImageReader::transparentXPM()
{
    QImage image(prefix + QLatin1String("nontransparent.xpm"));
    QImage image2(prefix + QLatin1String("transparent.xpm"));
    QCOMPARE(image.format(), QImage::Format_RGB32);
    QCOMPARE(image2.format(), QImage::Format_ARGB32);
}

void tst_QImageReader::multiWordNamedColorXPM()
{
    QImage image(prefix + QLatin1String("namedcolors.xpm"));
    QCOMPARE(image.pixel(0, 0), qRgb(102, 139, 139)); // pale turquoise 4
    QCOMPARE(image.pixel(0, 1), qRgb(250, 250, 210)); // light golden rod yellow
    QCOMPARE(image.pixel(0, 2), qRgb(255, 250, 205)); // lemon chiffon
}

namespace {
template <typename ForwardIterator>
bool is_sorted_unique(ForwardIterator first, ForwardIterator last)
{
    // a range is sorted with no dups iff each *i < *(i+1), so check that none are >=:
    return std::adjacent_find(first, last, std::greater_equal<>{}) == last;
}
}

void tst_QImageReader::supportedFormats()
{
    const QList<QByteArray> formats = QImageReader::supportedImageFormats();
    auto printOnFailure = qScopeGuard([&] { qDebug() << formats; });
    QVERIFY(is_sorted_unique(formats.begin(), formats.end()));
    printOnFailure.dismiss();
}

void tst_QImageReader::supportedMimeTypes()
{
    const QList<QByteArray> mimeTypes = QImageReader::supportedMimeTypes();
    auto printOnFailure = qScopeGuard([&] { qDebug() << mimeTypes; });
    QVERIFY(is_sorted_unique(mimeTypes.begin(), mimeTypes.end()));
    // check the list as a minimum contains image/bmp
    QVERIFY(mimeTypes.contains("image/bmp"));
    printOnFailure.dismiss();
}

void tst_QImageReader::setBackgroundColor_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QColor>("color");

    BOBUIest::newRow("BMP: colorful") << QString("colorful.bmp") << QColor(BobUI::white);
    BOBUIest::newRow("BMP: font") << QString("font.bmp") << QColor(BobUI::black);
    BOBUIest::newRow("BMP: signed char") << QString("crash-signed-char.bmp") << QColor(BobUI::red);
    BOBUIest::newRow("XPM: marble") << QString("marble.xpm") << QColor(BobUI::darkRed);
    BOBUIest::newRow("PNG: kollada") << QString("kollada.png") << QColor(BobUI::green);
    BOBUIest::newRow("PPM: teapot") << QString("teapot.ppm") << QColor(BobUI::darkGreen);
    BOBUIest::newRow("PPM: runners") << QString("runners.ppm") << QColor(BobUI::red);
    BOBUIest::newRow("PPM: test") << QString("test.ppm") << QColor(BobUI::white);
    BOBUIest::newRow("XBM: gnus") << QString("gnus.xbm") << QColor(BobUI::blue);

    BOBUIest::newRow("JPEG: beavis") << QString("beavis.jpg") << QColor(BobUI::darkBlue);

    BOBUIest::newRow("GIF: earth") << QString("earth.gif") << QColor(BobUI::cyan);
    BOBUIest::newRow("GIF: trolltech") << QString("trolltech.gif") << QColor(BobUI::magenta);

    BOBUIest::newRow("SVG: rect") << QString("rect.svg") << QColor(BobUI::darkGreen);
    BOBUIest::newRow("SVGZ: rect") << QString("rect.svgz") << QColor(BobUI::darkGreen);
}

void tst_QImageReader::setBackgroundColor()
{
    QFETCH(QString, fileName);
    QFETCH(QColor, color);
    QImageReader io("images/" + fileName);
    io.setBackgroundColor(color);
    if (io.backgroundColor().isValid())
        QCOMPARE(io.backgroundColor(), color);
}

void tst_QImageReader::supportsAnimation_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<bool>("success");

    BOBUIest::newRow("BMP: colorful") << QString("colorful.bmp") << false;
    BOBUIest::newRow("BMP: font") << QString("font.bmp") << false;
    BOBUIest::newRow("BMP: signed char") << QString("crash-signed-char.bmp") << false;
    BOBUIest::newRow("BMP: test32bfv4") << QString("test32bfv4.bmp") << false;
    BOBUIest::newRow("BMP: test32v5") << QString("test32v5.bmp") << false;
    BOBUIest::newRow("XPM: marble") << QString("marble.xpm") << false;
    BOBUIest::newRow("PNG: kollada") << QString("kollada.png") << false;
    BOBUIest::newRow("PPM: teapot") << QString("teapot.ppm") << false;
    BOBUIest::newRow("PPM: runners") << QString("runners.ppm") << false;
    BOBUIest::newRow("XBM: gnus") << QString("gnus.xbm") << false;

    BOBUIest::newRow("JPEG: beavis") << QString("beavis.jpg") << false;

    BOBUIest::newRow("GIF: earth") << QString("earth.gif") << true;
    BOBUIest::newRow("GIF: trolltech") << QString("trolltech.gif") << true;

    BOBUIest::newRow("SVG: rect") << QString("rect.svg") << false;
    BOBUIest::newRow("SVGZ: rect") << QString("rect.svgz") << false;
}

void tst_QImageReader::supportsAnimation()
{
    QFETCH(QString, fileName);
    QFETCH(bool, success);
    QImageReader io(prefix + fileName);
    QCOMPARE(io.supportsAnimation(), success);
}

void tst_QImageReader::sizeBeforeRead_data()
{
    imageFormat_data();
}

void tst_QImageReader::sizeBeforeRead()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    QImageReader reader(prefix + fileName);
    QVERIFY(reader.canRead());

    QSize size = reader.size();
    QVERIFY(reader.canRead());
    QImage image = reader.read();
    QVERIFY(!image.isNull());
    QCOMPARE(size, image.size());
}

void tst_QImageReader::sizeBeforeFormat_data()
{
    imageFormat_data();
}

void tst_QImageReader::sizeBeforeFormat()
{
    QFETCH(QString, fileName);

    QByteArray formatA, formatB;

    {
        QImageReader reader(prefix + fileName);
        formatA = reader.format();
    }

    {
        QImageReader reader(prefix + fileName);
        reader.size();
        formatB = reader.format();
    }

    QCOMPARE(formatA, formatB);
}

void tst_QImageReader::imageFormatBeforeRead_data()
{
    imageFormat_data();
}

void tst_QImageReader::imageFormatBeforeRead()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);
    QFETCH(QImage::Format, imageFormat);

    SKIP_IF_UNSUPPORTED(format);

    QImageReader reader(prefix + fileName);
    QVERIFY(reader.canRead());
    if (reader.supportsOption(QImageIOHandler::ImageFormat)) {
        QImage::Format fileFormat = reader.imageFormat();
        QCOMPARE(fileFormat, imageFormat);
        QSize size = reader.size();
        QImage image(size, fileFormat);
        QVERIFY(reader.read(&image));
        QCOMPARE(image.format(), fileFormat);
    }
}

void tst_QImageReader::gifHandlerBugs()
{
    SKIP_IF_UNSUPPORTED("gif");

    {
        QImageReader io(prefix + "trolltech.gif");
        QVERIFY(io.loopCount() != 1);
        int count=0;
        for (; io.canRead(); io.read(), ++count) ;
        QCOMPARE(count, 34);
    }

    // Task 95166
    {
        QImageReader io1(prefix + "bat1.gif");
        QImageReader io2(prefix + "bat2.gif");
        QVERIFY(io1.canRead());
        QVERIFY(io2.canRead());
        QImage im1 = io1.read();
        QImage im2 = io2.read();
        QVERIFY(!im1.isNull());
        QVERIFY(!im2.isNull());
        QCOMPARE(im1, im2);
    }

    // Task 9994
    {
        QImageReader io1(prefix + "noclearcode.gif");
        QImageReader io2(prefix + "noclearcode.bmp");
        QVERIFY(io1.canRead());  QVERIFY(io2.canRead());
        QImage im1 = io1.read(); QImage im2 = io2.read();
        QVERIFY(!im1.isNull());  QVERIFY(!im2.isNull());
        QCOMPARE(im1.convertToFormat(QImage::Format_ARGB32), im2.convertToFormat(QImage::Format_ARGB32));
    }

    // Check the undocumented feature.
    {
        QImageReader io(prefix + "endless-anim.gif");
        QVERIFY(io.canRead());
        QCOMPARE(io.loopCount(), -1);
    }

    // Check that pixels with the transparent color are transparent but not zeroed
    {
        QImageReader io(prefix + "trans.gif");
        QVERIFY(io.canRead());
        QImage im = io.read();
        QCOMPARE(im.pixel(0,0), qRgba(0x3f, 0xff, 0x7f, 0x00));
        QCOMPARE(im.pixel(10,10), qRgba(0x3f, 0xff, 0x7f, 0x00));
    }
}

void tst_QImageReader::animatedGif()
{
    SKIP_IF_UNSUPPORTED("gif");

    QImageReader io(":images/bobui.gif");
    QImage image = io.read();
    QVERIFY(!image.isNull());
    int i = 0;
    while(!image.isNull()){
        QString frameName = QLatin1String(":images/bobui") + QString::number(++i) + QLatin1String(".gif");
        QCOMPARE(image, QImage(frameName));
        image = io.read();
    }
}

// BOBUIBUG-6696
// Check the count of images in various call orders...
void tst_QImageReader::gifImageCount()
{
    SKIP_IF_UNSUPPORTED("gif");

    // just read every frame... and see how much we got..
    {
        QImageReader io(":images/four-frames.gif");

        QVERIFY(io.canRead());
        QImage blackFrame = io.read();

        QVERIFY(io.canRead());
        QImage whiteFrame = io.read();

        QVERIFY(io.canRead());
        QImage greenFrame = io.read();

        QCOMPARE(io.imageCount(), 4);

        QVERIFY(io.canRead());
        QImage blueFrame = io.read();

        QVERIFY(!io.canRead());
        QImage emptyFrame = io.read();

        QVERIFY(!io.canRead());
        QCOMPARE(blackFrame.pixel(0,0), qRgb(0, 0, 0));
        QCOMPARE(blackFrame.size(), QSize(64,64));

        QCOMPARE(whiteFrame.pixel(0,0), qRgb(0xff, 0xff, 0xff));
        QCOMPARE(whiteFrame.size(), QSize(64,64));

        QCOMPARE(greenFrame.pixel(0,0), qRgb(0x0, 0xff, 0x0));
        QCOMPARE(greenFrame.size(), QSize(64,64));

        QCOMPARE(blueFrame.pixel(0,0), qRgb(0x0, 0x0, 0xff));
        QCOMPARE(blueFrame.size(), QSize(64,64));
        QVERIFY(emptyFrame.isNull());
    }

    // Read and get the size
    {
        QImageReader io(":images/four-frames.gif");

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QVERIFY(io.canRead());
        QImage blackFrame = io.read();

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QVERIFY(io.canRead());
        QImage whiteFrame = io.read();

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QVERIFY(io.canRead());
        QImage greenFrame = io.read();

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QVERIFY(io.canRead());
        QImage blueFrame = io.read();

        QVERIFY(!io.canRead());
        QCOMPARE(io.size(), QSize());
        QCOMPARE(io.size(), QSize());
        QVERIFY(!io.canRead());
        QImage emptyFrame = io.read();

        QVERIFY(!io.canRead());
        QCOMPARE(blackFrame.pixel(0,0), qRgb(0, 0, 0));
        QCOMPARE(blackFrame.size(), QSize(64,64));

        QCOMPARE(whiteFrame.pixel(0,0), qRgb(0xff, 0xff, 0xff));
        QCOMPARE(whiteFrame.size(), QSize(64,64));

        QCOMPARE(greenFrame.pixel(0,0), qRgb(0x0, 0xff, 0x0));
        QCOMPARE(greenFrame.size(), QSize(64,64));

        QCOMPARE(blueFrame.pixel(0,0), qRgb(0x0, 0x0, 0xff));
        QCOMPARE(blueFrame.size(), QSize(64,64));
        QVERIFY(emptyFrame.isNull());
    }

    // Do a Size query as substitute for canRead
    {
        QImageReader io(":images/four-frames.gif");

        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QImage blackFrame = io.read();

        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QImage whiteFrame = io.read();

        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QImage greenFrame = io.read();

        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QImage blueFrame = io.read();

        QCOMPARE(io.size(), QSize());
        QVERIFY(!io.canRead());
        QImage emptyFrame = io.read();

        QVERIFY(!io.canRead());
        QCOMPARE(blackFrame.pixel(0,0), qRgb(0, 0, 0));
        QCOMPARE(blackFrame.size(), QSize(64,64));

        QCOMPARE(whiteFrame.pixel(0,0), qRgb(0xff, 0xff, 0xff));
        QCOMPARE(whiteFrame.size(), QSize(64,64));

        QCOMPARE(greenFrame.pixel(0,0), qRgb(0x0, 0xff, 0x0));
        QCOMPARE(greenFrame.size(), QSize(64,64));

        QCOMPARE(blueFrame.pixel(0,0), qRgb(0x0, 0x0, 0xff));
        QCOMPARE(blueFrame.size(), QSize(64,64));
        QVERIFY(emptyFrame.isNull());
    }
    {
        QImageReader io(":images/trolltech.gif");
        QCOMPARE(io.imageCount(), 34);
        QCOMPARE(io.size(), QSize(128,64));
    }
}

void tst_QImageReader::gifLoopCount()
{
    SKIP_IF_UNSUPPORTED("gif");

    {
        QImageReader io(":images/bobui-gif-anim.gif");
        QCOMPARE(io.loopCount(), -1); // infinite loop
    }
    {
        QImageReader io(":images/bobui-gif-noanim.gif");
        QCOMPARE(io.loopCount(), 0); // no loop
    }
}

void tst_QImageReader::ppmMaxval_data()
{
    BOBUIest::addColumn<bool>("hasColor");
    BOBUIest::addColumn<QByteArray>("bytes");

    BOBUIest::newRow("PGM plain  8bit full") << false << QByteArray("P2 3 1   255   255 0   127\n");
    BOBUIest::newRow("PGM plain  8bit lim.") << false << QByteArray("P2 3 1    50    50 0    25\n");
    BOBUIest::newRow("PGM plain 16bit full") << false << QByteArray("P2 3 1 65535 65535 0 32767\n");
    BOBUIest::newRow("PGM plain 16bit lim.") << false << QByteArray("P2 3 1  5000  5000 0  2500\n");
    BOBUIest::newRow("PGM raw    8bit full") << false << QByteArray("P5 3 1   255 \xff\x00\x7f", 13 + 3);
    BOBUIest::newRow("PGM raw    8bit lim.") << false << QByteArray("P5 3 1    50 \x32\x00\x19", 13 + 3);
    BOBUIest::newRow("PGM raw   16bit full") << false << QByteArray("P5 3 1 65535 \xff\xff\x00\x00\x7f\xff", 13 + 3 * 2);
    BOBUIest::newRow("PGM raw   16bit lim.") << false << QByteArray("P5 3 1  5000 \x13\x88\x00\x00\x09\xc4", 13 + 3 * 2);

    BOBUIest::newRow("PPM plain  8bit full") << true  << QByteArray("P3 3 2   255 "
                                                                 "255 255 255   0   0   0 127 127 127 "
                                                                 "255   0   0   0 255   0   0   0 255\n");

    BOBUIest::newRow("PPM plain  8bit lim.") << true  << QByteArray("P3 3 2    50 "
                                                                 " 50  50  50   0   0   0  25  25  25 "
                                                                 " 50   0   0   0  50   0   0   0  50\n");

    BOBUIest::newRow("PPM plain 16bit full") << true  << QByteArray("P3 3 2 65535 "
                                                                 "65535 65535 65535     0     0     0 32767 32767 32767 "
                                                                 "65535     0     0     0 65535     0     0     0 65535\n");

    BOBUIest::newRow("PPM plain 16bit lim.") << true  << QByteArray("P3 3 2  5000 "
                                                                 " 5000  5000  5000     0     0     0  2500  2500  2500 "
                                                                 " 5000     0     0     0  5000     0     0     0  5000\n");

    BOBUIest::newRow("PPM raw    8bit full") << true  << QByteArray("P6 3 2   255 "
                                                                 "\xff\xff\xff\x00\x00\x00\x7f\x7f\x7f"
                                                                 "\xff\x00\x00\x00\xff\x00\x00\x00\xff", 13 + 6 * 3);

    BOBUIest::newRow("PPM raw    8bit lim.") << true  << QByteArray("P6 3 2    50 "
                                                                 "\x32\x32\x32\x00\x00\x00\x19\x19\x19"
                                                                 "\x32\x00\x00\x00\x32\x00\x00\x00\x32", 13 + 6 * 3);

    BOBUIest::newRow("PPM raw   16bit full") << true  << QByteArray("P6 3 2 65535 "
                                                                 "\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x7f\xff\x7f\xff\x7f\xff"
                                                                 "\xff\xff\x00\x00\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\xff\xff", 13 + 6 * 3 * 2);

    BOBUIest::newRow("PPM raw   16bit lim.") << true  << QByteArray("P6 3 2  5000 "
                                                                 "\x13\x88\x13\x88\x13\x88\x00\x00\x00\x00\x00\x00\x09\xc4\x09\xc4\x09\xc4"
                                                                 "\x13\x88\x00\x00\x00\x00\x00\x00\x13\x88\x00\x00\x00\x00\x00\x00\x13\x88", 13 + 6 * 3 * 2);
}

void tst_QImageReader::ppmMaxval()
{
    SKIP_IF_UNSUPPORTED("ppm");

    QFETCH(bool, hasColor);
    QFETCH(QByteArray, bytes);

    QImage img;
    img.loadFromData(bytes);
    QVERIFY(!img.isNull());
    QCOMPARE(img.width(), 3);
    QCOMPARE(img.height(), hasColor ? 2 : 1);

    QCOMPARE(img.pixel(0,0), qRgb(0xff, 0xff, 0xff));
    QCOMPARE(img.pixel(1,0), qRgb(0, 0, 0));
    QRgb gray = img.pixel(2,0);
    QVERIFY(qIsGray(gray));
    QVERIFY(qRed(gray) > 0x70 && qRed(gray) < 0x90 );

    if (hasColor) {
        QCOMPARE(img.pixel(0,1), qRgb(0xff, 0, 0));
        QCOMPARE(img.pixel(1,1), qRgb(0, 0xff, 0));
        QCOMPARE(img.pixel(2,1), qRgb(0, 0, 0xff));
    }
}

class Server : public QObject
{
    Q_OBJECT
public:
    Server(const QByteArray &data) :serverSocket(0)
    {
        connect(&server, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
        server.listen();
        this->data = data;
    }

public slots:
    void runTest()
    {
        connect(&clientSocket, SIGNAL(connected()), this, SLOT(connected()));
        clientSocket.connectToHost(QHostAddress::LocalHost, server.serverPort());
    }

public:
    inline BOBUIcpSocket *socket() const { return serverSocket; }

signals:
    void ready();

private slots:
    void acceptNewConnection()
    {
        serverSocket = server.nextPendingConnection();
        connect(serverSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
                this, SLOT(remoteHostClosed()));
    }

    void connected()
    {
        clientSocket.write(data);
        clientSocket.disconnectFromHost();
    }

    void remoteHostClosed()
    {
        emit ready();
    }

private:
    BOBUIcpServer server;
    BOBUIcpSocket clientSocket;
    BOBUIcpSocket *serverSocket;
    QByteArray data;
};

void tst_QImageReader::readFromDevice_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QByteArray>("format");

    BOBUIest::newRow("pbm") << QString("image.pbm") << QByteArray("pbm");
    BOBUIest::newRow("pgm") << QString("image.pgm") << QByteArray("pgm");
    BOBUIest::newRow("ppm-1") << QString("image.ppm") << QByteArray("ppm");
    BOBUIest::newRow("ppm-2") << QString("teapot.ppm") << QByteArray("ppm");
    BOBUIest::newRow("ppm-3") << QString("teapot.ppm") << QByteArray("ppm");
    BOBUIest::newRow("ppm-4") << QString("runners.ppm") << QByteArray("ppm");

    BOBUIest::newRow("jpeg-1") << QString("beavis.jpg") << QByteArray("jpeg");
    BOBUIest::newRow("jpeg-2") << QString("YCbCr_cmyk.jpg") << QByteArray("jpeg");
    BOBUIest::newRow("jpeg-3") << QString("YCbCr_rgb.jpg") << QByteArray("jpeg");
    BOBUIest::newRow("jpeg-4") << QString("bobuibug13653-no_eoi.jpg") << QByteArray("jpeg");

    BOBUIest::newRow("gif-1") << QString("earth.gif") << QByteArray("gif");
    BOBUIest::newRow("gif-2") << QString("trolltech.gif") << QByteArray("gif");

    BOBUIest::newRow("xbm") << QString("gnus.xbm") << QByteArray("xbm");
    BOBUIest::newRow("xpm") << QString("marble.xpm") << QByteArray("xpm");
    BOBUIest::newRow("bmp-1") << QString("colorful.bmp") << QByteArray("bmp");
    BOBUIest::newRow("bmp-2") << QString("font.bmp") << QByteArray("bmp");
    BOBUIest::newRow("bmp-3") << QString("test32bfv4.bmp") << QByteArray("bmp");
    BOBUIest::newRow("bmp-4") << QString("test32v5.bmp") << QByteArray("bmp");
    BOBUIest::newRow("png") << QString("kollada.png") << QByteArray("png");

    BOBUIest::newRow("svg") << QString("rect.svg") << QByteArray("svg");
    BOBUIest::newRow("svgz") << QString("rect.svgz") << QByteArray("svgz");
#if defined BOBUIEST_HAVE_TGA
    BOBUIest::newRow("tga") << QString("test-flag.tga") << QByteArray("tga");
#endif
}

static QByteArray msgReadFromDeviceFail(const QString &sourceFileName,
                                        const QByteArray &detectedFormat)
{
    QByteArray result = "Failure for '";
    result += sourceFileName.toLocal8Bit();
    result += "', detected as: '";
    result += detectedFormat;
    result += '\'';
    return result;
}

void tst_QImageReader::readFromDevice()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    const QString imageFileName = prefix + fileName;
    QImage expectedImage(imageFileName, format);
    QFile file(imageFileName);
    QVERIFY2(file.open(QFile::ReadOnly), msgFileOpenReadFailed(file).constData());
    QByteArray imageData = file.readAll();
    QVERIFY(!imageData.isEmpty());
    {
        QBuffer buffer;
        buffer.setData(imageData);
        QVERIFY(buffer.open(QBuffer::ReadOnly));

        QImageReader reader(&buffer);
        QVERIFY(reader.canRead());
        QImage imageReaderImage = reader.read();

        QVERIFY2(!imageReaderImage.isNull(), msgReadFromDeviceFail(imageFileName, reader.format()).constData());
        QCOMPARE(imageReaderImage, expectedImage);

        buffer.seek(0);

        QImage image1;
        QVERIFY(image1.loadFromData((const uchar *)buffer.data().data(),
                                    buffer.data().size(), format.data()));
        QCOMPARE(image1, expectedImage);

        QByteArray throughBase64 = QByteArray::fromBase64(imageData.toBase64());
        QVERIFY(image1.loadFromData((const uchar *)throughBase64.data(),
                                    throughBase64.size(), format.data()));
        QCOMPARE(image1, expectedImage);
    }

    Server server(imageData);
    QEventLoop loop;
    connect(&server, SIGNAL(ready()), &loop, SLOT(quit()));
    BOBUIimer::singleShot(0, &server, SLOT(runTest()));
    BOBUIimer::singleShot(5000, &loop, SLOT(quit()));
    loop.exec();

    QImageReader reader(server.socket(), format == "xbm" ? "xbm" : "");
    if (format == "xbm")
        QVERIFY(!reader.canRead());
    else
        QVERIFY(reader.canRead());
    QImage imageReaderImage = reader.read();
    QCOMPARE(imageReaderImage, expectedImage);
}

void tst_QImageReader::readFromFileAfterJunk_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QByteArray>("format");

    BOBUIest::newRow("pbm") << QString("image.pbm") << QByteArray("pbm");
    BOBUIest::newRow("pgm") << QString("image.pgm") << QByteArray("pgm");
    BOBUIest::newRow("ppm-1") << QString("image.ppm") << QByteArray("ppm");
    BOBUIest::newRow("ppm-2") << QString("teapot.ppm") << QByteArray("ppm");
    BOBUIest::newRow("ppm-3") << QString("teapot.ppm") << QByteArray("ppm");
    BOBUIest::newRow("ppm-4") << QString("runners.ppm") << QByteArray("ppm");

    BOBUIest::newRow("jpeg-1") << QString("beavis.jpg") << QByteArray("jpeg");
    BOBUIest::newRow("jpeg-2") << QString("YCbCr_cmyk.jpg") << QByteArray("jpeg");
    BOBUIest::newRow("jpeg-3") << QString("YCbCr_rgb.jpg") << QByteArray("jpeg");

    BOBUIest::newRow("xbm") << QString("gnus.xbm") << QByteArray("xbm");
    BOBUIest::newRow("xpm") << QString("marble.xpm") << QByteArray("xpm");
    BOBUIest::newRow("bmp-1") << QString("colorful.bmp") << QByteArray("bmp");
    BOBUIest::newRow("bmp-2") << QString("font.bmp") << QByteArray("bmp");
    BOBUIest::newRow("bmp-3") << QString("test32bfv4.bmp") << QByteArray("bmp");
    BOBUIest::newRow("bmp-4") << QString("test32v5.bmp") << QByteArray("bmp");
    BOBUIest::newRow("png") << QString("kollada.png") << QByteArray("png");
    BOBUIest::newRow("svg") << QString("rect.svg") << QByteArray("svg");
    BOBUIest::newRow("svgz") << QString("rect.svgz") << QByteArray("svgz");
}

void tst_QImageReader::readFromFileAfterJunk()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    BOBUIemporaryFile junkFile(m_temporaryDir.path() + QLatin1String("/junkXXXXXX"));
    QVERIFY2(junkFile.open(), msgFileOpenWriteFailed(junkFile).constData());

    QFile imageFile(prefix + fileName);
    QVERIFY2(imageFile.open(QFile::ReadOnly), msgFileOpenReadFailed(imageFile).constData());
    QByteArray imageData = imageFile.readAll();
    QVERIFY(!imageData.isNull());

    int iterations = 3;
    if (format == "ppm" || format == "pbm" || format == "pgm" || format == "svg" || format == "svgz")
        iterations = 1;

    if (!QImageWriter::supportedImageFormats().contains(format)) {
        for (int i = 0; i < iterations; ++i) {
            junkFile.write("deadbeef", 9);
            QCOMPARE(junkFile.write(imageData), qint64(imageData.size()));
        }
    } else {
        for (int i = 0; i < iterations; ++i) {
            QImageWriter writer(&junkFile, format);
            junkFile.write("deadbeef", 9);
            QVERIFY(writer.write(QImage(prefix + fileName)));
        }
    }
    junkFile.close();
    QVERIFY2(junkFile.open(), msgFileOpenReadFailed(junkFile).constData());

    for (int i = 0; i < iterations; ++i) {
        QByteArray ole = junkFile.read(9);
        junkFile.ungetChar(ole[ole.size() - 1]);
        char c;
        junkFile.getChar(&c);
        QImageReader reader(&junkFile);
        QVERIFY(reader.canRead());
        QVERIFY(!reader.read().isNull());
    }
}

void tst_QImageReader::devicePosition_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QByteArray>("format");

    BOBUIest::newRow("pbm") << QString("image.pbm") << QByteArray("pbm");
    BOBUIest::newRow("pgm") << QString("image.pgm") << QByteArray("pgm");
    BOBUIest::newRow("ppm-1") << QString("image.ppm") << QByteArray("ppm");

    BOBUIest::newRow("jpeg-1") << QString("beavis.jpg") << QByteArray("jpeg");
    BOBUIest::newRow("jpeg-2") << QString("YCbCr_cmyk.jpg") << QByteArray("jpeg");
    BOBUIest::newRow("jpeg-3") << QString("YCbCr_rgb.jpg") << QByteArray("jpeg");

    BOBUIest::newRow("gif-1") << QString("earth.gif") << QByteArray("gif");

    BOBUIest::newRow("xbm") << QString("gnus.xbm") << QByteArray("xbm");
    BOBUIest::newRow("xpm") << QString("marble.xpm") << QByteArray("xpm");
    BOBUIest::newRow("bmp-1") << QString("colorful.bmp") << QByteArray("bmp");
    BOBUIest::newRow("bmp-2") << QString("font.bmp") << QByteArray("bmp");
    BOBUIest::newRow("bmp-3") << QString("test32bfv4.bmp") << QByteArray("bmp");
    BOBUIest::newRow("bmp-4") << QString("test32v5.bmp") << QByteArray("bmp");
    BOBUIest::newRow("png") << QString("kollada.png") << QByteArray("png");
    BOBUIest::newRow("svg") << QString("rect.svg") << QByteArray("svg");
    BOBUIest::newRow("svgz") << QString("rect.svgz") << QByteArray("svgz");
}

void tst_QImageReader::devicePosition()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    QImage expected(prefix + fileName);
    QVERIFY(!expected.isNull());

    QFile imageFile(prefix + fileName);
    QVERIFY2(imageFile.open(QFile::ReadOnly), msgFileOpenReadFailed(imageFile).constData());
    QByteArray imageData = imageFile.readAll();
    QVERIFY(!imageData.isNull());
    const qint64 imageDataSize = imageData.size();

    const char *preStr = "prebeef\n";
    const qint64 preLen = qstrlen(preStr);
    imageData.prepend(preStr);
    if (format != "svg" && format != "svgz") // Doesn't handle trailing data
        imageData.append("\npostbeef");
    QBuffer buf(&imageData);
    buf.open(QIODevice::ReadOnly);
    buf.seek(preLen);
    QImageReader reader(&buf, format);
    QCOMPARE(expected, reader.read());
    if (format != "ppm" &&
        format != "pgm" &&
        format != "pbm" &&
        format != "gif")  // Known not to work
        QCOMPARE(buf.pos(), preLen + imageDataSize);
}


void tst_QImageReader::readFromResources_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QByteArray>("format");
    BOBUIest::addColumn<QSize>("size");
    BOBUIest::addColumn<QString>("message");

    BOBUIest::newRow("corrupt.bmp") << QString("corrupt.bmp")
                                        << QByteArray("bmp") << QSize(0, 0)
                                        << QString("");
    BOBUIest::newRow("negativeheight.bmp") << QString("negativeheight.bmp")
                                               << QByteArray("bmp") << QSize(127, 64)
                                               << QString("");
    BOBUIest::newRow("font.bmp") << QString("font.bmp")
                                     << QByteArray("bmp") << QSize(240, 8)
                                     << QString("");
    BOBUIest::newRow("noclearcode.bmp") << QString("noclearcode.bmp")
                                            << QByteArray("bmp") << QSize(29, 18)
                                            << QString("");
    BOBUIest::newRow("colorful.bmp") << QString("colorful.bmp")
                                         << QByteArray("bmp") << QSize(320, 200)
                                         << QString("");
    BOBUIest::newRow("16bpp.bmp") << QString("16bpp.bmp")
                                      << QByteArray("bmp") << QSize(320, 240)
                                      << QString("");
    BOBUIest::newRow("crash-signed-char.bmp") << QString("crash-signed-char.bmp")
                                                  << QByteArray("bmp") << QSize(360, 280)
                                                  << QString("");
    BOBUIest::newRow("4bpp-rle.bmp") << QString("4bpp-rle.bmp")
                                         << QByteArray("bmp") << QSize(640, 480)
                                         << QString("");
    BOBUIest::newRow("test32bfv4.bmp") << QString("test32bfv4.bmp")
                                         << QByteArray("bmp") << QSize(373, 156)
                                         << QString("");
    BOBUIest::newRow("test32v5.bmp") << QString("test32v5.bmp")
                                         << QByteArray("bmp") << QSize(373, 156)
                                         << QString("");
    BOBUIest::newRow("corrupt.gif") << QString("corrupt.gif")
                                        << QByteArray("gif") << QSize(0, 0)
                                        << QString("");
    BOBUIest::newRow("trolltech.gif") << QString("trolltech.gif")
                                          << QByteArray("gif") << QSize(128, 64)
                                          << QString("");
    BOBUIest::newRow("noclearcode.gif") << QString("noclearcode.gif")
                                            << QByteArray("gif") << QSize(29, 18)
                                            << QString("");
    BOBUIest::newRow("earth.gif") << QString("earth.gif")
                                      << QByteArray("gif") << QSize(320, 200)
                                      << QString("");
    BOBUIest::newRow("bat1.gif") << QString("bat1.gif")
                                     << QByteArray("gif") << QSize(32, 32)
                                     << QString("");
    BOBUIest::newRow("bat2.gif") << QString("bat2.gif")
                                     << QByteArray("gif") << QSize(32, 32)
                                     << QString("");
    BOBUIest::newRow("corrupt.jpg") << QString("corrupt.jpg")
                                        << QByteArray("jpg") << QSize(0, 0)
                                        << QString("JPEG datastream contains no image");
    BOBUIest::newRow("beavis.jpg") << QString("beavis.jpg")
                                       << QByteArray("jpg") << QSize(350, 350)
                                       << QString("");
    BOBUIest::newRow("YCbCr_cmyk.jpg") << QString("YCbCr_cmyk.jpg")
                                           << QByteArray("jpg") << QSize(75, 50)
                                           << QString("");
    BOBUIest::newRow("YCbCr_rgb.jpg") << QString("YCbCr_rgb.jpg")
                                          << QByteArray("jpg") << QSize(75, 50)
                                          << QString("");
    BOBUIest::newRow("bobuibug13653-no_eoi.jpg") << QString("bobuibug13653-no_eoi.jpg")
                                        << QByteArray("jpg") << QSize(240, 180)
                                        << QString("");
    BOBUIest::newRow("rect.svg") << QString("rect.svg")
                                     << QByteArray("svg") << QSize(128, 128)
                                     << QString("");
    BOBUIest::newRow("rect.svgz") << QString("rect.svgz")
                                     << QByteArray("svgz") << QSize(128, 128)
                                     << QString("");
    BOBUIest::newRow("corrupt.svg") << QString("corrupt.svg")
                                     << QByteArray("svg") << QSize(0, 0)
                                     << QString("");
    BOBUIest::newRow("corrupt.svgz") << QString("corrupt.svgz")
                                     << QByteArray("svgz") << QSize(0, 0)
                                     << QString("");
    BOBUIest::newRow("image.pbm") << QString("image.pbm")
                                      << QByteArray("pbm") << QSize(16, 6)
                                      << QString("");
    BOBUIest::newRow("image.pgm") << QString("image.pgm")
                                      << QByteArray("pgm") << QSize(24, 7)
                                      << QString("");
    BOBUIest::newRow("corrupt.png") << QString("corrupt.png")
                                        << QByteArray("png") << QSize(0, 0)
                                        << QString("");
    BOBUIest::newRow("away.png") << QString("away.png")
                                     << QByteArray("png") << QSize(16, 16)
                                     << QString("");
    BOBUIest::newRow("image.png") << QString("image.png")
                                      << QByteArray("png") << QSize(22, 22)
                                      << QString("");
    BOBUIest::newRow("kollada.png") << QString("kollada.png")
                                        << QByteArray("png") << QSize(436, 160)
                                        << QString("");
    BOBUIest::newRow("black.png") << QString("black.png")
                                      << QByteArray("png") << QSize(48, 48)
                                      << QString("");
    BOBUIest::newRow("YCbCr_cmyk.png") << QString("YCbCr_cmyk.png")
                                           << QByteArray("png") << QSize(75, 50)
                                           << QString("");
    BOBUIest::newRow("teapot.ppm") << QString("teapot.ppm")
                                       << QByteArray("ppm") << QSize(256, 256)
                                       << QString("");
    BOBUIest::newRow("image.ppm") << QString("image.ppm")
                                      << QByteArray("ppm") << QSize(4, 4)
                                      << QString("");
    BOBUIest::newRow("runners.ppm") << QString("runners.ppm")
                                        << QByteArray("ppm") << QSize(400, 400)
                                        << QString("");
    BOBUIest::newRow("test.ppm") << QString("test.ppm")
                                     << QByteArray("ppm") << QSize(10, 10)
                                     << QString("");
    BOBUIest::newRow("gnus.xbm") << QString("gnus.xbm")
                                     << QByteArray("xbm") << QSize(271, 273)
                                     << QString("");
    BOBUIest::newRow("corrupt-colors.xpm") << QString("corrupt-colors.xpm")
                                               << QByteArray("xpm") << QSize(0, 0)
                                               << QString("XPM color specification is missing: bla9an.n#x");
    BOBUIest::newRow("corrupt-pixels.xpm") << QString("corrupt-pixels.xpm")
                                               << QByteArray("xpm") << QSize(0, 0)
                                               << QString("XPM pixels missing on image line 3");
    BOBUIest::newRow("corrupt-pixel-count.xpm") << QString("corrupt-pixel-count.xpm")
                                             << QByteArray("xpm") << QSize(0, 0)
                                             << QString("");
    BOBUIest::newRow("marble.xpm") << QString("marble.xpm")
                                       << QByteArray("xpm") << QSize(240, 240)
                                       << QString("");
    BOBUIest::newRow("test.xpm") << QString("test.xpm")
                                     << QByteArray("xpm") << QSize(256, 256)
                                     << QString("");
    BOBUIest::newRow("black.xpm") << QString("black.xpm")
                                      << QByteArray("xpm") << QSize(48, 48)
                                      << QString("");
    BOBUIest::newRow("namedcolors.xpm") << QString("namedcolors.xpm")
                                            << QByteArray("xpm") << QSize(8, 8)
                                            << QString("");
    BOBUIest::newRow("nontransparent.xpm") << QString("nontransparent.xpm")
                                               << QByteArray("xpm") << QSize(8, 8)
                                               << QString("");
    BOBUIest::newRow("transparent.xpm") << QString("transparent.xpm")
                                            << QByteArray("xpm") << QSize(8, 8)
                                            << QString("");
}

void tst_QImageReader::readFromResources()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);
    QFETCH(QSize, size);
    QFETCH(QString, message);

    SKIP_IF_UNSUPPORTED(format);

    for (int i = 0; i < 2; ++i) {
        QString file = i ? QString(QStringLiteral(":/images/") + fileName) : QString(prefix + fileName);
        {
            // suppress warnings if we expect them
            if (!message.isEmpty()) {
                for (int j = 0; j < 5; ++j)
                    BOBUIest::ignoreMessage(BobUIWarningMsg, message.toLatin1());
            }

            // 1) full filename, no format
            QImageReader reader(file);
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
        {
            // 2) full filename, with format
            QImageReader reader(file, format);
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
        {
            // 3) full filename, with uppercase format
            QImageReader reader(file, format.toUpper());
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
        {
            // 4) chopped filename, with format
            QImageReader reader(file.left(file.lastIndexOf(QLatin1Char('.'))), format);
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
        {
            // 5) chopped filename, with uppercase format
            QImageReader reader(file.left(file.lastIndexOf(QLatin1Char('.'))), format.toUpper());
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
    }

    // Check that the results are identical
    if (!message.isEmpty()) {
        BOBUIest::ignoreMessage(BobUIWarningMsg, message.toLatin1());
        BOBUIest::ignoreMessage(BobUIWarningMsg, message.toLatin1());
    }
    QCOMPARE(QImageReader(prefix + fileName).read(), QImageReader(":/images/" + fileName).read());
}

void tst_QImageReader::readCorruptImage_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<bool>("shouldFail");
    BOBUIest::addColumn<QString>("message");
    BOBUIest::addColumn<QByteArray>("format");
    BOBUIest::newRow("corrupt jpeg") << QString("corrupt.jpg") << true
                                  << QString("JPEG datastream contains no image")
                                  << QByteArray("jpeg");
    BOBUIest::newRow("corrupt gif") << QString("corrupt.gif") << true << QString("") << QByteArray("gif");
    BOBUIest::newRow("corrupt png") << QString("corrupt.png") << true << QString("") << QByteArray("png");
    BOBUIest::newRow("corrupt bmp") << QString("corrupt.bmp") << true << QString("") << QByteArray("bmp");
    BOBUIest::newRow("corrupt bmp (clut)") << QString("corrupt_clut.bmp") << true << QString("") << QByteArray("bmp");
    BOBUIest::newRow("corrupt xpm (colors)") << QString("corrupt-colors.xpm") << true
                                          << QString("XPM color specification is missing: bla9an.n#x")
                                          << QByteArray("xpm");
    BOBUIest::newRow("corrupt xpm (pixels)") << QString("corrupt-pixels.xpm") << true
                                          << QString("XPM pixels missing on image line 3")
                                          << QByteArray("xpm");
    BOBUIest::newRow("corrupt xbm") << QString("corrupt.xbm") << false << QString("") << QByteArray("xbm");
    BOBUIest::newRow("corrupt svg") << QString("corrupt.svg") << true << QString("") << QByteArray("svg");
    BOBUIest::newRow("corrupt svgz") << QString("corrupt.svgz") << true << QString("") << QByteArray("svgz");
}

void tst_QImageReader::readCorruptImage()
{
    QFETCH(QString, fileName);
    QFETCH(bool, shouldFail);
    QFETCH(QString, message);
    QFETCH(QByteArray, format);

    SKIP_IF_UNSUPPORTED(format);

    if (!message.isEmpty())
        BOBUIest::ignoreMessage(BobUIWarningMsg, message.toLatin1());
    QImageReader reader(prefix + fileName);
    QVERIFY(reader.canRead());
    QCOMPARE(reader.read().isNull(), shouldFail);
}

void tst_QImageReader::readCorruptBmp()
{
    QCOMPARE(QImage(prefix + "tst7.bmp").convertToFormat(QImage::Format_ARGB32_Premultiplied), QImage(prefix + "tst7.png").convertToFormat(QImage::Format_ARGB32_Premultiplied));
}

void tst_QImageReader::supportsOption_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QIntList>("options");

    BOBUIest::newRow("png") << QString("black.png")
                         << QIntList{
                                QImageIOHandler::Gamma,
                                QImageIOHandler::Description,
                                QImageIOHandler::Quality,
                                QImageIOHandler::CompressionRatio,
                                QImageIOHandler::Size,
                                QImageIOHandler::ImageFormat,
                            };
}

void tst_QImageReader::supportsOption()
{
    QFETCH(QString, fileName);
    QFETCH(const QIntList, options);

    QImageReader reader(prefix + fileName);

    for (int i = 0; ; ++i) {
        // this switch ensures the compiler warns when we miss an enumerator [-Wswitch]
        // do _not_ add a default case!
        switch (const auto o = QImageIOHandler::ImageOption(i)) {
        case QImageIOHandler::Size:
        case QImageIOHandler::ClipRect:
        case QImageIOHandler::Description:
        case QImageIOHandler::ScaledClipRect:
        case QImageIOHandler::ScaledSize:
        case QImageIOHandler::CompressionRatio:
        case QImageIOHandler::Gamma:
        case QImageIOHandler::Quality:
        case QImageIOHandler::Name:
        case QImageIOHandler::SubType:
        case QImageIOHandler::IncrementalReading:
        case QImageIOHandler::Endianness:
        case QImageIOHandler::Animation:
        case QImageIOHandler::BackgroundColor:
        case QImageIOHandler::ImageFormat:
        case QImageIOHandler::SupportedSubTypes:
        case QImageIOHandler::OptimizedWrite:
        case QImageIOHandler::ProgressiveScanWrite:
        case QImageIOHandler::ImageTransformation:
            {
                auto printOnFailure = qScopeGuard([&] { qDebug("failed at %d", i); });
                QCOMPARE(reader.supportsOption(o), options.contains(i));
                printOnFailure.dismiss();
                continue; // ... as long as `i` represents a valid ImageOption value
            }
        }
        break; // ... once `i` no longer represents a valid ImageOption value
    }
}

void tst_QImageReader::autoDetectImageFormat()
{
    // Assume PNG is supported :-)
    {
        // Disables file name extension probing
        QImageReader reader(prefix + "kollada");
        reader.setAutoDetectImageFormat(false);
        QVERIFY(!reader.canRead());
        QVERIFY(reader.read().isNull());
        reader.setAutoDetectImageFormat(true);
        QVERIFY(reader.canRead());
        QVERIFY(!reader.read().isNull());
    }
    {
        // Disables detection based on suffix
        QImageReader reader(prefix + "kollada.png");
        reader.setAutoDetectImageFormat(false);
        QVERIFY(!reader.canRead());
        QVERIFY(reader.read().isNull());
        reader.setAutoDetectImageFormat(true);
        QVERIFY(reader.canRead());
        QVERIFY(!reader.read().isNull());
    }
    {
        // Disables detection based on content
        QImageReader reader(prefix + "kollada-noext");
        reader.setAutoDetectImageFormat(false);
        QVERIFY(!reader.canRead());
        QVERIFY(reader.read().isNull());
        reader.setAutoDetectImageFormat(true);
        QVERIFY(reader.canRead());
        QVERIFY(!reader.read().isNull());
    }

    if (QImageReader::supportedImageFormats().contains("jpeg")) {
        QImageReader io(prefix + "YCbCr_rgb.jpg");
        io.setAutoDetectImageFormat(false);
        // This should fail since no format string is given
        QImage image;
        QVERIFY(!io.read(&image));
    }
    if (QImageReader::supportedImageFormats().contains("jpeg")) {
        QImageReader io(prefix + "YCbCr_rgb.jpg", "jpg");
        io.setAutoDetectImageFormat(false);
        QImage image;
        QVERIFY(io.read(&image));
    }
    {
        QImageReader io(prefix + "tst7.png");
        io.setAutoDetectImageFormat(false);
        // This should fail since no format string is given
        QImage image;
        QVERIFY(!io.read(&image));
    }
    {
        QImageReader io(prefix + "tst7.png", "png");
        io.setAutoDetectImageFormat(false);
        QImage image;
        QVERIFY(io.read(&image));
    }
}

void tst_QImageReader::fileNameProbing()
{
    QString name("doesnotexist.png");
    QImageReader r;
    r.setFileName(name); // non-existing / non-readable file
    QCOMPARE(r.fileName(), name);

    r.size();
    QCOMPARE(r.fileName(), name);
    r.read();
    QCOMPARE(r.fileName(), name);
}

void tst_QImageReader::pixelCompareWithBaseline_data()
{
    BOBUIest::addColumn<QString>("fileName");

    BOBUIest::newRow("floppy (16px,32px - 16 colors)") << "35floppy.ico";
    BOBUIest::newRow("semitransparent") << "semitransparent.ico";
    BOBUIest::newRow("slightlybrokenBMPHeader") << "kde_favicon.ico";
    BOBUIest::newRow("sightlybrokenIconHeader") << "connect.ico";
}

void tst_QImageReader::pixelCompareWithBaseline()
{
    QFETCH(QString, fileName);

    static int enteredCount = 0;    // Used for better error diagnostics if something fails. We
    static int loadFailCount = 0;   // don't know if the reason load() fails is that the plugin
                                    // does not exist or because of a bug in the plugin. But if at
                                    // least one file succeeded we know that the plugin was built.
                                    // The other failures are then real failures.
    QImage icoImg;
    const QString inputFileName(QLatin1String("images/") + fileName);
    QFileInfo fi(inputFileName);

    ++enteredCount;
    // might fail if the plugin does not exist, which is ok.
    if (icoImg.load(inputFileName)) {
        icoImg = icoImg.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        const QString baselineFileName(QLatin1String("baseline/") + fi.baseName() + QLatin1String(".png"));
#if 0
        icoImg.save(baselineFileName);
#else
        QImage baseImg;
        QVERIFY(baseImg.load(baselineFileName));
        baseImg = baseImg.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        QCOMPARE(int(baseImg.format()), int(icoImg.format()));
        QCOMPARE(baseImg, icoImg);
#endif
    } else {
        ++loadFailCount;
        if (enteredCount != loadFailCount) {
            QFAIL("Plugin is built, but some did not load properly");
        } else {
            qWarning("loading failed, check if ico plugin is built");
        }
    }
}


void tst_QImageReader::testIgnoresFormatAndExtension_data()
{
    BOBUIest::addColumn<QString>("name");
    BOBUIest::addColumn<QString>("extension");
    BOBUIest::addColumn<QString>("expected");

    BOBUIest::newRow("black.png") << "black" << "png" << "png";
    BOBUIest::newRow("black.xpm") << "black" << "xpm" << "xpm";
    BOBUIest::newRow("colorful.bmp") << "colorful" << "bmp" << "bmp";
    BOBUIest::newRow("image.ppm") << "image" << "ppm" << "ppm";
    BOBUIest::newRow("image.pbm") << "image" << "pbm" << "pbm";
    BOBUIest::newRow("image.pgm") << "image" << "pgm" << "pgm";

    BOBUIest::newRow("bat1.gif") << "bat1" << "gif" << "gif";

    BOBUIest::newRow("beavis.jpg") << "beavis" << "jpg" << "jpeg";

    BOBUIest::newRow("rect.svg") << "rect" << "svg" << "svg";
    BOBUIest::newRow("rect.svgz") << "rect" << "svgz" << "svgz";
}

static QByteArray msgIgnoreFormatAndExtensionFail(const QString &sourceFileName,
                                                  const QString &targetFileName,
                                                  const QString &detectedFormat)
{
    QByteArray result = "Failure for '";
    result += sourceFileName.toLocal8Bit();
    result += "' as '";
    result += targetFileName.toLocal8Bit();
    result += "', detected as: '";
    result += detectedFormat.toLocal8Bit();
    result += '\'';
    return result;
}

void tst_QImageReader::testIgnoresFormatAndExtension()
{
    QFETCH(QString, name);
    QFETCH(QString, extension);
    QFETCH(QString, expected);

    SKIP_IF_UNSUPPORTED(expected.toLatin1());

    const QList<QByteArray> formats = QImageReader::supportedImageFormats();
    QString fileNameBase = prefix + name + QLatin1Char('.');
    QString tempPath = m_temporaryDir.path();
    if (!tempPath.endsWith(QLatin1Char('/')))
        tempPath += QLatin1Char('/');

    for (const QByteArray &f : formats) {
        if (f == extension.toLocal8Bit())
            continue;

        QFile tmp(tempPath + name + QLatin1Char('_') + expected + QLatin1Char('.') + f);
        const QString sourceFileName = fileNameBase + extension;
        const QString tempFileName = QFileInfo(tmp).absoluteFilePath();
        QVERIFY(QFile::copy(sourceFileName, tempFileName));

        QString format;
        QImage image;
        {
            // image reader needs to be scoped for the remove() to work..
            QImageReader r;
            r.setFileName(tempFileName);
            r.setDecideFormatFromContent(true);
            format = r.format();
            r.read(&image);
        }

        tmp.remove();

        QVERIFY2(!image.isNull(), msgIgnoreFormatAndExtensionFail(sourceFileName, tempFileName, format).constData());
        QCOMPARE(format, expected);
    }
}


void tst_QImageReader::saveFormat_data()
{
    BOBUIest::addColumn<QImage::Format>("format");

    BOBUIest::newRow("Format_Mono") << QImage::Format_Mono;
    BOBUIest::newRow("Format_MonoLSB") << QImage::Format_MonoLSB;
    BOBUIest::newRow("Format_Indexed8") << QImage::Format_Indexed8;
    BOBUIest::newRow("Format_RGB32") << QImage::Format_RGB32;
    BOBUIest::newRow("Format_ARGB32") << QImage::Format_ARGB32;
    BOBUIest::newRow("Format_ARGB32_Premultiplied") << QImage::Format_ARGB32_Premultiplied;
    BOBUIest::newRow("Format_RGB16") << QImage::Format_RGB16;
    BOBUIest::newRow("Format_ARGB8565_Premultiplied") << QImage::Format_ARGB8565_Premultiplied;
    BOBUIest::newRow("Format_RGB666") << QImage::Format_RGB666;
    BOBUIest::newRow("Format_ARGB6666_Premultiplied") << QImage::Format_ARGB6666_Premultiplied;
    BOBUIest::newRow("Format_RGB555") << QImage::Format_RGB555;
    BOBUIest::newRow("Format_ARGB8555_Premultiplied") << QImage::Format_ARGB8555_Premultiplied;
    BOBUIest::newRow("Format_RGB888") << QImage::Format_RGB888;
    BOBUIest::newRow("Format_BGR888") << QImage::Format_BGR888;
    BOBUIest::newRow("Format_RGB444") << QImage::Format_RGB444;
    BOBUIest::newRow("Format_ARGB4444_Premultiplied") << QImage::Format_ARGB4444_Premultiplied;
    BOBUIest::newRow("Format_RGBA64") << QImage::Format_RGBA64;
    BOBUIest::newRow("Format_RGBA64_Premultiplied") << QImage::Format_RGBA64_Premultiplied;
}

void tst_QImageReader::saveFormat()
{
    QFETCH(QImage::Format, format);

    QImage orig(":/images/kollada.png");

    QImage converted = orig.convertToFormat(format);
    QBuffer buf;
    buf.open(QIODevice::WriteOnly);
    QVERIFY(converted.save(&buf, "png"));
    buf.close();
    QImage stored = QImage::fromData(buf.buffer(), "png");

    stored = stored.convertToFormat(QImage::Format_ARGB32);
    converted = converted.convertToFormat(QImage::Format_ARGB32);
    QCOMPARE(stored, converted);
}

void tst_QImageReader::saveColorSpace_data()
{
    BOBUIest::addColumn<QColorSpace::NamedColorSpace>("namedColorSpace");

    BOBUIest::newRow("sRGB")         << QColorSpace::SRgb;
    BOBUIest::newRow("sRGB(linear)") << QColorSpace::SRgbLinear;
    BOBUIest::newRow("AdobeRGB")     << QColorSpace::AdobeRgb;
    BOBUIest::newRow("DisplayP3")    << QColorSpace::DisplayP3;
    BOBUIest::newRow("ProPhotoRgb")  << QColorSpace::ProPhotoRgb;
}

void tst_QImageReader::saveColorSpace()
{
    QFETCH(QColorSpace::NamedColorSpace, namedColorSpace);

    QImage orig(":/images/kollada.png");

    orig.setColorSpace(namedColorSpace);
    QBuffer buf;
    buf.open(QIODevice::WriteOnly);
    QVERIFY(orig.save(&buf, "png"));
    buf.close();
    QImage stored = QImage::fromData(buf.buffer(), "png");

    QCOMPARE(stored, orig);
    QCOMPARE(stored.colorSpace(), orig.colorSpace());

    buf.open(QIODevice::WriteOnly);
    QVERIFY(orig.save(&buf, "jpeg"));
    buf.close();
    stored = QImage::fromData(buf.buffer(), "jpeg");

    QCOMPARE(stored.colorSpace(), orig.colorSpace());
}

void tst_QImageReader::readText_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QString>("key");
    BOBUIest::addColumn<QString>("text");

    BOBUIest::newRow("png, tEXt before img") << "txts.png" << "Title" << "PNG";
    BOBUIest::newRow("png, zTXt before img") << "txts.png" << "Comment" << "Some compressed text.";
    BOBUIest::newRow("png, tEXt after img") << "txts.png" << "Disclaimer" << "For testing only.";
    BOBUIest::newRow("png, zTXt after img") << "txts.png" << "Description" << "Rendered by Persistence of Vision (tm) Ray Tracer";

    BOBUIest::newRow("jpg, JPEG_COM Title") << "txts.jpg" << "Title" << "JPG";
    BOBUIest::newRow("jpg, JPEG_COM Comment") << "txts.jpg" << "Comment" << "Some non-compressed text.";
    BOBUIest::newRow("jpg, JPEG_COM Disclaimer") << "txts.jpg" << "Disclaimer" << "For testing only.";
    BOBUIest::newRow("jpg, JPEG_COM Description") << "txts.jpg" << "Description" << "Rendered by Persistence of Vision (tm) Ray Tracer";
}


void tst_QImageReader::readText()
{
#ifdef BOBUI_NO_IMAGEIO_TEXT_LOADING
    QSKIP("Reading text from image is configured away");
#endif

    QFETCH(QString, fileName);
    QFETCH(QString, key);
    QFETCH(QString, text);

    QImage img(prefix + fileName);
    QVERIFY(img.textKeys().contains(key));
    QCOMPARE(img.text(key), text);
}


void tst_QImageReader::preserveTexts_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QString>("text");

    QString latin1set;
    for (int c = 0x20; c <= 0x7e; c++)
        latin1set.append(QLatin1Char(c));
    for (int c = 0xa0; c <= 0xff; c++)
        latin1set.append(QLatin1Char(c));

    const QList<QLatin1StringView> fileNames{
        QLatin1StringView(":/images/kollada.png"),
        QLatin1StringView(":/images/txts.jpg")
        // Common prefix of length 9 before file names: ":/images/", skipped below by + 9.
    };
    for (const auto &fileName : fileNames) {
        BOBUIest::addRow("Simple %s", fileName.data() + 9)
            << QString(fileName) << "simpletext";
        BOBUIest::addRow("Whitespace %s", fileName.data() + 9)
            << QString(fileName) << " A text  with whitespace ";
        BOBUIest::addRow("Newline %s", fileName.data() + 9)
            << QString(fileName) << "A text\nwith newlines\n";
        BOBUIest::addRow("Double newlines %s", fileName.data() + 9)
            << QString(fileName) << "A text\n\nwith double newlines\n\n";
        BOBUIest::addRow("Long %s", fileName.data() + 9)
            << QString(fileName)
            << QString("A rather long text, at least after many repetitions. ").repeated(100);
        BOBUIest::addRow("All Latin1 chars %s", fileName.data() + 9)
            << QString(fileName) << latin1set;
#if 0
        // Depends on iTXt support in libpng
        BOBUIest::addRow("Multibyte string %s", fileName.data() + 9)
            << QString(fileName)
            << QString::fromUtf8("\341\233\222\341\233\226\341\232\251\341\232"
                                 "\271\341\232\242\341\233\232\341\232\240");
#endif
    }
}


void tst_QImageReader::preserveTexts()
{
#ifdef BOBUI_NO_IMAGEIO_TEXT_LOADING
    QSKIP("Reading text from image is configured away");
#endif

    QFETCH(QString, fileName);
    QByteArray format = fileName.right(3).toLatin1();
    QFETCH(QString, text);
    QString key("testkey");
    QString key2("testkey2");
    QString text2("Some other text.");
    QString key3("testkey3");
    QString text3("Some more other text.");

    QImage img(fileName);
    img.setText(key, text);
    img.setText(key2, text2);
    QBuffer buf;
    buf.open(QIODevice::WriteOnly);
    QVERIFY(img.save(&buf, format.constData()));
    buf.close();
    QImage stored = QImage::fromData(buf.buffer(), format.constData());
    QCOMPARE(stored.text(key), text);
    QCOMPARE(stored.text(key2), text2);

    QImage img2(fileName);
    img2.setText(key3, text3);
    QBuffer buf2;
    QImageWriter w(&buf2, format);
    w.setText(key, text);
    w.setText(key2, text2);
    QVERIFY(w.write(img2));
    buf2.close();
    QImageReader r(&buf2, format);
    QCOMPARE(r.text(key), text.simplified());
    QCOMPARE(r.text(key2), text2.simplified());
    QCOMPARE(r.text(key3), text3.simplified());
}

void tst_QImageReader::devicePixelRatio_data()
{
    BOBUIest::addColumn<QString>("fileName");
    BOBUIest::addColumn<QSize>("size");
    BOBUIest::addColumn<qreal>("dpr");

    BOBUIest::newRow("1x") << "bobuiicon16.png" << QSize(16, 16) << 1.0;
    BOBUIest::newRow("2x") << "bobuiicon16@2x.png" << QSize(32, 32) << 2.0;
    BOBUIest::newRow("3x") << "bobuiicon16@3x.png" << QSize(48, 48) << 3.0;
}

void tst_QImageReader::devicePixelRatio()
{
    QFETCH(QString, fileName);
    QFETCH(QSize, size);
    QFETCH(qreal, dpr);

    QImageReader r(":/images/" + fileName);
    QImage img = r.read();
    QCOMPARE(img.size(), size);
    QCOMPARE(img.devicePixelRatio(), dpr);
}

void tst_QImageReader::xpmBufferOverflow()
{
    // Please note that the overflow only showed when BobUI was configured with "-sanitize address".
    QImageReader(":/images/oss-fuzz-23988.xpm").read();
}

void tst_QImageReader::xbmBufferHandling()
{
    uint8_t original_buffer[256];
    for (int i = 0; i < 256; ++i)
        original_buffer[i] = i;

    QImage image(original_buffer, 256, 8, QImage::Format_MonoLSB);
    image.setColorTable({0xff000000, 0xffffffff});

    QByteArray buffer;
    {
        QBuffer buf(&buffer);
        QImageWriter writer(&buf, "xbm");
        writer.write(image);
    }

    QCOMPARE(QImage::fromData(buffer, "xbm"), image);

    auto i = buffer.indexOf(',');
    buffer.insert(i + 1, "                                                                                ");
    QCOMPARE(QImage::fromData(buffer, "xbm"), image);
    buffer.insert(i + 1, "                                                                                ");
    QCOMPARE(QImage::fromData(buffer, "xbm"), image);
    buffer.insert(i + 1, "                                                                              ");
#if 0   // Lines longer than 300 chars not supported currently
    QCOMPARE(QImage::fromData(buffer, "xbm"), image);
#endif

    i = buffer.lastIndexOf("\n ");
    buffer.truncate(i + 1);
    buffer.append(QByteArray(297, ' '));
    buffer.append("0x");
    // Only check we get no buffer overflow
    QImage::fromData(buffer, "xbm");
}

BOBUIEST_MAIN(tst_QImageReader)
#include "tst_qimagereader.moc"
