// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BOBUIextDocument>
#include <BOBUIextCursor>
#include <BOBUIextBlock>
#include <BOBUIextList>
#include <BOBUIextTable>
#include <QBuffer>
#include <QDebug>

#include <private/bobuiextodfwriter_p.h>

class tst_BOBUIextOdfWriter : public QObject
{
    Q_OBJECT
public slots:
    void init();
    void cleanup();

private slots:
    void testWriteParagraph_data();
    void testWriteParagraph();
    void testWriteStyle1_data();
    void testWriteStyle1();
    void testWriteStyle2();
    void testWriteList();
    void testWriteList2();
    void createArchive();
    void testWriteAll();
    void testWriteSection();
    void testWriteTable();
    void testWriteFrameFormat();
    void testWriteDefaultFont_data();
    void testWriteDefaultFont();

private:
    /// closes the document and returns the part of the XML stream that the test wrote
    QString getContentFromXml();

private:
    BOBUIextDocument *document;
    QXmlStreamWriter *xmlWriter;
    BOBUIextOdfWriter *odfWriter;
    QBuffer *buffer;
};

void tst_BOBUIextOdfWriter::init()
{
    document = new BOBUIextDocument();
    odfWriter = new BOBUIextOdfWriter(*document, 0);

    buffer = new QBuffer();
    buffer->open(QIODevice::WriteOnly);
    xmlWriter = new QXmlStreamWriter(buffer);
    xmlWriter->writeNamespace(odfWriter->officeNS, "office");
    xmlWriter->writeNamespace(odfWriter->textNS, "text");
    xmlWriter->writeNamespace(odfWriter->styleNS, "style");
    xmlWriter->writeNamespace(odfWriter->foNS, "fo");
    xmlWriter->writeNamespace(odfWriter->tableNS, "table");
    xmlWriter->writeStartDocument();
    xmlWriter->writeStartElement("dummy");
}

void tst_BOBUIextOdfWriter::cleanup()
{
    delete document;
    delete odfWriter;
    delete xmlWriter;
    delete buffer;
}

QString tst_BOBUIextOdfWriter::getContentFromXml()
{
    xmlWriter->writeEndDocument();
    buffer->close();
    QString stringContent = QString::fromUtf8(buffer->data());
    QString ret;
    int index = stringContent.indexOf("<dummy");
    if (index > 0) {
        index = stringContent.indexOf('>', index);
        if (index > 0)
            ret = stringContent.mid(index+1, stringContent.size() - index - 10);
    }
    return ret;
}

void tst_BOBUIextOdfWriter::testWriteParagraph_data()
{
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<QString>("xml");

    BOBUIest::newRow("empty") << "" <<
        "<text:p text:style-name=\"p1\"/>";
    BOBUIest::newRow("spaces") << "foobar   word" <<
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">foobar <text:s text:c=\"2\"/>word</text:span></text:p>";
    BOBUIest::newRow("starting spaces") << "  starting spaces" <<
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\"><text:s text:c=\"2\"/>starting spaces</text:span></text:p>";
    BOBUIest::newRow("trailing spaces") << "trailing spaces  " <<
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">trailing spaces <text:s/></text:span></text:p>";
    BOBUIest::newRow("tab") << "word\ttab x" <<
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">word<text:tab/>tab x</text:span></text:p>";
    BOBUIest::newRow("tab2") << "word\t\ttab\tx" <<
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">word<text:tab/><text:tab/>tab<text:tab/>x</text:span></text:p>";
    BOBUIest::newRow("misc") << "foobar   word\ttab x" <<
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">foobar <text:s text:c=\"2\"/>word<text:tab/>tab x</text:span></text:p>";
    BOBUIest::newRow("misc2") << "\t     \tFoo" <<
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\"><text:tab/> <text:s text:c=\"4\"/><text:tab/>Foo</text:span></text:p>";
    BOBUIest::newRow("linefeed") << (QStringLiteral("line1") + QChar(0x2028) + QStringLiteral("line2")) <<
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">line1<text:tab/><text:line-break/>line2</text:span></text:p>";
    BOBUIest::newRow("spaces") << "The quick brown fox jumped over the lazy dog" <<
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">The quick brown fox jumped over the lazy dog</text:span></text:p>";
}

void tst_BOBUIextOdfWriter::testWriteParagraph()
{
    QFETCH(QString, input);
    QFETCH(QString, xml);

    BOBUIextCursor cursor(document);
    cursor.insertText(input);

    odfWriter->writeBlock(*xmlWriter, document->begin());
    QCOMPARE( getContentFromXml(), xml);
}

void tst_BOBUIextOdfWriter::testWriteStyle1_data()
{
    BOBUIest::addColumn<QString>("htmlInput");
    BOBUIest::addColumn<int>("cursorPosition");
    BOBUIest::addColumn<QString>("xml");

    QString text1 = "Normal<b>bold</b><i>italic</i><b><i>Bold/Italic</i></b>";
    BOBUIest::newRow("normal") << text1 << 2 <<
        "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Sans\"/></style:style>";
    BOBUIest::newRow("bold") << text1 << 10 <<
        "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-weight=\"bold\" fo:font-family=\"Sans\"/></style:style>";
    BOBUIest::newRow("italic") << text1 << 14 <<
        "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-style=\"italic\" fo:font-family=\"Sans\"/></style:style>";
    BOBUIest::newRow("bold+italic") << text1 << 25 <<
        "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-style=\"italic\" fo:font-weight=\"bold\" fo:font-family=\"Sans\"/></style:style>";
    QString colorText = "<span style=\"color: #00FF00; background-color: #FF0000;\"> Color Text </span>";
    BOBUIest::newRow("green/red") << colorText  << 3 <<
        "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Sans\" fo:color=\"#00ff00\" fo:background-color=\"#ff0000\"/></style:style>";

}

void tst_BOBUIextOdfWriter::testWriteDefaultFont_data()
{
    BOBUIest::addColumn<QFont>("defaultFont");
    BOBUIest::addColumn<BOBUIextCharFormat>("charFormat");
    BOBUIest::addColumn<QString>("xml");

    BOBUIextCharFormat emptyFormat;

    BOBUIest::newRow("default")
        << QFont()
        << emptyFormat
        << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Sans\"/></style:style>";

    BOBUIest::newRow("family")
        << QFont(QStringLiteral("Foobar"))
        << emptyFormat
        << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Foobar\"/></style:style>";

    {
        QFont font;
        font.setPointSizeF(7.5);
        BOBUIest::newRow("point-size")
            << font
            << emptyFormat
            << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Sans\" fo:font-size=\"7.5pt\"/></style:style>";
    }

    {
        QFont font;
        font.setItalic(true);
        BOBUIest::newRow("italic")
            << font
            << emptyFormat
            << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-style=\"italic\" fo:font-family=\"Sans\"/></style:style>";
    }

    {
        QFont font;
        font.setBold(true);
        BOBUIest::newRow("bold")
            << font
            << emptyFormat
            << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-weight=\"bold\" fo:font-family=\"Sans\"/></style:style>";
    }

    {
        QFont font;
        font.setCapitalization(QFont::AllUppercase);
        BOBUIest::newRow("capitalization")
            << font
            << emptyFormat
            << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Sans\" fo:text-transform=\"uppercase\"/></style:style>";
    }

    {
        QFont font;
        font.setLetterSpacing(QFont::PercentageSpacing, 10);
        BOBUIest::newRow("letter-spacing")
            << font
            << emptyFormat
            << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Sans\" fo:letter-spacing=\"7.5pt\"/></style:style>";
    }

    {
        QFont font;
        font.setWordSpacing(10);
        BOBUIest::newRow("word-spacing")
            << font
            << emptyFormat
            << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Sans\" fo:word-spacing=\"7.5pt\"/></style:style>";
    }

    {
        QFont font;
        font.setUnderline(true);
        BOBUIest::newRow("underline")
            << font
            << emptyFormat
            << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Sans\" style:text-underline-type=\"single\"/></style:style>";
    }

    {
        QFont font;
        font.setStrikeOut(true);
        BOBUIest::newRow("strike-out")
            << font
            << emptyFormat
            << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-family=\"Sans\" style:text-line-through-type=\"single\"/></style:style>";
    }

    {
        QFont font;
        font.setFamily(QStringLiteral("Foo"));
        font.setPointSizeF(7.5);
        font.setStrikeOut(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setLetterSpacing(QFont::PercentageSpacing, 10);
        font.setWordSpacing(20);
        font.setWeight(QFont::Bold);
        font.setCapitalization(QFont::AllUppercase);

        BOBUIextCharFormat format;
        format.setFontItalic(true);
        format.setFontFamilies(QStringList() << QStringLiteral("Bar"));
        format.setFontPointSize(5.5);
        format.setFontStrikeOut(true);
        format.setFontLetterSpacing(20);
        format.setFontWordSpacing(30);
        format.setFontWeight(QFont::Light);
        format.setUnderlineStyle(BOBUIextCharFormat::SingleUnderline);
        format.setFontCapitalization(QFont::AllLowercase);

        BOBUIest::newRow("char-format-precedence")
            << font
            << format
            << "<style:style style:name=\"c4\" style:family=\"text\"><style:text-properties fo:font-style=\"italic\" fo:font-weight=\"300\" fo:font-family=\"Bar\" fo:font-size=\"5.5pt\" fo:text-transform=\"lowercase\" fo:letter-spacing=\"15pt\" fo:word-spacing=\"22.5pt\" style:text-underline-type=\"single\" style:text-line-through-type=\"single\" style:text-underline-style=\"solid\"/></style:style>";
    }

}

void tst_BOBUIextOdfWriter::testWriteDefaultFont()
{
    QFETCH(QFont, defaultFont);
    QFETCH(BOBUIextCharFormat, charFormat);
    QFETCH(QString, xml);

    document->clear();
    document->setDefaultFont(defaultFont);

    BOBUIextCursor cursor(document);
    cursor.setCharFormat(charFormat);
    cursor.insertText(QStringLiteral("Test"));

    odfWriter->writeCharacterFormat(*xmlWriter, cursor.charFormat(), 4);
    QCOMPARE( getContentFromXml(), xml);


}

void tst_BOBUIextOdfWriter::testWriteStyle1()
{
    QFETCH(QString, htmlInput);
    QFETCH(int, cursorPosition);
    QFETCH(QString, xml);
    document->setHtml(htmlInput);

    BOBUIextCursor cursor(document);
    cursor.setPosition(cursorPosition);
    odfWriter->writeCharacterFormat(*xmlWriter, cursor.charFormat(), 4);
    QCOMPARE( getContentFromXml(), xml);
}

void tst_BOBUIextOdfWriter::testWriteStyle2()
{
    BOBUIextBlockFormat bf; // = cursor.blockFormat();
    QList<BOBUIextOption::Tab> tabs;
    BOBUIextOption::Tab tab1(40, BOBUIextOption::RightTab);
    tabs << tab1;
    BOBUIextOption::Tab tab2(80, BOBUIextOption::DelimiterTab, 'o');
    tabs << tab2;
    bf.setTabPositions(tabs);

    odfWriter->writeBlockFormat(*xmlWriter, bf, 1);
    QString xml = QString::fromLatin1(
        "<style:style style:name=\"p1\" style:family=\"paragraph\">"
            "<style:paragraph-properties>"
                "<style:tab-stops>"
                    "<style:tab-stop style:position=\"30pt\" style:type=\"right\"/>"
                    "<style:tab-stop style:position=\"60pt\" style:type=\"char\" style:char=\"o\"/>"
                "</style:tab-stops>"
            "</style:paragraph-properties>"
        "</style:style>");
    QCOMPARE(getContentFromXml(), xml);
}

void tst_BOBUIextOdfWriter::testWriteList()
{
    BOBUIextCursor cursor(document);
    BOBUIextList *list = cursor.createList(BOBUIextListFormat::ListDisc);
    cursor.insertText("ListItem 1");
    list->add(cursor.block());
    cursor.insertBlock();
    cursor.insertText("ListItem 2");
    list->add(cursor.block());

    odfWriter->writeBlock(*xmlWriter, cursor.block());
    QString xml = QString::fromLatin1(
        "<text:list text:style-name=\"L2\">"
          "<text:list-item>"
        //"<text:numbered-paragraph text:style-name=\"L2\" text:level=\"1\">"
            //"<text:number>")+ QChar(0x25cf) + QString::fromLatin1("</text:number>" // 0x25cf is a bullet
            "<text:p text:style-name=\"p3\"><text:span text:style-name=\"c0\">ListItem 2</text:span></text:p>"
          "</text:list-item>"
        "</text:list>");

    QCOMPARE(getContentFromXml(), xml);
}

void tst_BOBUIextOdfWriter::testWriteList2()
{
    BOBUIextCursor cursor(document);
    BOBUIextList *list = cursor.createList(BOBUIextListFormat::ListDisc);
    cursor.insertText("Cars");
    list->add(cursor.block());
    cursor.insertBlock();
    BOBUIextListFormat level2;
    level2.setStyle(BOBUIextListFormat::ListSquare);
    level2.setIndent(2);
    BOBUIextList *list2 = cursor.createList(level2);
    cursor.insertText("Model T");
    list2->add(cursor.block());
    cursor.insertBlock();
    cursor.insertText("Kitt");
    list2->add(cursor.block());
    cursor.insertBlock();
    cursor.insertText("Animals");
    list->add(cursor.block());

    cursor.insertBlock(BOBUIextBlockFormat(), BOBUIextCharFormat()); // start a new completely unrelated list.
    BOBUIextList *list3 = cursor.createList(BOBUIextListFormat::ListDecimal);
    cursor.insertText("Foo");
    list3->add(cursor.block());

    // and another block thats NOT in a list.
    cursor.insertBlock(BOBUIextBlockFormat(), BOBUIextCharFormat());
    cursor.insertText("Bar");

    odfWriter->writeFrame(*xmlWriter, document->rootFrame());
    QString xml = QString::fromLatin1(
        "<text:list text:style-name=\"L2\">"
          "<text:list-item>"
        //"<text:numbered-paragraph text:style-name=\"L2\" text:level=\"1\">"
            //"<text:number>")+ QChar(0x25cf) + QString::fromLatin1("</text:number>" // 0x25cf is a bullet
            "<text:p text:style-name=\"p3\"><text:span text:style-name=\"c0\">Cars</text:span></text:p>"
          "</text:list-item>"
          "<text:list-item>"
            "<text:list text:style-name=\"L4\">"
              "<text:list-item>"
                "<text:p text:style-name=\"p5\"><text:span text:style-name=\"c0\">Model T</text:span></text:p>"
              "</text:list-item>"
              "<text:list-item>"
                "<text:p text:style-name=\"p5\"><text:span text:style-name=\"c0\">Kitt</text:span></text:p>"
              "</text:list-item>"
            "</text:list>"
          "</text:list-item>"
          "<text:list-item>"
            "<text:p text:style-name=\"p3\"><text:span text:style-name=\"c0\">Animals</text:span></text:p>"
          "</text:list-item>"
        "</text:list>"
        "<text:list text:style-name=\"L6\">"
          "<text:list-item>"
            "<text:p text:style-name=\"p7\"><text:span text:style-name=\"c0\">Foo</text:span></text:p>"
          "</text:list-item>"
        "</text:list>"
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">Bar</text:span></text:p>");

    // QString x = getContentFromXml();
    // for (int i=0; i < x.length(); i+=150) qDebug() << x.mid(i, 150);
    QCOMPARE(getContentFromXml(), xml);
}


void tst_BOBUIextOdfWriter::createArchive()
{
    document->setPlainText("a"); // simple doc is enough ;)
    BOBUIextOdfWriter writer(*document, buffer);
    QCOMPARE(writer.createArchive(), true); // default
    writer.writeAll();
/*
QFile file("createArchive-odt");
file.open(QIODevice::WriteOnly);
file.write(buffer->data());
file.close();
*/
    QVERIFY(buffer->data().size() > 80);
    QCOMPARE(buffer->data()[0], 'P'); // its a zip :)
    QCOMPARE(buffer->data()[1], 'K');
    QString mimetype(buffer->data().mid(38, 39));
    QCOMPARE(mimetype, QString::fromLatin1("application/vnd.oasis.opendocument.text"));
}

void tst_BOBUIextOdfWriter::testWriteAll()
{
    document->setPlainText("a"); // simple doc is enough ;)
    BOBUIextOdfWriter writer(*document, buffer);
    QCOMPARE(writer.createArchive(), true);
    writer.setCreateArchive(false);
    writer.writeAll();
    QString result = QString(buffer->data());
    // details we check elsewhere, all we have to do is check availability.
    QVERIFY(result.indexOf("office:automatic-styles") >= 0);
    QVERIFY(result.indexOf("<style:style style:name=\"p1\"") >= 0);
    QVERIFY(result.indexOf("<style:style style:name=\"c0\"") >= 0);
    QVERIFY(result.indexOf("office:body") >= 0);
    QVERIFY(result.indexOf("office:text") >= 0);
    QVERIFY(result.indexOf("style:style") >= 0);
}

void tst_BOBUIextOdfWriter::testWriteSection()
{
    BOBUIextCursor cursor(document);
    cursor.insertText("foo\nBar");
    BOBUIextFrameFormat ff;
    cursor.insertFrame(ff);
    cursor.insertText("baz");

    odfWriter->writeFrame(*xmlWriter, document->rootFrame());
    QString xml = QString::fromLatin1(
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">foo</text:span></text:p>"
        "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">Bar</text:span></text:p>"
        "<text:section>"
            "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">baz</text:span></text:p>"
        "</text:section>"
        "<text:p text:style-name=\"p1\"/>");

    QCOMPARE(getContentFromXml(), xml);
}

void tst_BOBUIextOdfWriter::testWriteTable()
{
    // create table with merged cells
    BOBUIextCursor cursor(document);
    BOBUIextTable * table = cursor.insertTable(3, 3);
    table->mergeCells(1, 0, 2, 2);
    table->mergeCells(0, 1, 1, 2);
    cursor = table->cellAt(0, 0).firstCursorPosition();
    cursor.insertText("a");
    cursor.movePosition(BOBUIextCursor::NextCell);
    cursor.insertText("b");
    cursor.movePosition(BOBUIextCursor::NextCell);
    cursor.insertText("c");
    cursor.movePosition(BOBUIextCursor::NextCell);
    cursor.insertText("d");
    cursor.movePosition(BOBUIextCursor::NextCell);
    cursor.insertText("e");
    /*
      +-+---+
      |a|b  |
      +-+-+-+
      |c  |d|
      +   +-+
      |   |e|
      +-+-+-+
    */

    odfWriter->writeFrame(*xmlWriter, document->rootFrame());
    QString xml = QString::fromLatin1(
        "<text:p text:style-name=\"p1\"/>"
        "<table:table table:style-name=\"Table2\">"
            "<table:table-column table:number-columns-repeated=\"3\"/>"
            "<table:table-row>"
                "<table:table-cell table:style-name=\"T3\">"
                    "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">a</text:span></text:p>"
                "</table:table-cell>"
                "<table:table-cell table:number-columns-spanned=\"2\" table:style-name=\"T6\">"
                    "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c7\">b</text:span></text:p>"
                "</table:table-cell>"
            "</table:table-row>"
            "<table:table-row>"
                "<table:table-cell table:number-columns-spanned=\"2\" table:number-rows-spanned=\"2\" table:style-name=\"T5\">"
                    "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c8\">c</text:span></text:p>"
                "</table:table-cell>"
                "<table:table-cell table:style-name=\"T3\">"
                    "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">d</text:span></text:p>"
                "</table:table-cell>"
            "</table:table-row>"
            "<table:table-row>"
                "<table:table-cell table:style-name=\"T3\">"
                    "<text:p text:style-name=\"p1\"><text:span text:style-name=\"c0\">e</text:span></text:p>"
                "</table:table-cell>"
            "</table:table-row>"
        "</table:table>"
        "<text:p text:style-name=\"p1\"/>");

    QCOMPARE(getContentFromXml(), xml);
}

void tst_BOBUIextOdfWriter::testWriteFrameFormat()
{
    BOBUIextFrameFormat tff;
    tff.setTopMargin(20);
    tff.setBottomMargin(20);
    tff.setLeftMargin(20);
    tff.setRightMargin(20);
    BOBUIextCursor tc(document);
    odfWriter->writeFrameFormat(*xmlWriter, tff, 0);
    // Value of 15pt is based on the pixelToPoint() calculation done in bobuiextodfwriter.cpp
    QString xml = QString::fromLatin1(
            "<style:style style:name=\"s0\" style:family=\"section\">"
            "<style:section-properties fo:margin-top=\"15pt\" fo:margin-bottom=\"15pt\""
            " fo:margin-left=\"15pt\" fo:margin-right=\"15pt\"/>"
            "</style:style>");
    QCOMPARE(getContentFromXml(), xml);
}

BOBUIEST_MAIN(tst_BOBUIextOdfWriter)
#include "tst_bobuiextodfwriter.moc"
