// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include <bobuiextdocument.h>
#include <qabstracttextdocumentlayout.h>
#include <qdebug.h>
#include <qpainter.h>
#include <bobuiexttable.h>
#ifndef BOBUI_NO_WIDGETS
#include <bobuiextedit.h>
#include <qscrollbar.h>
#endif

class tst_BOBUIextDocumentLayout : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void cleanupTestCase();
    void defaultPageSizeHandling();
    void idealWidth();
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void lineSeparatorFollowingTable();
#endif
#ifndef BOBUI_NO_WIDGETS
    void wrapAtWordBoundaryOrAnywhere();
#endif
    void inlineImage();
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void clippedTableCell();
#endif
    void floatingTablePageBreak();
    void imageAtRightAlignedTab();
    void blockVisibility();
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void testHitTest();

    void largeImage();
#endif

private:
    BOBUIextDocument *doc;
};

void tst_BOBUIextDocumentLayout::init()
{
    doc = new BOBUIextDocument;
}

void tst_BOBUIextDocumentLayout::cleanup()
{
    delete doc;
    doc = 0;
}

void tst_BOBUIextDocumentLayout::cleanupTestCase()
{
    if (qgetenv("BOBUIEST_KEEP_IMAGEDATA").toInt() == 0) {
        QFile::remove(QLatin1String("expected.png"));
        QFile::remove(QLatin1String("img.png"));
    }
}

void tst_BOBUIextDocumentLayout::defaultPageSizeHandling()
{
    QAbstractTextDocumentLayout *layout = doc->documentLayout();
    QVERIFY(layout);

    QVERIFY(!doc->pageSize().isValid());
    QSizeF docSize = layout->documentSize();
    QVERIFY(docSize.width() > 0 && docSize.width() < 1000);
    QVERIFY(docSize.height() > 0 && docSize.height() < 1000);

    doc->setPlainText("Some text\nwith a few lines\nand not real information\nor anything otherwise useful");

    docSize = layout->documentSize();
    QVERIFY(docSize.isValid());
    QVERIFY(docSize.width() != INT_MAX);
    QVERIFY(docSize.height() != INT_MAX);
}

void tst_BOBUIextDocumentLayout::idealWidth()
{
    doc->setPlainText("Some text\nwith a few lines\nand not real information\nor anything otherwise useful");
    doc->setTextWidth(1000);
    QCOMPARE(doc->textWidth(), qreal(1000));
    QCOMPARE(doc->size().width(), doc->textWidth());
    QVERIFY(doc->idealWidth() < doc->textWidth());
    QVERIFY(doc->idealWidth() > 0);

    BOBUIextBlockFormat fmt;
    fmt.setAlignment(BobUI::AlignRight | BobUI::AlignAbsolute);
    BOBUIextCursor cursor(doc);
    cursor.select(BOBUIextCursor::Document);
    cursor.mergeBlockFormat(fmt);

    QCOMPARE(doc->textWidth(), qreal(1000));
    QCOMPARE(doc->size().width(), doc->textWidth());
    QVERIFY(doc->idealWidth() < doc->textWidth());
    QVERIFY(doc->idealWidth() > 0);
}

#ifndef BOBUI_NO_TEXTHTMLPARSER
// none of the BOBUIextLine items in the document should intersect with the margin rect
void tst_BOBUIextDocumentLayout::lineSeparatorFollowingTable()
{
    QString html_begin("<html><table border=1><tr><th>Column 1</th></tr><tr><td>Data</td></tr></table><br>");
    QString html_text("bla bla bla bla bla bla bla bla<br>");
    QString html_end("<table border=1><tr><th>Column 1</th></tr><tr><td>Data</td></tr></table></html>");

    QString html = html_begin;

    for (int i = 0; i < 80; ++i)
        html += html_text;

    html += html_end;

    doc->setHtml(html);

    BOBUIextCursor cursor(doc);
    cursor.movePosition(BOBUIextCursor::Start);

    const int margin = 87;
    const int pageWidth = 873;
    const int pageHeight = 1358;

    BOBUIextFrameFormat fmt = doc->rootFrame()->frameFormat();
    fmt.setMargin(margin);
    doc->rootFrame()->setFrameFormat(fmt);

    QFont font(doc->defaultFont());
    font.setPointSize(10);
    doc->setDefaultFont(font);
    doc->setPageSize(QSizeF(pageWidth, pageHeight));

    QRectF marginRect(QPointF(0, pageHeight - margin), QSizeF(pageWidth, 2 * margin));

    // force layouting
    doc->pageCount();

    for (BOBUIextBlock block = doc->begin(); block != doc->end(); block = block.next()) {
        BOBUIextLayout *layout = block.layout();
        for (int i = 0; i < layout->lineCount(); ++i) {
            BOBUIextLine line = layout->lineAt(i);
            QRectF rect = line.rect().translated(layout->position());
            QVERIFY(!rect.intersects(marginRect));
        }
    }
}
#endif

#ifndef BOBUI_NO_WIDGETS
void tst_BOBUIextDocumentLayout::wrapAtWordBoundaryOrAnywhere()
{
    //task 150562
    BOBUIextEdit edit;
    edit.setText("<table><tr><td>hello hello hello"
            "thisisabigwordthisisabigwordthisisabigwordthisisabigwordthisisabigword"
            "hello hello hello</td></tr></table>");
    edit.setWordWrapMode(BOBUIextOption::WrapAtWordBoundaryOrAnywhere);
    edit.resize(100, 100);
    edit.show();
    QVERIFY(!edit.horizontalScrollBar()->isVisible());
}
#endif

void tst_BOBUIextDocumentLayout::inlineImage()
{
    doc->setPageSize(QSizeF(800, 500));

    QImage img(400, 400, QImage::Format_RGB32);
    QLatin1String name("bigImage");

    doc->addResource(BOBUIextDocument::ImageResource, QUrl(name), img);

    BOBUIextImageFormat imgFormat;
    imgFormat.setName(name);
    imgFormat.setWidth(img.width());

    BOBUIextFrameFormat fmt = doc->rootFrame()->frameFormat();
    qreal height = doc->pageSize().height() - fmt.topMargin() - fmt.bottomMargin();
    imgFormat.setHeight(height);

    BOBUIextCursor cursor(doc);
    cursor.insertImage(imgFormat);

    QCOMPARE(doc->pageCount(), 1);
}

#ifndef BOBUI_NO_TEXTHTMLPARSER
void tst_BOBUIextDocumentLayout::clippedTableCell()
{
    const char *html =
        "<table style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\""
        "border=\"0\" margin=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td></td></tr></table>";

    doc->setHtml(html);
    doc->pageSize();

    BOBUIextCursor cursor(doc);
    cursor.movePosition(BOBUIextCursor::Right);

    BOBUIextTable *table = cursor.currentTable();
    QVERIFY(table);

    BOBUIextCursor cellCursor = table->cellAt(0, 0).firstCursorPosition();
    QImage src(16, 16, QImage::Format_ARGB32_Premultiplied);
    src.fill(0xffff0000);
    cellCursor.insertImage(src);

    BOBUIextBlock block = cellCursor.block();
    QRectF r = doc->documentLayout()->blockBoundingRect(block);

    QRectF rect(0, 0, r.left() + 1, 64);

    QImage img(64, 64, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x0);
    QImage expected = img;
    QPainter p(&img);
    doc->drawContents(&p, rect);
    p.end();
    p.begin(&expected);
    r.setWidth(1);
    p.fillRect(r, BobUI::red);
    p.end();

    img.save("img.png");
    expected.save("expected.png");
    QCOMPARE(img, expected);
}
#endif

void tst_BOBUIextDocumentLayout::floatingTablePageBreak()
{
    doc->clear();

    BOBUIextCursor cursor(doc);

    BOBUIextTableFormat tableFormat;
    tableFormat.setPosition(BOBUIextFrameFormat::FloatLeft);
    BOBUIextTable *table = cursor.insertTable(50, 1, tableFormat);
    Q_UNUSED(table);

    // Make height of document 2/3 of the table, fitting the table into two pages
    QSizeF documentSize = doc->size();
    documentSize.rheight() *= 2.0 / 3.0;

    doc->setPageSize(documentSize);

    QCOMPARE(doc->pageCount(), 2);
}

void tst_BOBUIextDocumentLayout::imageAtRightAlignedTab()
{
    doc->clear();

    BOBUIextFrameFormat fmt = doc->rootFrame()->frameFormat();
    fmt.setMargin(0);
    doc->rootFrame()->setFrameFormat(fmt);

    BOBUIextCursor cursor(doc);
    BOBUIextBlockFormat blockFormat;
    QList<BOBUIextOption::Tab> tabs;
    BOBUIextOption::Tab tab;
    tab.position = 300;
    tab.type = BOBUIextOption::RightTab;
    tabs.append(tab);
    blockFormat.setTabPositions(tabs);

    // First block: text, some of it right-aligned
    cursor.insertBlock(blockFormat);
    cursor.insertText("first line\t");
    cursor.insertText("right-aligned text");

    // Second block: text, then right-aligned image
    cursor.insertBlock(blockFormat);
    cursor.insertText("second line\t");
    QImage img(48, 48, QImage::Format_RGB32);
    const QString name = QString::fromLatin1("image");
    doc->addResource(BOBUIextDocument::ImageResource, QUrl(name), img);
    BOBUIextImageFormat imgFormat;
    imgFormat.setName(name);
    cursor.insertImage(imgFormat);

    // Everything should fit into the 300 pixels
    qreal bearing = QFontMetricsF(doc->defaultFont()).rightBearing(QLatin1Char('t'));
    QCOMPARE(doc->idealWidth(), std::max(300.0, 300.0 - bearing));
}

void tst_BOBUIextDocumentLayout::blockVisibility()
{
    BOBUIextCursor cursor(doc);
    for (int i = 0; i < 10; ++i) {
        if (!doc->isEmpty())
            cursor.insertBlock();
        cursor.insertText("A");
    }

    qreal margin = doc->documentMargin();
    QSizeF emptySize(2 * margin, 2 * margin);
    QSizeF halfSize = doc->size();
    halfSize.rheight() -= 2 * margin;
    halfSize.rheight() /= 2;
    halfSize.rheight() += 2 * margin;

    for (int i = 0; i < 10; i += 2) {
        BOBUIextBlock block = doc->findBlockByNumber(i);
        block.setVisible(false);
        doc->markContentsDirty(block.position(), block.length());
    }

    QCOMPARE(doc->size(), halfSize);

    for (int i = 1; i < 10; i += 2) {
        BOBUIextBlock block = doc->findBlockByNumber(i);
        block.setVisible(false);
        doc->markContentsDirty(block.position(), block.length());
    }

    QCOMPARE(doc->size(), emptySize);

    for (int i = 0; i < 10; i += 2) {
        BOBUIextBlock block = doc->findBlockByNumber(i);
        block.setVisible(true);
        doc->markContentsDirty(block.position(), block.length());
    }

    QCOMPARE(doc->size(), halfSize);
}

#ifndef BOBUI_NO_TEXTHTMLPARSER
void tst_BOBUIextDocumentLayout::largeImage()
{
     auto img = QImage(400, 500, QImage::Format_ARGB32_Premultiplied);
     img.fill(BobUI::black);

     {
         BOBUIextDocument document;

         document.addResource(BOBUIextDocument::ImageResource,
                 QUrl("data://test.png"), QVariant(img));
         document.setPageSize({500, 504});

         auto html = "<img src=\"data://test.png\">";
         document.setHtml(html);

         QCOMPARE(document.pageCount(), 2);
     }

     {
         BOBUIextDocument document;

         document.addResource(BOBUIextDocument::ImageResource,
                 QUrl("data://test.png"), QVariant(img));
         document.setPageSize({500, 508});

         auto html = "<img src=\"data://test.png\">";
         document.setHtml(html);

         QCOMPARE(document.pageCount(), 1);
     }

     {
         BOBUIextDocument document;

         document.addResource(BOBUIextDocument::ImageResource,
                 QUrl("data://test.png"), QVariant(img));
         document.setPageSize({585, 250});

         auto html = "<img src=\"data://test.png\">";
         document.setHtml(html);

         QCOMPARE(document.pageCount(), 3);
     }

     {
         BOBUIextDocument document;

         document.addResource(BOBUIextDocument::ImageResource,
                 QUrl("data://test.png"), QVariant(img));
         document.setPageSize({585, 258});

         auto html = "<img src=\"data://test.png\">";
         document.setHtml(html);

         QCOMPARE(document.pageCount(), 2);
     }
}

void tst_BOBUIextDocumentLayout::testHitTest()
{
    BOBUIextDocument document;
    BOBUIextCursor cur(&document);
    int topMargin = 20;

    //insert 500 blocks into textedit
    for (int i = 0; i < 500; i++) {
      cur.insertBlock();
      cur.insertHtml(QString("block %1").arg(i));
    }

    //randomly set half the blocks invisible
    BOBUIextBlock blk=document.begin();
    for (int i = 0; i < 500; i++) {
      if (i % 7)
        blk.setVisible(0);
      blk = blk.next();
    }

    //set margin for all blocks (not strictly necessary, but makes easier to click in between blocks)
    BOBUIextBlockFormat blkfmt;
    blkfmt.setTopMargin(topMargin);
    cur.movePosition(BOBUIextCursor::Start);
    cur.movePosition(BOBUIextCursor::End, BOBUIextCursor::KeepAnchor);
    cur.mergeBlockFormat(blkfmt);

    for (int y = cur.selectionStart(); y < cur.selectionEnd(); y += 10) {
         QPoint mousePoint(1, y);
         int cursorPos = document.documentLayout()->hitTest(mousePoint, BobUI::FuzzyHit);
         int positionY = document.findBlock(cursorPos).layout()->position().toPoint().y();
         //mousePoint is in the rect of the current Block
         QVERIFY(positionY - topMargin <= y);
    }
}
#endif

BOBUIEST_MAIN(tst_BOBUIextDocumentLayout)
#include "tst_bobuiextdocumentlayout.moc"
