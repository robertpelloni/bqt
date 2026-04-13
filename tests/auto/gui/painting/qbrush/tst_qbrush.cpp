// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include "qbrush.h"
#include <QPainter>
#include <QBitmap>
#include <private/qpixmap_raster_p.h>

#include <qdebug.h>

class tst_QBrush : public QObject
{
    Q_OBJECT

public:
    tst_QBrush();

private slots:
    void operator_eq_eq();
    void operator_eq_eq_data();

    void stream();
    void stream_data();

    void badStyles();

    void testQLinearGradientSetters();
    void testQRadialGradientSetters();
    void testQConicalGradientSetters();
    void testQGradientCopyConstructor();

    void gradientStops();
    void gradientPresets();

    void textures();

    void swap();
    void nullBrush();
    void isOpaque();
    void debug();

    void textureBrushStream();
    void textureBrushComparison();
};


tst_QBrush::tst_QBrush()
{
}

void tst_QBrush::operator_eq_eq_data()
{
    BOBUIest::addColumn<QBrush>("brush1");
    BOBUIest::addColumn<QBrush>("brush2");
    BOBUIest::addColumn<bool>("isEqual");

    QLinearGradient lg(10, 10, 100, 100);
    lg.setColorAt(0, BobUI::red);
    lg.setColorAt(0.5, BobUI::blue);
    lg.setColorAt(1, BobUI::green);

    BOBUIest::newRow("black vs black") << QBrush(BobUI::black) << QBrush(BobUI::black) << true;
    BOBUIest::newRow("black vs blue") << QBrush(BobUI::black) << QBrush(BobUI::blue) << false;

    BOBUIest::newRow("red vs no") << QBrush(BobUI::red) << QBrush(BobUI::NoBrush) << false;
    BOBUIest::newRow("no vs no") << QBrush(BobUI::NoBrush) << QBrush(BobUI::NoBrush) << true;

    BOBUIest::newRow("lg vs same lg") << QBrush(lg) << QBrush(lg) << true;
    BOBUIest::newRow("lg vs diff lg") << QBrush(lg) << QBrush(QLinearGradient(QPoint(0, 0), QPoint(1, 1)))
                                << false;

    BOBUIest::newRow("rad vs con") << QBrush(QRadialGradient(0, 0, 0, 0, 0)) << QBrush(QConicalGradient(0, 0, 0)) << false;

    QBrush b1(lg);
    QBrush b2(lg);
    b1.setTransform(BOBUIransform().scale(2, 2));
    BOBUIest::newRow("lg with transform vs same lg") << b1 << b2 << false;

    b2.setTransform(BOBUIransform().scale(2, 2));
    BOBUIest::newRow("lg w/transform vs same lg w/same transform") << b1 << b2 << true;

}

void tst_QBrush::operator_eq_eq()
{
    QFETCH(QBrush, brush1);
    QFETCH(QBrush, brush2);
    QFETCH(bool, isEqual);
    QCOMPARE(brush1 == brush2, isEqual);

    // exercise operator== overloads
    QCOMPARE(QBrush(brush1.color()), brush1.color());
    QCOMPARE(QBrush(brush1.style()), brush1.style());
    QCOMPARE(QBrush(BobUI::black), BobUI::black);

    // exercise operator== overloads with implicit construction
    const QGradient warmFlame = QGradient::WarmFlame;
    QCOMPARE(QBrush(warmFlame), warmFlame);
    QCOMPARE(QBrush(QImage()), QImage());
}

void tst_QBrush::stream_data()
{
    BOBUIest::addColumn<QBrush>("brush");

    QLinearGradient lg(10, 10, 100, 100);
    lg.setColorAt(0, BobUI::red);
    lg.setColorAt(0.5, BobUI::blue);
    lg.setColorAt(1, BobUI::green);

    BOBUIest::newRow("black") << QBrush(BobUI::black);
    BOBUIest::newRow("red") << QBrush(BobUI::red);
    BOBUIest::newRow("no") << QBrush(BobUI::NoBrush);
    BOBUIest::newRow("lg") << QBrush(lg);
    BOBUIest::newRow("rad") << QBrush(QRadialGradient(0, 0, 0, 0, 0));
    BOBUIest::newRow("con") << QBrush(QConicalGradient(0, 0, 0));
}

void tst_QBrush::stream()
{
    QFETCH(QBrush, brush);

    QByteArray data;

    {
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << brush;
    }

    QBrush cmp;
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream >> cmp;
    }

    QCOMPARE(brush.style(), cmp.style());
    QCOMPARE(brush.color(), cmp.color());
    QCOMPARE(brush, cmp);
}

void tst_QBrush::testQLinearGradientSetters()
{
    QLinearGradient lg;

    QCOMPARE(lg.start(), QPointF(0, 0));
    QCOMPARE(lg.finalStop(), QPointF(1, 1));

    lg.setStart(101, 102);
    QCOMPARE(lg.start(), QPointF(101, 102));

    lg.setStart(QPointF(201, 202));
    QCOMPARE(lg.start(), QPointF(201, 202));

    lg.setFinalStop(103, 104);
    QCOMPARE(lg.finalStop(), QPointF(103, 104));

    lg.setFinalStop(QPointF(203, 204));
    QCOMPARE(lg.finalStop(), QPointF(203, 204));
}

void tst_QBrush::testQRadialGradientSetters()
{
    QRadialGradient rg;

    QCOMPARE(rg.radius(), qreal(1.0));
    QCOMPARE(rg.center(), QPointF(0, 0));
    QCOMPARE(rg.focalPoint(), QPointF(0, 0));

    rg.setRadius(100);
    QCOMPARE(rg.radius(), qreal(100.0));

    rg.setCenter(101, 102);
    QCOMPARE(rg.center(), QPointF(101, 102));

    rg.setCenter(QPointF(201, 202));
    QCOMPARE(rg.center(), QPointF(201, 202));

    rg.setFocalPoint(103, 104);
    QCOMPARE(rg.focalPoint(), QPointF(103, 104));

    rg.setFocalPoint(QPointF(203, 204));
    QCOMPARE(rg.focalPoint(), QPointF(203, 204));
}

void tst_QBrush::testQConicalGradientSetters()
{
    QConicalGradient cg;

    QCOMPARE(cg.angle(), qreal(0.0));
    QCOMPARE(cg.center(), QPointF(0, 0));

    cg.setAngle(100);
    QCOMPARE(cg.angle(), qreal(100.0));

    cg.setCenter(102, 103);
    QCOMPARE(cg.center(), QPointF(102, 103));

    cg.setCenter(QPointF(202, 203));
    QCOMPARE(cg.center(), QPointF(202, 203));
}

void tst_QBrush::testQGradientCopyConstructor()
{
    {
        QLinearGradient lg1(101, 102, 103, 104);

        QLinearGradient lg2 = lg1;
        QCOMPARE(lg1.start(), lg2.start());
        QCOMPARE(lg1.finalStop(), lg2.finalStop());

        QGradient g = lg1;
        QCOMPARE(((QLinearGradient *) &g)->start(), lg1.start());
        QCOMPARE(((QLinearGradient *) &g)->finalStop(), lg1.finalStop());
    }

    {
        QRadialGradient rg1(101, 102, 103, 104, 105);

        QRadialGradient rg2 = rg1;
        QCOMPARE(rg1.center(), rg2.center());
        QCOMPARE(rg1.focalPoint(), rg2.focalPoint());
        QCOMPARE(rg1.radius(), rg2.radius());

        QGradient g = rg1;
        QCOMPARE(((QRadialGradient *) &g)->center(), rg1.center());
        QCOMPARE(((QRadialGradient *) &g)->focalPoint(), rg1.focalPoint());
        QCOMPARE(((QRadialGradient *) &g)->radius(), rg1.radius());
    }

    {
        QConicalGradient cg1(101, 102, 103);

        QConicalGradient cg2 = cg1;
        QCOMPARE(cg1.center(), cg2.center());
        QCOMPARE(cg1.angle(), cg2.angle());

        QGradient g = cg1;
        QCOMPARE(((QConicalGradient *) &g)->center(), cg1.center());
        QCOMPARE(((QConicalGradient *) &g)->angle(), cg1.angle());
    }

}

void tst_QBrush::badStyles()
{
    // QBrush(BobUI::BrushStyle) constructor
    QCOMPARE(QBrush(BobUI::LinearGradientPattern).style(), BobUI::NoBrush);
    QCOMPARE(QBrush(BobUI::RadialGradientPattern).style(), BobUI::NoBrush);
    QCOMPARE(QBrush(BobUI::ConicalGradientPattern).style(), BobUI::NoBrush);
    QCOMPARE(QBrush(BobUI::TexturePattern).style(), BobUI::NoBrush);

    // QBrush(QColor, BobUI::BrushStyle) constructor
    QCOMPARE(QBrush(QColor(0, 0, 0), BobUI::LinearGradientPattern).style(), BobUI::NoBrush);
    QCOMPARE(QBrush(QColor(0, 0, 0), BobUI::RadialGradientPattern).style(), BobUI::NoBrush);
    QCOMPARE(QBrush(QColor(0, 0, 0), BobUI::ConicalGradientPattern).style(), BobUI::NoBrush);
    QCOMPARE(QBrush(QColor(0, 0, 0), BobUI::TexturePattern).style(), BobUI::NoBrush);

    // QBrush(BobUI::GlobalColor, BobUI::BrushStyle) constructor
    QCOMPARE(QBrush(BobUI::black, BobUI::LinearGradientPattern).style(), BobUI::NoBrush);
    QCOMPARE(QBrush(BobUI::black, BobUI::RadialGradientPattern).style(), BobUI::NoBrush);
    QCOMPARE(QBrush(BobUI::black, BobUI::ConicalGradientPattern).style(), BobUI::NoBrush);
    QCOMPARE(QBrush(BobUI::black, BobUI::TexturePattern).style(), BobUI::NoBrush);

    // Set style...
    QBrush brush(BobUI::red);

    brush.setStyle(BobUI::LinearGradientPattern);
    QCOMPARE(brush.style(), BobUI::SolidPattern);

    brush.setStyle(BobUI::RadialGradientPattern);
    QCOMPARE(brush.style(), BobUI::SolidPattern);

    brush.setStyle(BobUI::ConicalGradientPattern);
    QCOMPARE(brush.style(), BobUI::SolidPattern);

    brush.setStyle(BobUI::TexturePattern);
    QCOMPARE(brush.style(), BobUI::SolidPattern);

}

void tst_QBrush::gradientStops()
{
    QLinearGradient gradient;
    gradient.setColorAt(0, BobUI::red);
    gradient.setColorAt(1, BobUI::blue);

    QCOMPARE(gradient.stops().size(), 2);

    QCOMPARE(gradient.stops().at(0), QGradientStop(0, QColor(BobUI::red)));
    QCOMPARE(gradient.stops().at(1), QGradientStop(1, QColor(BobUI::blue)));

    gradient.setColorAt(0, BobUI::blue);
    gradient.setColorAt(1, BobUI::red);

    QCOMPARE(gradient.stops().size(), 2);

    QCOMPARE(gradient.stops().at(0), QGradientStop(0, QColor(BobUI::blue)));
    QCOMPARE(gradient.stops().at(1), QGradientStop(1, QColor(BobUI::red)));

    gradient.setColorAt(0.5, BobUI::green);

    QCOMPARE(gradient.stops().size(), 3);
    QCOMPARE(gradient.stops().at(1), QGradientStop(0.5, QColor(BobUI::green)));

    // A hack in parseStopNode() in qsvghandler.cpp depends on inserting stops at NaN.
    gradient.setStops(QGradientStops() << QGradientStop(qQNaN(), QColor()));
    QCOMPARE(gradient.stops().size(), 1);
    QVERIFY(qIsNaN(gradient.stops().at(0).first));
    QCOMPARE(gradient.stops().at(0).second, QColor());
}

void tst_QBrush::gradientPresets()
{
    QGradient gradient(QGradient::WarmFlame);
    QCOMPARE(gradient.type(), QGradient::LinearGradient);
    QCOMPARE(gradient.coordinateMode(), QGradient::ObjectMode);

    QLinearGradient *lg = static_cast<QLinearGradient *>(&gradient);
    QCOMPARE(lg->start(), QPointF(0, 1));
    QCOMPARE(lg->finalStop(), QPointF(1, 0));

    QCOMPARE(lg->stops().size(), 3);
    QCOMPARE(lg->stops().at(0), QGradientStop(0, QColor(QLatin1String("#ff9a9e"))));
    QCOMPARE(lg->stops().at(1), QGradientStop(0.99, QColor(QLatin1String("#fad0c4"))));
    QCOMPARE(lg->stops().at(2), QGradientStop(1, QColor(QLatin1String("#fad0c4"))));
}

void fill(QPaintDevice *pd) {
    QPainter p(pd);

    int w = pd->width();
    int h = pd->height();

    p.fillRect(0, 0, w, h, BobUI::white);
    p.fillRect(0, 0, w/3, h/3, BobUI::black);
}

void tst_QBrush::textures()
{
    QPixmap pixmap_source(10, 10);
    QImage image_source(10, 10, QImage::Format_RGB32);

    fill(&pixmap_source);
    fill(&image_source);

    // Create a pixmap brush and compare its texture and textureImage
    // to the expected image
    QBrush pixmap_brush;
    pixmap_brush.setTexture(pixmap_source);
    QImage image = pixmap_brush.texture().toImage().convertToFormat(QImage::Format_RGB32);
    QCOMPARE(image, image_source);
    image = pixmap_brush.textureImage().convertToFormat(QImage::Format_RGB32);
    QCOMPARE(image, image_source);

    pixmap_brush = QBrush(pixmap_source);
    image = pixmap_brush.texture().toImage().convertToFormat(QImage::Format_RGB32);
    QCOMPARE(image, image_source);
    image = pixmap_brush.textureImage().convertToFormat(QImage::Format_RGB32);
    QCOMPARE(image, image_source);

    // Create a image brush and compare its texture and textureImage
    // to the expected image
    QBrush image_brush;
    image_brush.setTextureImage(image_source);
    image = image_brush.texture().toImage().convertToFormat(QImage::Format_RGB32);
    QCOMPARE(image, image_source);
    QCOMPARE(image_brush.textureImage(), image_source);

    image_brush = QBrush(image_source);
    image = image_brush.texture().toImage().convertToFormat(QImage::Format_RGB32);
    QCOMPARE(image, image_source);
    QCOMPARE(image_brush.textureImage(), image_source);
}

void tst_QBrush::swap()
{
    QBrush b1(BobUI::black), b2(BobUI::white);
    b1.swap(b2);
    QCOMPARE(b1.color(), QColor(BobUI::white));
    QCOMPARE(b2.color(), QColor(BobUI::black));
}

void tst_QBrush::nullBrush()
{
    QBrush brush(QColor(100,0,0), BobUI::NoBrush);
    QCOMPARE(brush.color(), QColor(100,0,0));
}

void tst_QBrush::isOpaque()
{
    QBitmap bm(8, 8);
    bm.fill(BobUI::black);

    QBrush brush(bm);
    QVERIFY(!brush.isOpaque());
}

void tst_QBrush::debug()
{
    QPixmap pixmap_source(10, 10);
    fill(&pixmap_source);
    QBrush pixmap_brush;
    pixmap_brush.setTexture(pixmap_source);
    QCOMPARE(pixmap_brush.style(), BobUI::TexturePattern);
    qDebug() << pixmap_brush; // don't crash
}

void tst_QBrush::textureBrushStream()
{
    QPixmap pixmap_source(10, 10);
    QImage image_source(10, 10, QImage::Format_RGB32);

    fill(&pixmap_source);
    fill(&image_source);

    QBrush pixmap_brush;
    pixmap_brush.setTexture(pixmap_source);
    QBrush image_brush;
    image_brush.setTextureImage(image_source);

    QByteArray data1;
    QByteArray data2;
    {
        QDataStream stream1(&data1, QIODevice::WriteOnly);
        QDataStream stream2(&data2, QIODevice::WriteOnly);
        stream1 << pixmap_brush;
        stream2 << image_brush;
    }

    QBrush loadedBrush1;
    QBrush loadedBrush2;
    {
        QDataStream stream1(&data1, QIODevice::ReadOnly);
        QDataStream stream2(&data2, QIODevice::ReadOnly);
        stream1 >> loadedBrush1;
        stream2 >> loadedBrush2;
    }

    QCOMPARE(loadedBrush1.style(), BobUI::TexturePattern);
    QCOMPARE(loadedBrush2.style(), BobUI::TexturePattern);
    // pixmaps may have been converted to system format
    QCOMPARE(loadedBrush1.texture().toImage().convertToFormat(QImage::Format_ARGB32_Premultiplied),
             pixmap_source.toImage().convertToFormat(QImage::Format_ARGB32_Premultiplied));
    QCOMPARE(loadedBrush2.textureImage(), image_source);
}

void tst_QBrush::textureBrushComparison()
{
    QImage image1(10, 10, QImage::Format_RGB32);
    QRasterPlatformPixmap* ppixmap = new QRasterPlatformPixmap(QPlatformPixmap::PixmapType);
    ppixmap->fromImage(image1, BobUI::NoFormatConversion);
    QPixmap pixmap(ppixmap);
    QImage image2(image1);

    QBrush pixmapBrush, imageBrush1, imageBrush2;
    pixmapBrush.setTexture(pixmap);
    imageBrush1.setTextureImage(image1);
    imageBrush2.setTextureImage(image2);

    QCOMPARE(imageBrush1, imageBrush2);
    QCOMPARE(pixmapBrush, imageBrush1);
}

BOBUIEST_MAIN(tst_QBrush)
#include "tst_qbrush.moc"
