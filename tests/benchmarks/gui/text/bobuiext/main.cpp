// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QDebug>
#include <BOBUIextDocument>
#include <BOBUIextDocumentWriter>
#include <BOBUIextLayout>
#include <BOBUIextCursor>
#include <qmath.h>
#include <QFile>
#include <QPainter>
#include <QBuffer>
#include <bobuiest.h>

Q_DECLARE_METATYPE(QList<BOBUIextLayout::FormatRange>)

class tst_BOBUIext: public QObject
{
    Q_OBJECT
public:
    tst_BOBUIext() {
        m_lorem = QString::fromLatin1("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.");
        m_shortLorem = QString::fromLatin1("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    }

private slots:
    void loadHtml_data();
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void loadHtml();
#endif

    void shaping_data();
    void shaping();

    void odfWriting_empty();
    void odfWriting_text();
    void odfWriting_images();

    void constructDocument();

    void newLineReplacement();
    void formatManipulation();
    void fontResolution();

    void layout_data();
    void layout();
    void formattedLayout_data();
    void formattedLayout();
    void paintLayoutToPixmap();
    void paintLayoutToPixmap_painterFill();

#ifndef BOBUI_NO_TEXTHTMLPARSER
    void document();
    void paintDocToPixmap();
    void paintDocToPixmap_painterFill();
#endif

private:
    QSize setupTextLayout(BOBUIextLayout *layout, bool wrap = true, int wrapWidth = 100);

    QString m_lorem;
    QString m_shortLorem;
};

void tst_BOBUIext::loadHtml_data()
{
    BOBUIest::addColumn<QString>("source");
    BOBUIest::newRow("empty") << QString();
    BOBUIest::newRow("simple") << QString::fromLatin1("<html><b>Foo</b></html>");
    BOBUIest::newRow("simple2") << QString::fromLatin1("<b>Foo</b>");

    QString parag = QString::fromLatin1("<p>%1</p>").arg(m_lorem);
    QString header = QString::fromLatin1("<html><head><title>test</title></head><body>");
    BOBUIest::newRow("long") << QString::fromLatin1("<html><head><title>test</title></head><body>") + parag + parag + parag
        + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag + parag
        + QString::fromLatin1("</html>");
    BOBUIest::newRow("table") <<  header + QLatin1String("<table border=\"1\"1><tr><td>xx</td></tr><tr><td colspan=\"2\">")
        + parag + QLatin1String("</td></tr></table></html");
    BOBUIest::newRow("crappy") <<  header + QLatin1String("<table border=\"1\"1><tr><td>xx</td></tr><tr><td colspan=\"2\">")
        + parag;
}

#ifndef BOBUI_NO_TEXTHTMLPARSER
void tst_BOBUIext::loadHtml()
{
    QFETCH(QString, source);
    BOBUIextDocument doc;
    QBENCHMARK {
        doc.setHtml(source);
    }
}
#endif

void tst_BOBUIext::shaping_data()
{
    BOBUIest::addColumn<QString>("parag");
    BOBUIest::newRow("empty") << QString();
    BOBUIest::newRow("lorem") << m_lorem;
    BOBUIest::newRow("short") << QString::fromLatin1("Lorem ipsum dolor sit amet");

    QString testFile = QFINDTESTDATA("bidi.txt");
    QVERIFY2(!testFile.isEmpty(), "cannot find test file bidi.txt!");
    QFile file(testFile);
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray data = file.readAll();
    QVERIFY(data.size() > 1000);
    QStringList list = QString::fromUtf8(data.data()).split(QLatin1Char('\n'), BobUI::SkipEmptyParts);
    QVERIFY(list.size() % 2 == 0); // even amount as we have title and then content.
    for (int i = 0; i < list.size(); i += 2) {
        BOBUIest::newRow(list.at(i).toLatin1()) << list.at(i+1);
    }
}

void tst_BOBUIext::shaping()
{
    QFETCH(QString, parag);

    BOBUIextLayout lay(parag);
    lay.setCacheEnabled(false);

    // do one run to make sure any fonts are loaded.
    lay.beginLayout();
    lay.createLine();
    lay.endLayout();

    QBENCHMARK {
        lay.beginLayout();
        lay.createLine();
        lay.endLayout();
    }
}

void tst_BOBUIext::odfWriting_empty()
{
    QVERIFY(BOBUIextDocumentWriter::supportedDocumentFormats().contains("ODF")); // odf compiled in
    BOBUIextDocument *doc = new BOBUIextDocument();
    // write it
    QBENCHMARK {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        BOBUIextDocumentWriter writer(&buffer, "ODF");
        writer.write(doc);
    }
    delete doc;
}

void tst_BOBUIext::odfWriting_text()
{
    BOBUIextDocument *doc = new BOBUIextDocument();
    BOBUIextCursor cursor(doc);
    BOBUIextBlockFormat bf;
    bf.setIndent(2);
    cursor.insertBlock(bf);
    cursor.insertText(m_lorem);
    bf.setTopMargin(10);
    cursor.insertBlock(bf);
    cursor.insertText(m_lorem);
    bf.setRightMargin(30);
    cursor.insertBlock(bf);
    cursor.insertText(m_lorem);

    // write it
    QBENCHMARK {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        BOBUIextDocumentWriter writer(&buffer, "ODF");
        writer.write(doc);
    }
    delete doc;
}

void tst_BOBUIext::odfWriting_images()
{
    BOBUIextDocument *doc = new BOBUIextDocument();
    BOBUIextCursor cursor(doc);
    cursor.insertText(m_lorem);
    QImage image(400, 200, QImage::Format_ARGB32_Premultiplied);
    cursor.insertImage(image);
    cursor.insertText(m_lorem);

    // write it
    QBENCHMARK {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        BOBUIextDocumentWriter writer(&buffer, "ODF");
        writer.write(doc);
    }
    delete doc;
}

QSize tst_BOBUIext::setupTextLayout(BOBUIextLayout *layout, bool wrap, int wrapWidth)
{
    layout->setCacheEnabled(true);

    int height = 0;
    qreal widthUsed = 0;
    qreal lineWidth = 0;

    //set manual width
    if (wrap)
        lineWidth = wrapWidth;

    layout->beginLayout();
    while (1) {
        BOBUIextLine line = layout->createLine();
        if (!line.isValid())
            break;

        if (wrap)
            line.setLineWidth(lineWidth);
    }
    layout->endLayout();

    for (int i = 0; i < layout->lineCount(); ++i) {
        BOBUIextLine line = layout->lineAt(i);
        widthUsed = qMax(widthUsed, line.naturalTextWidth());
        line.setPosition(QPointF(0, height));
        height += int(line.height());
    }
    return QSize(qCeil(widthUsed), height);
}

void tst_BOBUIext::constructDocument()
{
    BOBUIextDocument *doc = new BOBUIextDocument;
    delete doc;

    QBENCHMARK {
        BOBUIextDocument *doc = new BOBUIextDocument;
        delete doc;
    }
}

//this step is needed before giving the string to a BOBUIextLayout
void tst_BOBUIext::newLineReplacement()
{
    QString text = QString::fromLatin1("H\ne\nl\nl\no\n\nW\no\nr\nl\nd");

    QBENCHMARK {
        QString tmp = text;
        tmp.replace(QLatin1Char('\n'), QChar::LineSeparator);
    }
}

void tst_BOBUIext::formatManipulation()
{
    QFont font;

    QBENCHMARK {
        BOBUIextCharFormat format;
        format.setFont(font);
    }
}

void tst_BOBUIext::fontResolution()
{
    QFont font;
    QFont font2;
    font.setFamily("DejaVu");
    font2.setBold(true);

    QBENCHMARK {
        QFont res = font.resolve(font2);
    }
}

void tst_BOBUIext::layout_data()
{
    BOBUIest::addColumn<bool>("wrap");
    BOBUIest::newRow("wrap") << true;
    BOBUIest::newRow("nowrap") << false;
}

void tst_BOBUIext::layout()
{
    QFETCH(bool,wrap);
    BOBUIextLayout layout(m_shortLorem);
    setupTextLayout(&layout, wrap);

    QBENCHMARK {
        BOBUIextLayout layout(m_shortLorem);
        setupTextLayout(&layout, wrap);
    }
}

//### requires tst_BOBUIext to be a friend of BOBUIextLayout
/*void tst_BOBUIext::stackTextLayout()
{
    QStackTextEngine engine(m_shortLorem, qApp->font());
    BOBUIextLayout layout(&engine);
    setupTextLayout(&layout);

    QBENCHMARK {
        QStackTextEngine engine(m_shortLorem, qApp->font());
        BOBUIextLayout layout(&engine);
        setupTextLayout(&layout);
    }
}*/

void tst_BOBUIext::formattedLayout_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<QList<BOBUIextLayout::FormatRange>>("ranges");

    BOBUIextCharFormat format;
    format.setForeground(QColor("steelblue"));

    {
        QList<BOBUIextLayout::FormatRange> ranges;

        BOBUIextLayout::FormatRange formatRange;
        formatRange.format = format;
        formatRange.start = 0;
        formatRange.length = 50;
        ranges.append(formatRange);

        BOBUIest::newRow("short-single") << m_shortLorem << ranges;
    }
    {
        QList<BOBUIextLayout::FormatRange> ranges;

        QString text = m_lorem.repeated(100);
        const int width = 1;
        for (int i = 0; i < text.size(); i += width) {
            BOBUIextLayout::FormatRange formatRange;
            formatRange.format.setForeground(QBrush(QColor(i % 255, 255, 255)));
            formatRange.start = i;
            formatRange.length = width;
            ranges.append(formatRange);
        }

        BOBUIest::newRow("long-many") << m_shortLorem << ranges;
    }
}

void tst_BOBUIext::formattedLayout()
{
    QFETCH(QString, text);
    QFETCH(QList<BOBUIextLayout::FormatRange>, ranges);

    BOBUIextLayout layout(text);
    layout.setFormats(ranges);
    setupTextLayout(&layout);

    QBENCHMARK {
        BOBUIextLayout layout(text);
        layout.setFormats(ranges);
        setupTextLayout(&layout);
    }
}

void tst_BOBUIext::paintLayoutToPixmap()
{
    BOBUIextLayout layout(m_shortLorem);
    QSize size = setupTextLayout(&layout);

    QBENCHMARK {
        QPixmap img(size);
        img.fill(BobUI::transparent);
        QPainter p(&img);
        layout.draw(&p, QPointF(0, 0));
    }
}

void tst_BOBUIext::paintLayoutToPixmap_painterFill()
{
    BOBUIextLayout layout(m_shortLorem);
    QSize size = setupTextLayout(&layout);

    QBENCHMARK {
        QPixmap img(size);
        QPainter p(&img);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(0, 0, img.width(), img.height(), BobUI::transparent);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
        layout.draw(&p, QPointF(0, 0));
    }
}

#ifndef BOBUI_NO_TEXTHTMLPARSER
void tst_BOBUIext::document()
{
    BOBUIextDocument *doc = new BOBUIextDocument;
    Q_UNUSED(doc);

    QBENCHMARK {
        BOBUIextDocument *doc = new BOBUIextDocument;
        doc->setHtml(m_shortLorem);
    }
}

void tst_BOBUIext::paintDocToPixmap()
{
    BOBUIextDocument *doc = new BOBUIextDocument;
    doc->setHtml(m_shortLorem);
    doc->setTextWidth(300);
    QSize size = doc->size().toSize();

    QBENCHMARK {
        QPixmap img(size);
        img.fill(BobUI::transparent);
        QPainter p(&img);
        doc->drawContents(&p);
    }
}

void tst_BOBUIext::paintDocToPixmap_painterFill()
{
    BOBUIextDocument *doc = new BOBUIextDocument;
    doc->setHtml(m_shortLorem);
    doc->setTextWidth(300);
    QSize size = doc->size().toSize();

    QBENCHMARK {
        QPixmap img(size);
        QPainter p(&img);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(0, 0, img.width(), img.height(), BobUI::transparent);
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
        doc->drawContents(&p);
    }
}
#endif

BOBUIEST_MAIN(tst_BOBUIext)

#include "main.moc"
