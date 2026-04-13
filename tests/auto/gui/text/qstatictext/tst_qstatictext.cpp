// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/QSet>
#include <BobUIGui/QGuiApplication>
#include <BobUIGui/QPainter>
#include <BobUIGui/QImage>

#include <qstatictext.h>
#include <qpaintengine.h>

#ifdef BOBUI_BUILD_INTERNAL
#include <private/qstatictext_p.h>
#endif

// #define DEBUG_SAVE_IMAGE

static inline QImage blankSquare()
{
    // a "blank" square; we compare against in our testfunctions to verify
    // that we have actually painted something
    QPixmap pm(1000, 1000);
    pm.fill(BobUI::white);
    return pm.toImage();
}

class tst_QStaticText: public QObject
{
    Q_OBJECT
public:
    tst_QStaticText() : m_whiteSquare(blankSquare()) {}

private slots:
    void constructionAndDestruction();
    void drawToPoint_data();
    void drawToPoint();
    void drawToRect_data();
    void drawToRect();
    void compareToDrawText_data();
    void compareToDrawText();
    void setFont();
    void setTextWidth();
    void prepareToCorrectData();
    void prepareToWrongData();

    void copyConstructor();

    void translatedPainter();
    void rotatedPainter();
    void scaledPainter();
    void projectedPainter();
    void transformationChanged();

    void plainTextVsRichText();

    void setPenPlainText_data();
    void setPenPlainText();
    void setPenRichText();
    void richTextOverridesPen();

    void unprintableCharacter_bobuibug12614();

#ifdef BOBUI_BUILD_INTERNAL
    void underlinedColor_bobuibug20159();
    void textDocumentColor();
#endif

    void multiLine();

    void size_bobuibug65836();

private:
    bool supportsTransformations() const;

    const QImage m_whiteSquare;
};

Q_DECLARE_METATYPE(QImage::Format);

void tst_QStaticText::constructionAndDestruction()
{
    QStaticText text("My text");
}

void tst_QStaticText::copyConstructor()
{
    QStaticText text(QLatin1String("My text"));

    BOBUIextOption textOption(BobUI::AlignRight);
    text.setTextOption(textOption);

    text.setPerformanceHint(QStaticText::AggressiveCaching);
    text.setTextWidth(123.456);
    text.setTextFormat(BobUI::PlainText);

    QStaticText copiedText(text);
    copiedText.setText(QLatin1String("Other text"));

    QCOMPARE(copiedText.textOption().alignment(), BobUI::AlignRight);
    QCOMPARE(copiedText.performanceHint(), QStaticText::AggressiveCaching);
    QCOMPARE(copiedText.textWidth(), 123.456);
    QCOMPARE(copiedText.textFormat(), BobUI::PlainText);

    QStaticText otherCopiedText(copiedText);
    otherCopiedText.setTextWidth(789);

    QCOMPARE(otherCopiedText.text(), QString::fromLatin1("Other text"));
}

Q_DECLARE_METATYPE(QStaticText::PerformanceHint)
void tst_QStaticText::drawToPoint_data()
{
    BOBUIest::addColumn<QStaticText::PerformanceHint>("performanceHint");

    BOBUIest::newRow("Moderate caching") << QStaticText::ModerateCaching;
    BOBUIest::newRow("Aggressive caching") << QStaticText::AggressiveCaching;
}

void tst_QStaticText::drawToPoint()
{
    QFETCH(QStaticText::PerformanceHint, performanceHint);

    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.drawText(11, 12, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);
        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextFormat(BobUI::PlainText);
        text.setPerformanceHint(performanceHint);
        p.drawStaticText(QPointF(11, 12 - QFontMetricsF(p.font()).ascent()), text);
    }

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}

void tst_QStaticText::drawToRect_data()
{
    BOBUIest::addColumn<QStaticText::PerformanceHint>("performanceHint");

    BOBUIest::newRow("Moderate caching") << QStaticText::ModerateCaching;
    BOBUIest::newRow("Aggressive caching") << QStaticText::AggressiveCaching;
}

void tst_QStaticText::drawToRect()
{
    QFETCH(QStaticText::PerformanceHint, performanceHint);

    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.drawText(QRectF(11, 12, 10, 500), "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);
        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextWidth(10),
        p.setClipRect(QRectF(11, 12, 10, 500));
        text.setPerformanceHint(performanceHint);
        text.setTextFormat(BobUI::PlainText);
        p.drawStaticText(QPointF(11, 12), text);
    }

#if defined(DEBUG_SAVE_IMAGE)
    imageDrawText.save("drawToRect_imageDrawText.png");
    imageDrawStaticText.save("drawToRect_imageDrawStaticText.png");
#endif

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}

void tst_QStaticText::compareToDrawText_data()
{
    BOBUIest::addColumn<QFont>("font");

    BOBUIest::newRow("default") << QFont();
    QFont sansserif; sansserif.setStyleHint(QFont::SansSerif);
    QFont serif; serif.setStyleHint(QFont::Serif);
    QFont monospace; monospace.setStyleHint(QFont::Monospace);
    BOBUIest::newRow("sans-serif") << QFont(sansserif.defaultFamily());
    BOBUIest::newRow("serif") << QFont(serif.defaultFamily());
    BOBUIest::newRow("monospace") << QFont(monospace.defaultFamily());
}

void tst_QStaticText::compareToDrawText()
{
    QFETCH(QFont, font);

    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.setFont(font);
        p.drawText(QRectF(11, 12, 30, 500), "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticPlainText(1000, 1000);
    imageDrawStaticPlainText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticPlainText);
        p.setFont(font);
        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextWidth(10),
        p.setClipRect(QRectF(11, 12, 30, 500));
        text.setTextFormat(BobUI::PlainText);
        p.drawStaticText(QPointF(11, 12), text);
    }

    QPixmap imageDrawStaticRichText(1000, 1000);
    imageDrawStaticRichText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticRichText);
        p.setFont(font);
        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextWidth(10),
        p.setClipRect(QRectF(11, 12, 30, 500));
        text.setTextFormat(BobUI::RichText);
        p.drawStaticText(QPointF(11, 12), text);
    }

#if defined(DEBUG_SAVE_IMAGE)
    imageDrawText.save("compareToDrawText_imageDrawText.png");
    imageDrawStaticPlainText.save("compareToDrawText_imageDrawStaticPlainText.png");
    imageDrawStaticRichText.save("compareToDrawText_imageDrawStaticRichText.png");
#endif

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);
    QCOMPARE(imageDrawStaticPlainText, imageDrawText);
    QCOMPARE(imageDrawStaticRichText, imageDrawText);
}

void tst_QStaticText::prepareToCorrectData()
{
    BOBUIransform transform;
    transform.scale(2.0, 2.0);
    transform.translate(100, 10);
    transform.rotate(90, BobUI::ZAxis);

    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.setTransform(transform);
        p.drawText(11, 12, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);
        p.setTransform(transform);
        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.prepare(transform, p.font());
        text.setTextFormat(BobUI::PlainText);
        p.drawStaticText(QPointF(11, 12  - QFontMetricsF(p.font()).ascent()), text);
    }

#if defined(DEBUG_SAVE_IMAGE)
    imageDrawText.save("prepareToCorrectData_imageDrawText.png");
    imageDrawStaticText.save("prepareToCorrectData_imageDrawStaticText.png");
#endif

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);

    if (!supportsTransformations())
      QEXPECT_FAIL("", "Graphics system does not support transformed text on this platform", Abort);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}

void tst_QStaticText::prepareToWrongData()
{
    BOBUIransform transform;
    transform.scale(2.0, 2.0);
    transform.rotate(90, BobUI::ZAxis);

    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.drawText(11, 12, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);
        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.prepare(transform, p.font());
        text.setTextFormat(BobUI::PlainText);
        p.drawStaticText(QPointF(11, 12  - QFontMetricsF(p.font()).ascent()), text);
    }

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}


void tst_QStaticText::setFont()
{
    QFont font = QGuiApplication::font();
    font.setBold(true);
    font.setPointSize(28);

    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.drawText(QRectF(0, 0, 1000, 1000), 0, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");

        p.setFont(font);
        p.drawText(QRectF(11, 120, 1000, 1000), 0, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);

        QStaticText text;
        text.setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextFormat(BobUI::PlainText);

        p.drawStaticText(0, 0, text);

        p.setFont(font);
        p.drawStaticText(11, 120, text);
    }

#if defined(DEBUG_SAVE_IMAGE)
    imageDrawText.save("setFont_imageDrawText.png");
    imageDrawStaticText.save("setFont_imageDrawStaticText.png");
#endif

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}

void tst_QStaticText::setTextWidth()
{
    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.drawText(QRectF(11, 12, 10, 500), "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);
        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextWidth(10);
        p.setClipRect(QRectF(11, 12, 10, 500));
        p.drawStaticText(QPointF(11, 12), text);
    }

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}

void tst_QStaticText::translatedPainter()
{
    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.translate(100, 200);

        p.drawText(11, 12, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);
        p.translate(100, 200);

        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextFormat(BobUI::PlainText);

        p.drawStaticText(QPointF(11, 12 - QFontMetricsF(p.font()).ascent()), text);
    }

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}

bool tst_QStaticText::supportsTransformations() const
{
    QPixmap pm(10, 10);
    QPainter p(&pm);
    return p.paintEngine()->type() != QPaintEngine::OpenGL;
}

void tst_QStaticText::rotatedPainter()
{
    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.rotate(30.0);
        p.drawText(QRectF(0, 0, 1000, 100), 0, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextFormat(BobUI::PlainText);

        QPainter p(&imageDrawStaticText);
        p.rotate(30.0);
        p.drawStaticText(QPoint(0, 0), text);
    }

#if defined(DEBUG_SAVE_IMAGE)
    imageDrawText.save("rotatedPainter_imageDrawText.png");
    imageDrawStaticText.save("rotatedPainter_imageDrawStaticText.png");
#endif

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);

    if (!supportsTransformations())
      QEXPECT_FAIL("", "Graphics system does not support transformed text on this platform", Abort);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}

void tst_QStaticText::scaledPainter()
{
    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.scale(2.0, 0.2);

        p.drawText(11, 12, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);
        p.scale(2.0, 0.2);

        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextFormat(BobUI::PlainText);

        p.drawStaticText(QPointF(11, 12 - QFontMetricsF(p.font()).ascent()), text);
    }

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);

    if (!supportsTransformations())
      QEXPECT_FAIL("", "Graphics system does not support transformed text on this platform", Abort);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}

void tst_QStaticText::projectedPainter()
{
    BOBUIransform transform;
    transform.rotate(90, BobUI::XAxis);

    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.setTransform(transform);

        p.drawText(11, 12, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);
        p.setTransform(transform);

        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextFormat(BobUI::PlainText);

        p.drawStaticText(QPointF(11, 12 - QFontMetricsF(p.font()).ascent()), text);
    }

    QCOMPARE(imageDrawStaticText, imageDrawText);
}

void tst_QStaticText::transformationChanged()
{
    QPixmap imageDrawText(1000, 1000);
    imageDrawText.fill(BobUI::white);
    {
        QPainter p(&imageDrawText);
        p.rotate(33.0);
        p.scale(0.5, 0.7);

        p.drawText(QRectF(0, 0, 1000, 1000), 0, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");

        p.scale(2.0, 2.5);
        p.drawText(QRectF(0, 0, 1000, 1000), 0, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
    }

    QPixmap imageDrawStaticText(1000, 1000);
    imageDrawStaticText.fill(BobUI::white);
    {
        QPainter p(&imageDrawStaticText);
        p.rotate(33.0);
        p.scale(0.5, 0.7);

        QStaticText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
        text.setTextFormat(BobUI::PlainText);

        p.drawStaticText(QPointF(0, 0), text);

        p.scale(2.0, 2.5);
        p.drawStaticText(QPointF(0, 0), text);
    }

#if defined(DEBUG_SAVE_IMAGE)
    imageDrawText.save("transformationChanged_imageDrawText.png");
    imageDrawStaticText.save("transformationChanged_imageDrawStaticText.png");
#endif

    QVERIFY(imageDrawText.toImage() != m_whiteSquare);

    if (!supportsTransformations())
      QEXPECT_FAIL("", "Graphics system does not support transformed text on this platform", Abort);
    QCOMPARE(imageDrawStaticText, imageDrawText);
}

void tst_QStaticText::plainTextVsRichText()
{
    QPixmap imagePlainText(1000, 1000);
    imagePlainText.fill(BobUI::white);
    {
        QPainter p(&imagePlainText);

        QStaticText staticText;
        staticText.setText("FOObar");
        staticText.setTextFormat(BobUI::PlainText);

        p.drawStaticText(10, 10, staticText);
    }

    QPixmap imageRichText(1000, 1000);
    imageRichText.fill(BobUI::white);
    {
        QPainter p(&imageRichText);

        QStaticText staticText;
        staticText.setText("<html><body>FOObar</body></html>");
        staticText.setTextFormat(BobUI::RichText);

        p.drawStaticText(10, 10, staticText);
    }

#if defined(DEBUG_SAVE_IMAGE)
    imagePlainText.save("plainTextVsRichText_imagePlainText.png");
    imageRichText.save("plainTextVsRichText_imageRichText.png");
#endif

    QVERIFY(imagePlainText.toImage() != m_whiteSquare);
    QCOMPARE(imagePlainText, imageRichText);
}

static bool checkPixels(const QImage &image,
                        BobUI::GlobalColor expectedColor1, BobUI::GlobalColor expectedColor2,
                        QByteArray *errorMessage)
{
    const QRgb expectedRgb1 = QColor(expectedColor1).rgba();
    const QRgb expectedRgb2 = QColor(expectedColor2).rgba();

    for (int x = 0, w = image.width(); x < w; ++x) {
        for (int y = 0, h = image.height(); y < h; ++y) {
            const QRgb pixel = image.pixel(x, y);
            if (pixel != expectedRgb1 && pixel != expectedRgb2) {
                QString message;
                QDebug(&message) << "Color mismatch in image" << image
                    << "at" << x << ',' << y << ':' << BobUI::showbase << BobUI::hex << pixel
                    << "(expected: " << expectedRgb1 << ',' << expectedRgb2 << ')';
                *errorMessage = message.toLocal8Bit();
                return false;
            }
        }
    }
    return true;
}

void tst_QStaticText::setPenPlainText_data()
{
    BOBUIest::addColumn<QImage::Format>("format");

    BOBUIest::newRow("argb32pm") << QImage::Format_ARGB32_Premultiplied;
    BOBUIest::newRow("rgb32") << QImage::Format_RGB32;
    BOBUIest::newRow("rgba8888pm") << QImage::Format_RGBA8888_Premultiplied;
    BOBUIest::newRow("rgbx8888") << QImage::Format_RGBX8888;
}

void tst_QStaticText::setPenPlainText()
{
    QFETCH(QImage::Format, format);

    QFont font = QGuiApplication::font();
    font.setStyleStrategy(QFont::NoAntialias);

    QFontMetricsF fm(font);
    QImage image(qCeil(fm.horizontalAdvance("XXXXX")), qCeil(fm.height()), format);
    image.fill(BobUI::white);
    {
        QPainter p(&image);
        p.setFont(font);
        p.setPen(BobUI::yellow);

        QStaticText staticText("XXXXX");
        staticText.setTextFormat(BobUI::PlainText);
        p.drawStaticText(0, 0, staticText);
    }

    QByteArray errorMessage;
    QVERIFY2(checkPixels(image, BobUI::yellow, BobUI::white, &errorMessage),
             errorMessage.constData());
}

void tst_QStaticText::setPenRichText()
{
    QFont font = QGuiApplication::font();
    font.setStyleStrategy(QFont::NoAntialias);

    QFontMetricsF fm(font);
    QPixmap image(qCeil(fm.horizontalAdvance("XXXXX")), qCeil(fm.height()));
    image.fill(BobUI::white);
    {
        QPainter p(&image);
        p.setFont(font);
        p.setPen(BobUI::green);

        QStaticText staticText;
        staticText.setText("<html><body>XXXXX</body></html>");
        staticText.setTextFormat(BobUI::RichText);
        p.drawStaticText(0, 0, staticText);
    }

    QByteArray errorMessage;
    QVERIFY2(checkPixels(image.toImage(), BobUI::green, BobUI::white, &errorMessage),
             errorMessage.constData());
}

void tst_QStaticText::richTextOverridesPen()
{
    QFont font = QGuiApplication::font();
    font.setStyleStrategy(QFont::NoAntialias);

    QFontMetricsF fm(font);
    QPixmap image(qCeil(fm.horizontalAdvance("XXXXX")), qCeil(fm.height()));
    image.fill(BobUI::white);
    {
        QPainter p(&image);
        p.setFont(font);
        p.setPen(BobUI::green);

        QStaticText staticText;
        staticText.setText("<html><body><font color=\"#ff0000\">XXXXX</font></body></html>");
        staticText.setTextFormat(BobUI::RichText);
        p.drawStaticText(0, 0, staticText);
    }

    QByteArray errorMessage;
    QVERIFY2(checkPixels(image.toImage(), BobUI::red, BobUI::white, &errorMessage),
             errorMessage.constData());
}

void tst_QStaticText::unprintableCharacter_bobuibug12614()
{
    QString s(QChar(0x200B)); // U+200B, ZERO WIDTH SPACE

    QStaticText staticText(s);

    QVERIFY(staticText.size().isValid()); // Force layout. Should not crash.
}

#ifdef BOBUI_BUILD_INTERNAL
void tst_QStaticText::underlinedColor_bobuibug20159()
{
    QString multiScriptText;
    multiScriptText += QChar(0x0410); // Cyrillic 'A'
    multiScriptText += QLatin1Char('A');

    QStaticText staticText(multiScriptText);

    QFont font;
    font.setUnderline(true);

    staticText.prepare(BOBUIransform(), font);

    QStaticTextPrivate *d = QStaticTextPrivate::get(&staticText);
    QCOMPARE(d->itemCount, 2);

    // The pen should not be marked as dirty when drawing the underline
    QVERIFY(!d->items[0].color.isValid());
    QVERIFY(!d->items[1].color.isValid());
}

void tst_QStaticText::textDocumentColor()
{
    QStaticText staticText("A<font color=\"red\">B</font>");
    staticText.setTextFormat(BobUI::RichText);
    staticText.prepare();

    QStaticTextPrivate *d = QStaticTextPrivate::get(&staticText);
    QCOMPARE(d->itemCount, 2);

    // The pen should not be marked as dirty when drawing the underline
    QVERIFY(!d->items[0].color.isValid());
    QVERIFY(d->items[1].color.isValid());

    QCOMPARE(d->items[1].color, QColor(BobUI::red));
}
#endif

class TestPaintEngine: public QPaintEngine
{
public:
    void drawTextItem(const QPointF &p, const BOBUIextItem &) override
    {
        differentVerticalPositions.insert(qRound(p.y()));
    }

    void updateState(const QPaintEngineState &) override {}

    void drawPolygon(const QPointF *, int , PolygonDrawMode ) override {}

    bool begin(QPaintDevice *) override  { return true; }
    bool end() override { return true; }
    void drawPixmap(const QRectF &, const QPixmap &, const QRectF &) override {}
    Type type() const override
    {
        return User;
    }

    QSet<int> differentVerticalPositions;
};

class TestPixmap: public QPixmap
{
public:
    TestPixmap(int w, int h) : QPixmap(w, h), testPaintEngine(new TestPaintEngine) {}
    ~TestPixmap() { delete testPaintEngine; }

    QPaintEngine *paintEngine() const override
    {
        return testPaintEngine;
    }

    TestPaintEngine *testPaintEngine;
};

void tst_QStaticText::multiLine()
{
    TestPixmap pixmap(100, 100);

    TestPaintEngine *paintEngine = pixmap.testPaintEngine;

    {
        QPainter p(&pixmap);
        QStaticText text;
        text.setText(QLatin1String("line 1") + QChar(QChar::LineSeparator) + QLatin1String("line 2"));
        p.drawStaticText(0, 0, text);
    }

    QCOMPARE(paintEngine->differentVerticalPositions.size(), 2);
}

void tst_QStaticText::size_bobuibug65836()
{
    const QString text = QLatin1String("Lorem ipsum dolor sit amet, "
                                        "consectetur adipiscing elit.");
    QFont font("Courier");
    font.setPixelSize(15);

    {
        QStaticText st1(text);
        st1.setTextFormat(BobUI::PlainText);
        st1.prepare(BOBUIransform(), font);

        QStaticText st2(text);
        st2.setTextFormat(BobUI::RichText);
        BOBUIextOption opt;
        opt.setWrapMode(BOBUIextOption::NoWrap);
        st2.setTextOption(opt);
        st2.prepare(BOBUIransform(), font);

        QCOMPARE(st1.size(), st2.size());
    }

    {
        QStaticText st1(text);
        st1.setTextFormat(BobUI::PlainText);
        st1.setTextWidth(10.0);
        st1.prepare(BOBUIransform(), font);

        QStaticText st2(text);
        st2.setTextFormat(BobUI::RichText);
        st2.setTextWidth(10.0);
        st2.prepare(BOBUIransform(), font);

        QCOMPARE(st1.size(), st2.size());
    }
}

BOBUIEST_MAIN(tst_QStaticText)
#include "tst_qstatictext.moc"
