// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>

#include <bobuiextdocument.h>
#include <qdebug.h>

#include <bobuiextcursor.h>
#include <bobuiextdocumentfragment.h>
#include <bobuiextformat.h>
#include <bobuiextobject.h>
#include <bobuiexttable.h>
#include <qabstracttextdocumentlayout.h>
#include <bobuiextlist.h>
#include <qguiapplication.h>
#include <qurl.h>
#include <qpainter.h>
#include <qfontmetrics.h>
#include <qimage.h>
#include <bobuiextlayout.h>
#include <QDomDocument>
#include "common.h"

// #define DEBUG_WRITE_OUTPUT

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextDocument)

class tst_BOBUIextDocument : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIextDocument();

private slots:
    void init();
    void cleanup();
    void cleanupTestCase();
    void getSetCheck();
    void isEmpty();
    void find_data();
    void find();
    void find2();
    void findWithRegularExpression_data();
    void findWithRegularExpression();
    void findMultiple();
    void basicIsModifiedChecks();
    void moreIsModified();
    void isModified2();
    void isModified3();
    void isModified4();
    void noundo_basicIsModifiedChecks();
    void noundo_moreIsModified();
    void noundo_isModified2();
    void noundo_isModified3();
    void mightBeRichText();
    void mightBeRichText_data();

    void task240325();

    void preFont();

    void stylesheetFont_data();
    void stylesheetFont();

    void toHtml_data();
    void toHtml();
    void toHtml2();

    void setFragmentMarkersInHtmlExport();
    void setMediaRule();

    void toHtmlBodyBgColor();
    void toHtmlBodyBgColorRgba();
    void toHtmlBodyBgColorTransparent();
    void toHtmlRootFrameProperties();
    void toHtmlLineHeightProperties();
    void toHtmlDefaultFontSpacingProperties();
    void toHtmlTextDecorationUnderline();
    void capitalizationHtmlInExport();
    void wordspacingHtmlExport();

    void cursorPositionChanged();
    void cursorPositionChangedOnSetText();

    void textFrameIterator();

    void markContentsDirty();

    void clonePreservesMetaInformation();
    void clonePreservesPageSize();
    void clonePreservesPageBreakPolicies();
    void clonePreservesDefaultFont();
    void clonePreservesRootFrameFormat();
    void clonePreservesResources();
    void clonePreservesUserStates();
    void clonePreservesIndentWidth();
    void clonePreservesFormatsWhenEmpty();
    void blockCount();

    void defaultStyleSheet();
    void defaultTableStyle_data();
    void defaultTableStyle();

    void resolvedFontInEmptyFormat();

    void defaultRootFrameMargin();

    void clearResources();

    void setPlainText();
    void toPlainText_data();
    void toPlainText();
    void toRawText();

    void deleteTextObjectsOnClear();

    void maximumBlockCount();
    void adjustSize();
    void initialUserData();

    void html_defaultFont();

    void blockCountChanged();

    void nonZeroDocumentLengthOnClear();

    void setTextPreservesUndoRedoEnabled();

    void firstLast();

    void backgroundImage_toHtml();
    void backgroundImage_toHtml2();
    void backgroundImage_clone();
    void backgroundImage_copy();

    void documentCleanup();

    void characterAt();
    void revisions();
    void revisionWithUndoCompressionAndUndo();

    void testUndoCommandAdded();

    void testUndoBlocks();

    void receiveCursorPositionChangedAfterContentsChange();

    void copiedFontSize();

    void BOBUIBUG25778_pixelSizeFromHtml();

    void htmlExportImportBlockCount();

    void BOBUIBUG27354_spaceAndSoftSpace();
    void baseUrl_data();
    void baseUrl();

    void BOBUIBUG28998_linkColor();

    void textCursorUsageWithinContentsChange();
    void cssInheritance();

    void lineHeightType();
    void cssLineHeightMultiplier();

    void fontTagFace();

    void clearUndoRedoStacks();
    void mergeFontFamilies();

    void resourceProvider();

    void contentsChangeIndices_data();
    void contentsChangeIndices();

    void insertHtmlWithComments_data();
    void insertHtmlWithComments();

    void delayedLayout();
    void undoContentChangeIndices();

    void restoreStrokeFromHtml();
    void restoreForegroundGradientFromHtml();

private:
    void backgroundImage_checkExpectedHtml(const BOBUIextDocument &doc);
    void buildRegExpData();
    static QString cssFontSizeString(const QFont &font);
    void writeActualAndExpected(const char* testTag, const QString &actual, const QString &expected);

    BOBUIextDocument *doc;
    BOBUIextCursor cursor;
    QFont defaultFont;
    QString htmlHead;
    QString htmlTail;
};

class MyAbstractTextDocumentLayout : public QAbstractTextDocumentLayout
{
public:
    MyAbstractTextDocumentLayout(BOBUIextDocument *doc) : QAbstractTextDocumentLayout(doc) {}
    void draw(QPainter *, const PaintContext &) override {}
    int hitTest(const QPointF &, BobUI::HitTestAccuracy) const override { return 0; }
    int pageCount() const override { return 0; }
    QSizeF documentSize() const override { return QSizeF(); }
    QRectF frameBoundingRect(BOBUIextFrame *) const override { return QRectF(); }
    QRectF blockBoundingRect(const BOBUIextBlock &) const override { return QRectF(); }
    void documentChanged(int, int, int) override {}
};

QString tst_BOBUIextDocument::cssFontSizeString(const QFont &font)
{
    return font.pointSize() >= 0
            ? QString::number(font.pointSizeF()) + QStringLiteral("pt")
            : QString::number(font.pixelSize()) + QStringLiteral("px");
}

void tst_BOBUIextDocument::writeActualAndExpected(const char *testTag, const QString &actual, const QString &expected)
{
#ifdef DEBUG_WRITE_OUTPUT
    {
        QFile out(QDir::temp().absoluteFilePath(QLatin1String(testTag) + QLatin1String("-actual.html")));
        out.open(QFile::WriteOnly);
        out.write(actual.toUtf8());
        out.close();
    } {
        QFile out(QDir::temp().absoluteFilePath(QLatin1String(testTag) + QLatin1String("-expected.html")));
        out.open(QFile::WriteOnly);
        out.write(expected.toUtf8());
        out.close();
    }
#else
    Q_UNUSED(testTag);
    Q_UNUSED(actual);
    Q_UNUSED(expected);
#endif
}

// Testing get/set functions
void tst_BOBUIextDocument::getSetCheck()
{
    BOBUIextDocument obj1;
    // QAbstractTextDocumentLayout * BOBUIextDocument::documentLayout()
    // void BOBUIextDocument::setDocumentLayout(QAbstractTextDocumentLayout *)
    QPointer<MyAbstractTextDocumentLayout> var1 = new MyAbstractTextDocumentLayout(0);
    obj1.setDocumentLayout(var1);
    QCOMPARE(static_cast<QAbstractTextDocumentLayout *>(var1), obj1.documentLayout());
    obj1.setDocumentLayout((QAbstractTextDocumentLayout *)0);
    QVERIFY(var1.isNull());
    QVERIFY(obj1.documentLayout());

    // bool BOBUIextDocument::useDesignMetrics()
    // void BOBUIextDocument::setUseDesignMetrics(bool)
    obj1.setUseDesignMetrics(false);
    QCOMPARE(false, obj1.useDesignMetrics());
    obj1.setUseDesignMetrics(true);
    QCOMPARE(true, obj1.useDesignMetrics());
}

tst_BOBUIextDocument::tst_BOBUIextDocument()
{
    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    img.save("foo.png");
}

void tst_BOBUIextDocument::init()
{
    doc = new BOBUIextDocument;
    cursor = BOBUIextCursor(doc);
    defaultFont = QFont();

    htmlHead = QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" "
            "\"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
            "<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
            "p, li { white-space: pre-wrap; }\n"
            "hr { height: 1px; border-width: 0; }\n"
            "li.unchecked::marker { content: \"\\2610\"; }\n"
            "li.checked::marker { content: \"\\2612\"; }\n"
            "</style></head>"
            "<body style=\" font-family:'%1'; font-size:%2; font-weight:%3; font-style:%4;\">\n");
    htmlHead = htmlHead.arg(defaultFont.family())
                       .arg(cssFontSizeString(defaultFont))
                       .arg(defaultFont.weight())
                       .arg((defaultFont.italic() ? "italic" : "normal"));

    htmlTail = QString("</body></html>");
}

void tst_BOBUIextDocument::cleanup()
{
    cursor = BOBUIextCursor();
    delete doc;
    doc = 0;

    BOBUIextDocument::setDefaultResourceProvider({});
}

void tst_BOBUIextDocument::cleanupTestCase()
{
    QFile::remove(QLatin1String("foo.png"));
}

void tst_BOBUIextDocument::isEmpty()
{
    QVERIFY(doc->isEmpty());
}

void tst_BOBUIextDocument::find_data()
{
    BOBUIest::addColumn<QString>("haystack");
    BOBUIest::addColumn<QString>("needle");
    BOBUIest::addColumn<int>("flags");
    BOBUIest::addColumn<int>("from");
    BOBUIest::addColumn<int>("anchor");
    BOBUIest::addColumn<int>("position");

    BOBUIest::newRow("1") << "Hello World" << "World" << int(BOBUIextDocument::FindCaseSensitively) << 0 << 6 << 11;

    BOBUIest::newRow("2") << QString::fromLatin1("Hello") + QString(QChar::ParagraphSeparator) + QString::fromLatin1("World")
                    << "World" << int(BOBUIextDocument::FindCaseSensitively) << 1 << 6 << 11;

    BOBUIest::newRow("3") << QString::fromLatin1("Hello") + QString(QChar::ParagraphSeparator) + QString::fromLatin1("World")
                    << "Hello" << int(BOBUIextDocument::FindCaseSensitively | BOBUIextDocument::FindBackward) << 10 << 0 << 5;
    BOBUIest::newRow("4wholewords") << QString::fromLatin1("Hello Blah World")
                              << "Blah" << int(BOBUIextDocument::FindWholeWords) << 0 << 6 << 10;
    BOBUIest::newRow("5wholewords") << QString::fromLatin1("HelloBlahWorld")
                              << "Blah" << int(BOBUIextDocument::FindWholeWords) << 0 << -1 << -1;
    BOBUIest::newRow("6wholewords") << QString::fromLatin1("HelloBlahWorld Blah Hah")
                              << "Blah" << int(BOBUIextDocument::FindWholeWords) << 0 << 15 << 19;
    BOBUIest::newRow("7wholewords") << QString::fromLatin1("HelloBlahWorld Blah Hah")
                              << "Blah" << int(BOBUIextDocument::FindWholeWords | BOBUIextDocument::FindBackward) << 23 << 15 << 19;
    BOBUIest::newRow("8wholewords") << QString::fromLatin1("Hello: World\n")
                              << "orld" << int(BOBUIextDocument::FindWholeWords) << 0 << -1 << -1;

    BOBUIest::newRow("across-paragraphs") << QString::fromLatin1("First Parag\nSecond Parag with a lot more text")
                                       << "Parag" << int(BOBUIextDocument::FindBackward)
                                       << 15 << 6 << 11;

    BOBUIest::newRow("nbsp") << "Hello" + QString(QChar(QChar::Nbsp)) +"World" << " " << int(BOBUIextDocument::FindCaseSensitively) << 0 << 5 << 6;

    BOBUIest::newRow("from-the-end") << "Hello World" << "Hello World" << int(BOBUIextDocument::FindCaseSensitively| BOBUIextDocument::FindBackward) << 11 << 0 << 11;

    BOBUIest::newRow("bw-cross-paras-1") << "a1\na2\nb1" << "a" << int(BOBUIextDocument::FindBackward) << 7 << 3 << 4;
    BOBUIest::newRow("bw-cross-paras-2") << "a1\na2\nb1" << "a" << int(BOBUIextDocument::FindBackward) << 6 << 3 << 4;
    BOBUIest::newRow("bw-cross-paras-3") << "a1\na2\nb1" << "a" << int(BOBUIextDocument::FindBackward) << 5 << 3 << 4;
    BOBUIest::newRow("bw-cross-paras-4") << "a1\na2\nb1" << "a" << int(BOBUIextDocument::FindBackward) << 3 << 0 << 1;
    BOBUIest::newRow("bw-cross-paras-5") << "xa\n\nb1" << "a" << int(BOBUIextDocument::FindBackward) << 5 << 1 << 2;
    BOBUIest::newRow("bw-cross-paras-6") << "xa\n\nb1" << "a" << int(BOBUIextDocument::FindBackward) << 4 << 1 << 2;
    BOBUIest::newRow("bw-cross-paras-7") << "xa\n\nb1" << "a" << int(BOBUIextDocument::FindBackward) << 3 << 1 << 2;
    BOBUIest::newRow("bw-cross-paras-8") << "xa\n\nb1" << "a" << int(BOBUIextDocument::FindBackward) << 2 << 1 << 2;
}

void tst_BOBUIextDocument::find()
{
    QFETCH(QString, haystack);
    QFETCH(QString, needle);
    QFETCH(int, flags);
    QFETCH(int, from);
    QFETCH(int, anchor);
    QFETCH(int, position);

    cursor.insertText(haystack);
    cursor = doc->find(needle, from, BOBUIextDocument::FindFlags(flags));

    if (anchor != -1) {
        QCOMPARE(cursor.anchor(), anchor);
        QCOMPARE(cursor.position(), position);
    } else {
        QVERIFY(cursor.isNull());
    }

    //search using a regular expression
    QRegularExpression expr(QRegularExpression::escape(needle));
    BOBUIextDocument::FindFlags flg(flags);
    cursor = doc->find(expr, from, flg);

    if (anchor != -1) {
        QCOMPARE(cursor.anchor(), anchor);
        QCOMPARE(cursor.position(), position);
    } else {
        QVERIFY(cursor.isNull());
    }
}

void tst_BOBUIextDocument::findWithRegularExpression_data()
{
    buildRegExpData();
}

void tst_BOBUIextDocument::findWithRegularExpression()
{
    QFETCH(QString, haystack);
    QFETCH(QString, needle);
    QFETCH(int, flags);
    QFETCH(int, from);
    QFETCH(int, anchor);
    QFETCH(int, position);

    cursor.insertText(haystack);
    //search using a regular expression
    QRegularExpression expr(needle);
    BOBUIextDocument::FindFlags flg(flags);
    cursor = doc->find(expr, from, flg);

    if (anchor != -1) {
        QCOMPARE(cursor.anchor(), anchor);
        QCOMPARE(cursor.position(), position);
    } else {
        QVERIFY(cursor.isNull());
    }
}

void tst_BOBUIextDocument::find2()
{
    doc->setPlainText("aaa");
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextCharacter, BOBUIextCursor::KeepAnchor);
    BOBUIextCursor hit = doc->find("a", cursor);
    QCOMPARE(hit.position(), 2);
    QCOMPARE(hit.anchor(), 1);
}

void tst_BOBUIextDocument::findMultiple()
{
    const QString text("foo bar baz foo bar baz");
    doc->setPlainText(text);

    cursor.movePosition(BOBUIextCursor::Start);
    cursor = doc->find("bar", cursor);
    QCOMPARE(cursor.selectionStart(), text.indexOf("bar"));
    QCOMPARE(cursor.selectionEnd(), cursor.selectionStart() + 3);
    cursor = doc->find("bar", cursor);
    QCOMPARE(cursor.selectionStart(), text.lastIndexOf("bar"));
    QCOMPARE(cursor.selectionEnd(), cursor.selectionStart() + 3);

    cursor.movePosition(BOBUIextCursor::End);
    cursor = doc->find("bar", cursor, BOBUIextDocument::FindBackward);
    QCOMPARE(cursor.selectionStart(), text.lastIndexOf("bar"));
    QCOMPARE(cursor.selectionEnd(), cursor.selectionStart() + 3);
    cursor = doc->find("bar", cursor, BOBUIextDocument::FindBackward);
    QCOMPARE(cursor.selectionStart(), text.indexOf("bar"));
    QCOMPARE(cursor.selectionEnd(), cursor.selectionStart() + 3);

    QRegularExpression regularExpression("bar");

    cursor.movePosition(BOBUIextCursor::End);
    cursor = doc->find(regularExpression, cursor, BOBUIextDocument::FindBackward);
    QCOMPARE(cursor.selectionStart(), text.lastIndexOf("bar"));
    QCOMPARE(cursor.selectionEnd(), cursor.selectionStart() + 3);
    cursor = doc->find(regularExpression, cursor, BOBUIextDocument::FindBackward);
    QCOMPARE(cursor.selectionStart(), text.indexOf("bar"));
    QCOMPARE(cursor.selectionEnd(), cursor.selectionStart() + 3);

    cursor.movePosition(BOBUIextCursor::Start);
    cursor = doc->find(regularExpression, cursor);
    QCOMPARE(cursor.selectionStart(), text.indexOf("bar"));
    QCOMPARE(cursor.selectionEnd(), cursor.selectionStart() + 3);
    cursor = doc->find(regularExpression, cursor);
    QCOMPARE(cursor.selectionStart(), text.lastIndexOf("bar"));
    QCOMPARE(cursor.selectionEnd(), cursor.selectionStart() + 3);
}

void tst_BOBUIextDocument::basicIsModifiedChecks()
{
    QSignalSpy spy(doc, SIGNAL(modificationChanged(bool)));

    QVERIFY(!doc->isModified());
    cursor.insertText("Hello World");
    QVERIFY(doc->isModified());
    QCOMPARE(spy.size(), 1);
    QVERIFY(spy.takeFirst().at(0).toBool());

    doc->undo();
    QVERIFY(!doc->isModified());
    QCOMPARE(spy.size(), 1);
    QVERIFY(!spy.takeFirst().at(0).toBool());

    doc->redo();
    QVERIFY(doc->isModified());
    QCOMPARE(spy.size(), 1);
    QVERIFY(spy.takeFirst().at(0).toBool());
}

void tst_BOBUIextDocument::moreIsModified()
{
    QVERIFY(!doc->isModified());

    cursor.insertText("Hello");
    QVERIFY(doc->isModified());

    doc->undo();
    QVERIFY(!doc->isModified());

    cursor.insertText("Hello");

    doc->undo();
    QVERIFY(!doc->isModified());
}

void tst_BOBUIextDocument::isModified2()
{
    // reported on bobui4-preview-feedback
    QVERIFY(!doc->isModified());

    cursor.insertText("Hello");
    QVERIFY(doc->isModified());

    doc->setModified(false);
    QVERIFY(!doc->isModified());

    cursor.insertText("Hello");
    QVERIFY(doc->isModified());
}

void tst_BOBUIextDocument::isModified3()
{
    QVERIFY(!doc->isModified());

    doc->setUndoRedoEnabled(false);
    doc->setUndoRedoEnabled(true);

    cursor.insertText("Hello");

    QVERIFY(doc->isModified());
    doc->undo();
    QVERIFY(!doc->isModified());
}

void tst_BOBUIextDocument::isModified4()
{
    QVERIFY(!doc->isModified());

    cursor.insertText("Hello");
    cursor.insertText("World");

    doc->setModified(false);

    QVERIFY(!doc->isModified());

    cursor.insertText("Again");
    QVERIFY(doc->isModified());

    doc->undo();
    QVERIFY(!doc->isModified());
    doc->undo();
    QVERIFY(doc->isModified());

    doc->redo();
    QVERIFY(!doc->isModified());
    doc->redo();
    QVERIFY(doc->isModified());

    doc->undo();
    QVERIFY(!doc->isModified());
    doc->undo();
    QVERIFY(doc->isModified());

    //task 197769
    cursor.insertText("Hello");
    QVERIFY(doc->isModified());
}

void tst_BOBUIextDocument::noundo_basicIsModifiedChecks()
{
    doc->setUndoRedoEnabled(false);
    QSignalSpy spy(doc, SIGNAL(modificationChanged(bool)));

    QVERIFY(!doc->isModified());
    cursor.insertText("Hello World");
    QVERIFY(doc->isModified());
    QCOMPARE(spy.size(), 1);
    QVERIFY(spy.takeFirst().at(0).toBool());

    doc->undo();
    QVERIFY(doc->isModified());
    QCOMPARE(spy.size(), 0);

    doc->redo();
    QVERIFY(doc->isModified());
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIextDocument::task240325()
{
    doc->setHtml("<html><img width=\"100\" height=\"100\" align=\"right\"/>Foobar Foobar Foobar Foobar</html>");

    QImage img(1000, 7000, QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    QFontMetrics fm(p.font());

    // Set page size to contain image and one "Foobar"
    doc->setPageSize(QSize(100 + fm.horizontalAdvance("Foobar")*2, 1000));

    // Force layout
    doc->drawContents(&p);

    QCOMPARE(doc->blockCount(), 1);
    for (BOBUIextBlock block = doc->begin() ; block!=doc->end() ; block = block.next()) {
        BOBUIextLayout *layout = block.layout();
        QCOMPARE(layout->lineCount(), 4);

        for (int lineIdx=0;lineIdx<layout->lineCount();++lineIdx) {
            BOBUIextLine line = layout->lineAt(lineIdx);

            QString text = block.text().mid(line.textStart(), line.textLength()).trimmed();

            // Remove start token
            if (lineIdx == 0)
                text = text.mid(1);

            QCOMPARE(text, QString::fromLatin1("Foobar"));
        }
    }
}

void tst_BOBUIextDocument::stylesheetFont_data()
{
    BOBUIest::addColumn<QString>("stylesheet");
    BOBUIest::addColumn<QFont>("font");

    {
        QFont font;
        font.setBold(true);
        font.setPixelSize(64);

        BOBUIest::newRow("Regular font specification")
                 << "font-size: 64px; font-weight: bold;"
                 << font;
    }


    {
        QFont font;
        font.setBold(true);
        font.setPixelSize(64);

        BOBUIest::newRow("Shorthand font specification")
                << "font: normal bold 64px Arial;"
                << font;
    }

}

void tst_BOBUIextDocument::stylesheetFont()
{
    QFETCH(QString, stylesheet);
    QFETCH(QFont, font);

    QString html = QString::fromLatin1("<html>"
                                       "<body>"
                                       "<div style=\"%1\" >"
                                       "Foobar"
                                       "</div>"
                                       "</body>"
                                       "</html>").arg(stylesheet);

    qDebug() << html;
    doc->setHtml(html);
    QCOMPARE(doc->blockCount(), 1);

    // First and only block
    BOBUIextBlock block = doc->firstBlock();

    QString text = block.text();
    QCOMPARE(text, QString::fromLatin1("Foobar"));

    QFont actualFont = block.charFormat().font();

    QCOMPARE(actualFont.bold(), font.bold());
    QCOMPARE(actualFont.pixelSize(), font.pixelSize());
}

void tst_BOBUIextDocument::preFont()
{
    const QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    const QString html = QString::fromLatin1(   "<html>"
                                                "<body>"
                                                "<pre>"
                                                "Foobar"
                                                "</pre>"
                                                "</body>"
                                                "</html>");

    doc->setHtml(html);
    QCOMPARE(doc->blockCount(), 1);

    // First and only block
    BOBUIextBlock block = doc->firstBlock();

    QString text = block.text();
    QCOMPARE(text, QString::fromLatin1("Foobar"));

    QFont actualFont = block.charFormat().font();
    QCOMPARE(actualFont.family(), font.family());
}

void tst_BOBUIextDocument::noundo_moreIsModified()
{
    doc->setUndoRedoEnabled(false);
    QVERIFY(!doc->isModified());

    cursor.insertText("Hello");
    QVERIFY(doc->isModified());

    doc->undo();
    QVERIFY(doc->isModified());

    cursor.insertText("Hello");

    doc->undo();
    QVERIFY(doc->isModified());
}

void tst_BOBUIextDocument::noundo_isModified2()
{
    // reported on bobui4-preview-feedback
    QVERIFY(!doc->isModified());

    cursor.insertText("Hello");
    QVERIFY(doc->isModified());

    doc->setModified(false);
    QVERIFY(!doc->isModified());

    cursor.insertText("Hello");
    QVERIFY(doc->isModified());
}

void tst_BOBUIextDocument::noundo_isModified3()
{
    doc->setUndoRedoEnabled(false);
    QVERIFY(!doc->isModified());

    cursor.insertText("Hello");

    QVERIFY(doc->isModified());
    doc->undo();
    QVERIFY(doc->isModified());
}

void tst_BOBUIextDocument::mightBeRichText_data()
{
    const char bobuiDocuHeader[] = "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"
                                "<!DOCTYPE html\n"
                                "    PUBLIC ""-//W3C//DTD XHTML 1.0 Strict//EN\" \"DTD/xhtml1-strict.dtd\">\n"
                                "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">";
    QVERIFY(BobUI::mightBeRichText(QString::fromLatin1(bobuiDocuHeader)));
    QVERIFY(BobUI::mightBeRichText(QLatin1StringView(bobuiDocuHeader)));
    QVERIFY(QUtf8StringView(bobuiDocuHeader).isValidUtf8());
    QVERIFY(BobUI::mightBeRichText(QUtf8StringView(bobuiDocuHeader)));
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<bool>("result");

    BOBUIest::newRow("documentation-header") << QString("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"
                                                     "<!DOCTYPE html\n"
                                                     "    PUBLIC ""-//W3C//DTD XHTML 1.0 Strict//EN\" \"DTD/xhtml1-strict.dtd\">\n"
                                                     "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">")
                                          << true;
    BOBUIest::newRow("br-nospace") << QString("Test <br/> new line") << true;
    BOBUIest::newRow("br-space") << QString("Test <br /> new line") << true;
    BOBUIest::newRow("br-invalidspace") << QString("Test <br/ > new line") << false;
    BOBUIest::newRow("invalid closing tag") << QString("Test <br/ line") << false;
}

void tst_BOBUIextDocument::mightBeRichText()
{
    QFETCH(QString, input);
    QFETCH(bool, result);
    QCOMPARE(result, BobUI::mightBeRichText(input));
    QCOMPARE(result, BobUI::mightBeRichText(QStringView(input)));
    QCOMPARE(result, BobUI::mightBeRichText(QUtf8StringView(input.toUtf8())));
    QVERIFY(BobUIPrivate::isLatin1(input));
    QCOMPARE(result, BobUI::mightBeRichText(QLatin1StringView(input.toLatin1())));
}

Q_DECLARE_METATYPE(BOBUIextDocumentFragment)

#define CREATE_DOC_AND_CURSOR() \
        BOBUIextDocument doc; \
        doc.setDefaultFont(defaultFont); \
        BOBUIextCursor cursor(&doc);

void tst_BOBUIextDocument::toHtml_data()
{
    BOBUIest::addColumn<BOBUIextDocumentFragment>("input");
    BOBUIest::addColumn<QString>("expectedOutput");

    {
        CREATE_DOC_AND_CURSOR();

        cursor.insertText("Blah");

        BOBUIest::newRow("simple") << BOBUIextDocumentFragment(&doc) << QString("<p DEFAULTBLOCKSTYLE>Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        cursor.insertText("&<>");

        BOBUIest::newRow("entities") << BOBUIextDocumentFragment(&doc) << QString("<p DEFAULTBLOCKSTYLE>&amp;&lt;&gt;</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setFontFamilies({QLatin1String("Times")});
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("font-family") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><span style=\" font-family:'Times';\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setFontFamilies({QLatin1String("Foo's Family")});
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("font-family-with-quotes1") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><span style=\" font-family:&quot;Foo's Family&quot;;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setFontFamilies({QLatin1String("Foo\"s Family")});
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("font-family-with-quotes2") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><span style=\" font-family:'Foo&quot;s Family';\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setFontFamilies(QStringList{ "Times", "serif" });
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("font-family-with-fallback") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><span style=\" font-family:'Times','serif';\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setNonBreakableLines(true);
        cursor.insertBlock(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("pre") << BOBUIextDocumentFragment(&doc)
                          <<
                             QString("EMPTYBLOCK") +
                             QString("<pre DEFAULTBLOCKSTYLE>Blah</pre>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setFontPointSize(40);
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("font-size") << BOBUIextDocumentFragment(&doc)
                                << QString("<p DEFAULTBLOCKSTYLE><span style=\" font-size:40pt;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setProperty(BOBUIextFormat::FontSizeIncrement, 2);
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("logical-font-size") << BOBUIextDocumentFragment(&doc)
                                        << QString("<p DEFAULTBLOCKSTYLE><span style=\" font-size:x-large;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        cursor.insertText("Foo");

        BOBUIextCharFormat fmt;
        fmt.setFontPointSize(40);
        cursor.insertBlock(BOBUIextBlockFormat(), fmt);

        fmt.clearProperty(BOBUIextFormat::FontPointSize);
        cursor.insertText("Blub", fmt);

        BOBUIest::newRow("no-font-size") << BOBUIextDocumentFragment(&doc)
                                   << QString("<p DEFAULTBLOCKSTYLE>Foo</p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Blub</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setLayoutDirection(BobUI::RightToLeft);
        cursor.insertBlock(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("rtl") << BOBUIextDocumentFragment(&doc)
                          <<
                             QString("EMPTYBLOCK") +
                             QString("<p dir='rtl' DEFAULTBLOCKSTYLE>Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setAlignment(BobUI::AlignJustify);
        cursor.insertBlock(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("blockalign") << BOBUIextDocumentFragment(&doc)
                                 <<
                                    QString("EMPTYBLOCK") +
                                    QString("<p align=\"justify\" DEFAULTBLOCKSTYLE>Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setAlignment(BobUI::AlignCenter);
        cursor.insertBlock(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("blockalign2") << BOBUIextDocumentFragment(&doc)
                                  <<
                                    QString("EMPTYBLOCK") +
                                    QString("<p align=\"center\" DEFAULTBLOCKSTYLE>Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setAlignment(BobUI::AlignRight | BobUI::AlignAbsolute);
        cursor.insertBlock(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("blockalign3") << BOBUIextDocumentFragment(&doc)
                                  <<
                                    QString("EMPTYBLOCK") +
                                    QString("<p align=\"right\" DEFAULTBLOCKSTYLE>Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setBackground(QColor("#0000ff"));
        cursor.insertBlock(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("bgcolor") << BOBUIextDocumentFragment(&doc)
                                 << QString("EMPTYBLOCK") +
                                    QString("<p OPENDEFAULTBLOCKSTYLE background-color:#0000ff;\">Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setBackground(QColor(255, 0, 0, 51));
        cursor.insertBlock(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("bgcolor-rgba") << BOBUIextDocumentFragment(&doc)
                                      << QString("EMPTYBLOCK") +
                                         QString("<p OPENDEFAULTBLOCKSTYLE background-color:rgba(255,0,0,0.2);\">Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setBackground(QColor(255, 0, 0, 0));
        cursor.insertBlock(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("bgcolor-transparent") << BOBUIextDocumentFragment(&doc)
                                             << QString("EMPTYBLOCK") +
                                                QString("<p OPENDEFAULTBLOCKSTYLE background-color:transparent;\">Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setFontWeight(320);
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("font-weight") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><span style=\" font-weight:320;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setFontItalic(true);
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("font-italic") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><span style=\" font-style:italic;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setFontUnderline(true);
        fmt.setFontOverline(false);
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("text-decoration-1") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><span style=\" text-decoration: underline;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setForeground(QColor("#00ff00"));
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("color") << BOBUIextDocumentFragment(&doc)
                            << QString("<p DEFAULTBLOCKSTYLE><span style=\" color:#00ff00;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setForeground(QColor(0, 255, 0, 51));
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("color-rgba") << BOBUIextDocumentFragment(&doc)
                                    << QString("<p DEFAULTBLOCKSTYLE><span style=\" color:rgba(0,255,0,0.2);\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setForeground(QColor(0, 255, 0, 0));
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("color-transparent") << BOBUIextDocumentFragment(&doc)
                                           << QString("<p DEFAULTBLOCKSTYLE><span style=\" color:transparent;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setBackground(QColor("#00ff00"));
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("span-bgcolor") << BOBUIextDocumentFragment(&doc)
                            << QString("<p DEFAULTBLOCKSTYLE><span style=\" background-color:#00ff00;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setBackground(QColor(0, 255, 0, 51));
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("span-bgcolor-rgba") << BOBUIextDocumentFragment(&doc)
                                           << QString("<p DEFAULTBLOCKSTYLE><span style=\" background-color:rgba(0,255,0,0.2);\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setBackground(QColor(0, 255, 0, 0));
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("span-bgcolor-transparent") << BOBUIextDocumentFragment(&doc)
                                                  << QString("<p DEFAULTBLOCKSTYLE><span style=\" background-color:transparent;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setVerticalAlignment(BOBUIextCharFormat::AlignSubScript);
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("valign-sub") << BOBUIextDocumentFragment(&doc)
                                 << QString("<p DEFAULTBLOCKSTYLE><span style=\" vertical-align:sub;\">Blah</span></p>");

    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setVerticalAlignment(BOBUIextCharFormat::AlignSuperScript);
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("valign-super") << BOBUIextDocumentFragment(&doc)
                                   << QString("<p DEFAULTBLOCKSTYLE><span style=\" vertical-align:super;\">Blah</span></p>");

    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setAnchor(true);
        fmt.setAnchorNames({"blub"});
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("named anchor") << BOBUIextDocumentFragment(&doc)
                                   << QString("<p DEFAULTBLOCKSTYLE><a name=\"blub\"></a>Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setAnchor(true);
        fmt.setAnchorHref("http://www.kde.org/");
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("href anchor") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><a href=\"http://www.kde.org/\">Blah</a></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setAnchor(true);
        fmt.setAnchorHref("http://www.kde.org/?a=1&b=2");
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("href anchor with &") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><a href=\"http://www.kde.org/?a=1&amp;b=2\">Blah</a></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setAnchor(true);
        fmt.setAnchorHref("http://www.kde.org/?a='&b=\"");
        cursor.insertText("Blah", fmt);

        BOBUIest::newRow("href anchor with ' and \"") << BOBUIextDocumentFragment(&doc)
                                  << QString("<p DEFAULTBLOCKSTYLE><a href=\"http://www.kde.org/?a='&amp;b=&quot;\">Blah</a></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        cursor.insertTable(2, 2);

        BOBUIest::newRow("simpletable") << BOBUIextDocumentFragment(&doc)
                                  << QString("<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">"
                                             "\n<tr>\n<td></td>\n<td></td></tr>"
                                             "\n<tr>\n<td></td>\n<td></td></tr>"
                                             "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTable *table = cursor.insertTable(1, 4);
        table->mergeCells(0, 0, 1, 2);
        table->mergeCells(0, 2, 1, 2);

        BOBUIest::newRow("tablespans") << BOBUIextDocumentFragment(&doc)
                                 << QString("<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">"
                                             "\n<tr>\n<td colspan=\"2\"></td>\n<td colspan=\"2\"></td></tr>"
                                             "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTableFormat fmt;
        fmt.setBorder(1);
        fmt.setCellSpacing(3);
        fmt.setCellPadding(3);
        fmt.setBackground(QColor("#ff00ff"));
        fmt.setWidth(BOBUIextLength(BOBUIextLength::PercentageLength, 50));
        fmt.setAlignment(BobUI::AlignHCenter);
        fmt.setPosition(BOBUIextFrameFormat::FloatRight);
        cursor.insertTable(2, 2, fmt);

        BOBUIest::newRow("tableattrs") << BOBUIextDocumentFragment(&doc)
                                  << QString("<table border=\"1\" style=\" float: right; border-collapse:collapse;\" align=\"center\" width=\"50%\" cellspacing=\"3\" cellpadding=\"3\" bgcolor=\"#ff00ff\">"
                                             "\n<tr>\n<td></td>\n<td></td></tr>"
                                             "\n<tr>\n<td></td>\n<td></td></tr>"
                                             "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTableFormat fmt;
        fmt.setBorder(1);
        fmt.setCellSpacing(3);
        fmt.setCellPadding(3);
        fmt.setBackground(QColor("#ff00ff"));
        fmt.setWidth(BOBUIextLength(BOBUIextLength::PercentageLength, 50));
        fmt.setAlignment(BobUI::AlignHCenter);
        fmt.setPosition(BOBUIextFrameFormat::FloatRight);
        fmt.setLeftMargin(25);
        fmt.setBottomMargin(35);
        cursor.insertTable(2, 2, fmt);

        BOBUIest::newRow("tableattrs2") << BOBUIextDocumentFragment(&doc)
                                  << QString("<table border=\"1\" style=\" float: right; margin-top:0px; margin-bottom:35px; margin-left:25px; margin-right:0px; border-collapse:collapse;\" align=\"center\" width=\"50%\" cellspacing=\"3\" cellpadding=\"3\" bgcolor=\"#ff00ff\">"
                                             "\n<tr>\n<td></td>\n<td></td></tr>"
                                             "\n<tr>\n<td></td>\n<td></td></tr>"
                                             "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTableFormat fmt;
        fmt.setHeaderRowCount(2);
        cursor.insertTable(4, 2, fmt);

        BOBUIest::newRow("tableheader") << BOBUIextDocumentFragment(&doc)
                                  << QString("<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">"
                                             "<thead>\n<tr>\n<td></td>\n<td></td></tr>"
                                             "\n<tr>\n<td></td>\n<td></td></tr></thead>"
                                             "\n<tr>\n<td></td>\n<td></td></tr>"
                                             "\n<tr>\n<td></td>\n<td></td></tr>"
                                             "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTable *table = cursor.insertTable(2, 2);
        BOBUIextTable *subTable = table->cellAt(0, 1).firstCursorPosition().insertTable(1, 1);
        subTable->cellAt(0, 0).firstCursorPosition().insertText("Hey");

        BOBUIest::newRow("nestedtable") << BOBUIextDocumentFragment(&doc)
                                  << QString("<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">"
                                             "\n<tr>\n<td></td>\n<td>\n<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">\n<tr>\n<td>\n<p DEFAULTBLOCKSTYLE>Hey</p></td></tr></table></td></tr>"
                                             "\n<tr>\n<td></td>\n<td></td></tr>"
                                             "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTableFormat fmt;
        QList<BOBUIextLength> widths;
        widths.append(BOBUIextLength());
        widths.append(BOBUIextLength(BOBUIextLength::PercentageLength, 30));
        widths.append(BOBUIextLength(BOBUIextLength::FixedLength, 40));
        fmt.setColumnWidthConstraints(widths);
        cursor.insertTable(1, 3, fmt);

        BOBUIest::newRow("colwidths") << BOBUIextDocumentFragment(&doc)
                                  << QString("<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">"
                                             "\n<tr>\n<td></td>\n<td width=\"30%\"></td>\n<td width=\"40\"></td></tr>"
                                             "</table>");
    }

    // ### rowspan/colspan tests, once texttable api for that is back again
    //
    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTable *table = cursor.insertTable(1, 1);
        BOBUIextCursor cellCurs = table->cellAt(0, 0).firstCursorPosition();
        BOBUIextCharFormat fmt;
        fmt.setBackground(QColor("#ffffff"));
        cellCurs.mergeBlockCharFormat(fmt);

        BOBUIest::newRow("cellproperties") << BOBUIextDocumentFragment(&doc)
                                     << QString("<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">"
                                                "\n<tr>\n<td bgcolor=\"#ffffff\"></td></tr>"
                                                "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        // ### fixme: use programmatic api as soon as we can create floats through it
        const char html[] = "<html><body>Blah<img src=\"image.png\" width=\"10\" height=\"20\" style=\"float: right;\" />Blubb</body></html>";

        BOBUIest::newRow("image") << BOBUIextDocumentFragment::fromHtml(QString::fromLatin1(html))
                            << QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Blah<img src=\"image.png\" width=\"10\" height=\"20\" style=\"float: right;\" />Blubb</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextImageFormat fmt;
        fmt.setName("foo");
        fmt.setVerticalAlignment(BOBUIextCharFormat::AlignMiddle);
        cursor.insertImage(fmt);

        BOBUIest::newRow("image-align-middle") << BOBUIextDocumentFragment(&doc)
                            << QString("<p DEFAULTBLOCKSTYLE><img src=\"foo\" style=\"vertical-align: middle;\" /></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextImageFormat fmt;
        fmt.setName("foo");
        fmt.setVerticalAlignment(BOBUIextCharFormat::AlignTop);
        cursor.insertImage(fmt);

        BOBUIest::newRow("image-align-top") << BOBUIextDocumentFragment(&doc)
                            << QString("<p DEFAULTBLOCKSTYLE><img src=\"foo\" style=\"vertical-align: top;\" /></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextImageFormat fmt;
        fmt.setName("foo");
        cursor.insertImage(fmt);
        cursor.insertImage(fmt);

        BOBUIest::newRow("2images") << BOBUIextDocumentFragment(&doc)
                            << QString("<p DEFAULTBLOCKSTYLE><img src=\"foo\" /><img src=\"foo\" /></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        QString txt = QLatin1String("Blah");
        txt += QChar::LineSeparator;
        txt += QLatin1String("Bar");
        cursor.insertText(txt);

        BOBUIest::newRow("linebreaks") << BOBUIextDocumentFragment(&doc)
                                 << QString("<p DEFAULTBLOCKSTYLE>Blah<br />Bar</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setTopMargin(10);
        fmt.setBottomMargin(20);
        fmt.setLeftMargin(30);
        fmt.setRightMargin(40);
        cursor.insertBlock(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("blockmargins") << BOBUIextDocumentFragment(&doc)
                          <<
                             QString("EMPTYBLOCK") +
                             QString("<p style=\" margin-top:10px; margin-bottom:20px; margin-left:30px; margin-right:40px; -bobui-block-indent:0; text-indent:0px;\">Blah</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextList *list = cursor.insertList(BOBUIextListFormat::ListDisc);
        cursor.insertText("Blubb");
        cursor.insertBlock();
        cursor.insertText("Blah");
        QCOMPARE(list->count(), 2);

        BOBUIest::newRow("lists") << BOBUIextDocumentFragment(&doc)
                          <<
                             QString("EMPTYBLOCK") +
                             QString("<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -bobui-list-indent: 1;\">\n<li DEFAULTBLOCKSTYLE>Blubb</li>\n<li DEFAULTBLOCKSTYLE>Blah</li></ul>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextList *list = cursor.insertList(BOBUIextListFormat::ListDisc);
        cursor.insertText("Blubb");

        cursor.insertBlock();

        BOBUIextCharFormat blockCharFmt;
        blockCharFmt.setForeground(QColor("#0000ff"));
        cursor.mergeBlockCharFormat(blockCharFmt);

        BOBUIextCharFormat fmt;
        fmt.setForeground(QColor("#ff0000"));
        cursor.insertText("Blah", fmt);
        QCOMPARE(list->count(), 2);

        BOBUIest::newRow("charfmt-for-list-item") << BOBUIextDocumentFragment(&doc)
                          <<
                             QString("EMPTYBLOCK") +
                             QString("<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -bobui-list-indent: 1;\">\n<li DEFAULTBLOCKSTYLE>Blubb</li>\n<li style=\" color:#0000ff;\" DEFAULTBLOCKSTYLE><span style=\" color:#ff0000;\">Blah</span></li></ul>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setIndent(3);
        fmt.setTextIndent(30);
        cursor.insertBlock(fmt);
        cursor.insertText("Test");

        BOBUIest::newRow("block-indent") << BOBUIextDocumentFragment(&doc)
                                   <<
                                    QString("EMPTYBLOCK") +
                                    QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:3; text-indent:30px;\">Test</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextListFormat fmt;
        fmt.setStyle(BOBUIextListFormat::ListDisc);
        fmt.setIndent(4);
        cursor.insertList(fmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("list-indent") << BOBUIextDocumentFragment(&doc)
                                  <<
                                    QString("EMPTYBLOCK") +
                                    QString("<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -bobui-list-indent: 4;\">\n<li DEFAULTBLOCKSTYLE>Blah</li></ul>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        cursor.insertBlock();


        BOBUIest::newRow("emptyblock") << BOBUIextDocumentFragment(&doc)
                                    // after insertBlock() we /do/ have two blocks in the document, so also expect
                                    // these in the html output
                                    << QString("EMPTYBLOCK") + QString("EMPTYBLOCK");
    }

    {
        CREATE_DOC_AND_CURSOR();

        // if you press enter twice in an empty textedit and then insert 'Test'
        // you actually get three visible paragraphs, two empty leading ones and
        // a third with the actual text. the corresponding html representation
        // therefore should also contain three paragraphs.

        cursor.insertBlock();
        BOBUIextCharFormat fmt;
        fmt.setForeground(QColor("#00ff00"));
        fmt.setProperty(BOBUIextFormat::FontSizeIncrement, 1);
        cursor.mergeBlockCharFormat(fmt);

        fmt.setProperty(BOBUIextFormat::FontSizeIncrement, 2);
        cursor.insertText("Test", fmt);

        BOBUIest::newRow("blockcharfmt") << BOBUIextDocumentFragment(&doc)
                                   << QString("EMPTYBLOCK<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\"><span style=\" font-size:x-large; color:#00ff00;\">Test</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setForeground(QColor("#00ff00"));
        cursor.setBlockCharFormat(fmt);
        fmt.setForeground(QColor("#0000ff"));
        cursor.insertText("Test", fmt);

        BOBUIest::newRow("blockcharfmt2") << BOBUIextDocumentFragment(&doc)
                                   << QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\"><span style=\" color:#0000ff;\">Test</span></p>");
    }

    {
        BOBUIest::newRow("horizontal-ruler") << BOBUIextDocumentFragment::fromHtml("<hr />")
                                       <<
                                          QString("EMPTYBLOCK") +
                                          QString("<hr />");
    }
    {
        BOBUIest::newRow("horizontal-ruler-with-width") << BOBUIextDocumentFragment::fromHtml("<hr width=\"50%\"/>")
                                                  <<
                                                      QString("EMPTYBLOCK") +
                                                      QString("<hr width=\"50%\" />");
    }
    {
        BOBUIest::newRow("horizontal-ruler-with-color") << BOBUIextDocumentFragment::fromHtml("<hr style=\"background-color:green;\"/>")
                                                     <<
                                                        QString("EMPTYBLOCK") +
                                                        QString("<hr style=\"background-color:#008000;\"/>");
    }
    {
        BOBUIest::newRow("horizontal-ruler-with-width-and-color") << BOBUIextDocumentFragment::fromHtml("<hr width=\"50%\" style=\"background-color:green;\"/>")
                                                     <<
                                                        QString("EMPTYBLOCK") +
                                                        QString("<hr width=\"50%\" style=\"background-color:#008000;\"/>");
    }
    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextFrame *mainFrame = cursor.currentFrame();

        BOBUIextFrameFormat ffmt;
        ffmt.setBorder(1);
        ffmt.setPosition(BOBUIextFrameFormat::FloatRight);
        ffmt.setMargin(2);
        ffmt.setWidth(100);
        ffmt.setHeight(50);
        ffmt.setBackground(QColor("#00ff00"));
        cursor.insertFrame(ffmt);
        cursor.insertText("Hello World");
        cursor = mainFrame->lastCursorPosition();

        BOBUIest::newRow("frame") << BOBUIextDocumentFragment(&doc)
                            << QString("<table border=\"1\" style=\"-bobui-table-type: frame; float: right; margin-top:2px; margin-bottom:2px; margin-left:2px; margin-right:2px;\" width=\"100\" height=\"50\" bgcolor=\"#00ff00\">\n<tr>\n<td style=\"border: none;\">\n<p DEFAULTBLOCKSTYLE>Hello World</p></td></tr></table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;
        fmt.setForeground(QColor("#00ff00"));
//        fmt.setBackground(QColor("#0000ff"));
        cursor.setBlockCharFormat(fmt);

        fmt.setForeground(QBrush());
//        fmt.setBackground(QBrush());
        cursor.insertText("Test", fmt);

//        BOBUIest::newRow("nostylebrush") << BOBUIextDocumentFragment(&doc) << QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px; color:#00ff00; -bobui-blockcharfmt-background-color:#0000ff;\">Test</p>");
        BOBUIest::newRow("nostylebrush") << BOBUIextDocumentFragment(&doc) << QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Test</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTable *table = cursor.insertTable(2, 2);
        table->mergeCells(0, 0, 1, 2);
        BOBUIextTableFormat fmt = table->format();
        QList<BOBUIextLength> widths;
        widths.append(BOBUIextLength(BOBUIextLength::FixedLength, 20));
        widths.append(BOBUIextLength(BOBUIextLength::FixedLength, 40));
        fmt.setColumnWidthConstraints(widths);
        table->setFormat(fmt);

        BOBUIest::newRow("mergedtablecolwidths") << BOBUIextDocumentFragment(&doc)
                                  << QString("<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">"
                                             "\n<tr>\n<td colspan=\"2\"></td></tr>"
                                             "\n<tr>\n<td width=\"20\"></td>\n<td width=\"40\"></td></tr>"
                                             "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextCharFormat fmt;

        cursor.insertText("Blah\nGreen yellow green");
        cursor.setPosition(0);
        cursor.setPosition(23, BOBUIextCursor::KeepAnchor);
        fmt.setBackground(BobUI::green);
        cursor.mergeCharFormat(fmt);
        cursor.clearSelection();
        cursor.setPosition(11);
        cursor.setPosition(17, BOBUIextCursor::KeepAnchor);
        fmt.setBackground(BobUI::yellow);
        cursor.mergeCharFormat(fmt);
        cursor.clearSelection();

        BOBUIest::newRow("multiparagraph-bgcolor") << BOBUIextDocumentFragment(&doc)
                                 << QString("<p DEFAULTBLOCKSTYLE><span style=\" background-color:#00ff00;\">Blah</span></p>\n"
                                            "<p DEFAULTBLOCKSTYLE><span style=\" background-color:#00ff00;\">Green </span>"
                                            "<span style=\" background-color:#ffff00;\">yellow</span>"
                                            "<span style=\" background-color:#00ff00;\"> green</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat fmt;
        fmt.setBackground(QColor("#0000ff"));
        cursor.insertBlock(fmt);

        BOBUIextCharFormat charfmt;
        charfmt.setBackground(QColor("#0000ff"));
        cursor.insertText("Blah", charfmt);

        BOBUIest::newRow("nospan-bgcolor") << BOBUIextDocumentFragment(&doc)
                                 << QString("EMPTYBLOCK") +
                                    QString("<p OPENDEFAULTBLOCKSTYLE background-color:#0000ff;\"><span style=\" background-color:#0000ff;\">Blah</span></p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTable *table = cursor.insertTable(2, 2);
        BOBUIextCharFormat fmt = table->cellAt(0, 0).format();
        fmt.setVerticalAlignment(BOBUIextCharFormat::AlignMiddle);
        table->cellAt(0, 0).setFormat(fmt);
        fmt = table->cellAt(0, 1).format();
        fmt.setVerticalAlignment(BOBUIextCharFormat::AlignTop);
        table->cellAt(0, 1).setFormat(fmt);
        fmt = table->cellAt(1, 0).format();
        fmt.setVerticalAlignment(BOBUIextCharFormat::AlignBottom);
        table->cellAt(1, 0).setFormat(fmt);

        table->cellAt(0, 0).firstCursorPosition().insertText("Blah");

        BOBUIest::newRow("table-vertical-alignment") << BOBUIextDocumentFragment(&doc)
                                  << QString("<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">"
                                             "\n<tr>\n<td style=\" vertical-align:middle;\">\n"
                                             "<p DEFAULTBLOCKSTYLE>Blah</p></td>"
                                             "\n<td style=\" vertical-align:top;\"></td></tr>"
                                             "\n<tr>\n<td style=\" vertical-align:bottom;\"></td>"
                                             "\n<td></td></tr>"
                                             "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTable *table = cursor.insertTable(2, 2);
        BOBUIextTableCellFormat fmt = table->cellAt(0, 0).format().toTableCellFormat();
        fmt.setLeftPadding(1);
        table->cellAt(0, 0).setFormat(fmt);
        fmt = table->cellAt(0, 1).format().toTableCellFormat();
        fmt.setRightPadding(1);
        table->cellAt(0, 1).setFormat(fmt);
        fmt = table->cellAt(1, 0).format().toTableCellFormat();
        fmt.setTopPadding(1);
        table->cellAt(1, 0).setFormat(fmt);
        fmt = table->cellAt(1, 1).format().toTableCellFormat();
        fmt.setBottomPadding(1);
        table->cellAt(1, 1).setFormat(fmt);

        table->cellAt(0, 0).firstCursorPosition().insertText("Blah");

        BOBUIest::newRow("table-cell-paddings") << BOBUIextDocumentFragment(&doc)
                                  << QString("<table border=\"1\" style=\" border-collapse:collapse;\" cellpadding=\"4\">"
                                             "\n<tr>\n<td style=\" padding-left:1;\">\n"
                                             "<p DEFAULTBLOCKSTYLE>Blah</p></td>"
                                             "\n<td style=\" padding-right:1;\"></td></tr>"
                                             "\n<tr>\n<td style=\" padding-top:1;\"></td>"
                                             "\n<td style=\" padding-bottom:1;\"></td></tr>"
                                             "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextTableFormat fmt;
        fmt.setBorderBrush(QColor("#0000ff"));
        fmt.setBorderStyle(BOBUIextFrameFormat::BorderStyle_Solid);
        cursor.insertTable(2, 2, fmt);

        BOBUIest::newRow("tableborder") << BOBUIextDocumentFragment(&doc)
                                     << QString("<table border=\"1\" style=\" border-color:#0000ff; border-style:solid; border-collapse:collapse;\" cellpadding=\"4\">"
                                                "\n<tr>\n<td></td>\n<td></td></tr>"
                                                "\n<tr>\n<td></td>\n<td></td></tr>"
                                                "</table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        cursor.insertBlock();
        cursor.insertText("Foo");

        cursor.block().setUserState(42);

        BOBUIest::newRow("userstate") << BOBUIextDocumentFragment(&doc)
                                   << QString("EMPTYBLOCK") +
                                      QString("<p OPENDEFAULTBLOCKSTYLE -bobui-user-state:42;\">Foo</p>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextBlockFormat blockFmt;
        blockFmt.setPageBreakPolicy(BOBUIextFormat::PageBreak_AlwaysBefore);

        cursor.insertBlock(blockFmt);
        cursor.insertText("Foo");

        blockFmt.setPageBreakPolicy(BOBUIextFormat::PageBreak_AlwaysBefore | BOBUIextFormat::PageBreak_AlwaysAfter);

        cursor.insertBlock(blockFmt);
        cursor.insertText("Bar");

        BOBUIextTableFormat tableFmt;
        tableFmt.setPageBreakPolicy(BOBUIextFormat::PageBreak_AlwaysAfter);

        cursor.insertTable(1, 1, tableFmt);

        BOBUIest::newRow("pagebreak") << BOBUIextDocumentFragment(&doc)
                                   << QString("EMPTYBLOCK") +
                                      QString("<p OPENDEFAULTBLOCKSTYLE page-break-before:always;\">Foo</p>"
                                              "\n<p OPENDEFAULTBLOCKSTYLE page-break-before:always; page-break-after:always;\">Bar</p>"
                                              "\n<table border=\"1\" style=\" page-break-after:always; border-collapse:collapse;\" cellpadding=\"4\">\n<tr>\n<td></td></tr></table>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextListFormat listFmt;
        listFmt.setStyle(BOBUIextListFormat::ListDisc);

        cursor.insertList(listFmt);
        cursor.insertText("Blah");

        BOBUIest::newRow("list-ul-margin") << BOBUIextDocumentFragment(&doc)
                                        << QString("EMPTYBLOCK") +
                                           QString("<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -bobui-list-indent: 1;\">\n<li DEFAULTBLOCKSTYLE>Blah</li></ul>");
    }
    {
        CREATE_DOC_AND_CURSOR();
        const QString listHtml = "<ul><li>item-1</li><li>item-2<ul><li>item-2.1</li><li>item-2.2"
                                 "<ul><li>item-2.2.1</li></ul></li><li>item-2.3<ul><li>item-2.3.1"
                                 "</li></ul></li></ul></li><li>item-3</li></ul>";
        cursor.insertHtml(listHtml);

        BOBUIest::newRow("nested-lists-one") << BOBUIextDocumentFragment(&doc)
            << QString("<ul DEFAULTULSTYLE 1;\">\n<li style=\" margin-top:12px; margin-bottom:0px; "
                       "margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">"
                       "item-1</li>\n<li DEFAULTBLOCKSTYLE>item-2\n<ul DEFAULTULSTYLE 2;\">\n<li "
                       "DEFAULTBLOCKSTYLE>item-2.1</li>\n<li DEFAULTBLOCKSTYLE>item-2.2\n<ul "
                       "DEFAULTULSTYLE 3;\">\n<li DEFAULTBLOCKSTYLE>item-2.2.1</li></ul></li>\n"
                       "<li DEFAULTBLOCKSTYLE>item-2.3\n<ul DEFAULTULSTYLE 3;\">\n<li DEFAULTBLOCKSTYLE>"
                       "item-2.3.1</li></ul></li></ul></li>\n<li DEFAULTLASTLISTYLE>item-3</li></ul>");
    }
    {
        CREATE_DOC_AND_CURSOR();
        const QString listHtml = "<ul><li>item-1</li><li>item-2<ul><li>item-2.1</li></ul></li></ul>";
        cursor.insertHtml(listHtml);

        BOBUIest::newRow("nested-lists-two") << BOBUIextDocumentFragment(&doc)
            << QString("<ul DEFAULTULSTYLE 1;\">\n<li style=\" margin-top:12px; margin-bottom:0px; "
                       "margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">"
                       "item-1</li>\n<li DEFAULTLASTLISTYLE>item-2\n<ul DEFAULTULSTYLE 2;\">\n<li "
                       "DEFAULTBLOCKSTYLE>item-2.1</li></ul></li></ul>");
    }
    {
        CREATE_DOC_AND_CURSOR();
        const QString listHtml = "<ul><li>item-1</li><li>item-2<ul><li>item-2.1</li><li>item-2.2"
                                 "</li></ul></li></ul>";
        cursor.insertHtml(listHtml);

        BOBUIest::newRow("nested-lists-three") << BOBUIextDocumentFragment(&doc)
            << QString("<ul DEFAULTULSTYLE 1;\">\n<li style=\" margin-top:12px; margin-bottom:0px; "
                       "margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">"
                       "item-1</li>\n<li DEFAULTLASTLISTYLE>item-2\n<ul DEFAULTULSTYLE 2;\">\n<li "
                       "DEFAULTBLOCKSTYLE>item-2.1</li>\n<li DEFAULTBLOCKSTYLE>item-2.2</li></ul>"
                       "</li></ul>");
    }
    {
        CREATE_DOC_AND_CURSOR();
        const QString listHtml = "<ul><li>item-1.1</li><li>item-1.2<li></ul>"
                                 "<ul><li>item-2.1</li></ul>";
        cursor.insertHtml(listHtml);

        BOBUIest::newRow("not-nested-list") << BOBUIextDocumentFragment(&doc)
            << QString("<ul DEFAULTULSTYLE 1;\">\n<li style=\" margin-top:12px; margin-bottom:0px; "
                       "margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">"
                       "item-1.1</li>\n<li DEFAULTBLOCKSTYLE>item-1.2</li></ul>\n<ul DEFAULTULSTYLE 1;\">\n"
                       "<li style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; "
                       "margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">item-2.1</li></ul>");
    }
    {
        CREATE_DOC_AND_CURSOR();
        const QString listHtml = "<ul><li>bullet</li><li class=\"unchecked\">unchecked item</li><li class=\"checked\">checked item</li></ul>";
        cursor.insertHtml(listHtml);

        BOBUIest::newRow("list with and without checkboxes") << BOBUIextDocumentFragment(&doc)
            << QString("<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -bobui-list-indent: 1;\">\n"
                       "<li style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">bullet</li>\n"
                       "<li class=\"unchecked\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">unchecked item</li>\n"
                       "<li class=\"checked\" style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">checked item</li></ul>");
    }

    {
        CREATE_DOC_AND_CURSOR();

        BOBUIextListFormat fmt;
        fmt.setStyle(BOBUIextListFormat::ListDecimal);
        fmt.setStart(4);
        cursor.insertList(fmt);
        cursor.insertText("Blah");
        cursor.insertBlock();
        cursor.insertText("Bleh");

        BOBUIest::newRow("ordered list with start") << BOBUIextDocumentFragment(&doc)
            << QString("EMPTYBLOCK") +
               QString("<ol start=\"4\" style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -bobui-list-indent: 1;\">\n<li style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Blah</li>\n<li style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Bleh</li></ol>");
    }
}

void tst_BOBUIextDocument::toHtml()
{
    QFETCH(BOBUIextDocumentFragment, input);
    QFETCH(QString, expectedOutput);

    cursor.insertFragment(input);

    expectedOutput.prepend(htmlHead);

    expectedOutput.replace("OPENDEFAULTBLOCKSTYLE", "style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;");
    expectedOutput.replace("DEFAULTBLOCKSTYLE", "style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\"");
    expectedOutput.replace("EMPTYBLOCK", "<p style=\"-bobui-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\"><br /></p>\n");
    expectedOutput.replace("DEFAULTULSTYLE", "style=\"margin-top: 0px; margin-bottom: 0px; "
                           "margin-left: 0px; margin-right: 0px; -bobui-list-indent:");
    expectedOutput.replace("DEFAULTLASTLISTYLE", "style=\" margin-top:0px; margin-bottom:12px; "
                           "margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\"");

    if (expectedOutput.endsWith(QLatin1Char('\n')))
        expectedOutput.chop(1);
    expectedOutput.append(htmlTail);

    QString output = doc->toHtml();

    writeActualAndExpected(BOBUIest::currentDataTag(), output, expectedOutput);

    QCOMPARE(output, expectedOutput);

    QDomDocument document;
    QEXPECT_FAIL("charfmt-for-list-item",
                 "The attribute \"style\" is redefined in the generated HTML, which is not valid "
                 "according to XML standard. The new QDomDocument implementation follows the XML "
                 "standard.", Continue);
    QVERIFY2(document.setContent(output), "Output was not valid XML");
}

void tst_BOBUIextDocument::toHtml2()
{
    BOBUIextDocument doc;
    doc.setHtml("<p>text <img src=\"\">    text</p>"); // 4 spaces before the second 'text'
    BOBUIextBlock block = doc.firstBlock();
    BOBUIextBlock::Iterator iter = block.begin();
    BOBUIextFragment f = iter.fragment();
    QVERIFY(f.isValid());
    QCOMPARE(f.position(), 0);
    QCOMPARE(f.length(), 5);
    //qDebug() << block.text().mid(f.position(), f.length());

    iter++;
    f = iter.fragment();
    QVERIFY(f.isValid());
    QCOMPARE(f.position(), 5);
    QCOMPARE(f.length(), 1);
    //qDebug() << block.text().mid(f.position(), f.length());

    iter++;
    f = iter.fragment();
    //qDebug() << block.text().mid(f.position(), f.length());
    QVERIFY(f.isValid());
    QCOMPARE(f.position(), 6);
    QCOMPARE(f.length(), 5); // 1 space should be preserved.
    QCOMPARE(block.text().mid(f.position(), f.length()), QString(" text"));

    doc.setHtml("<table><tr><td>   foo</td></tr></table>    text"); // 4 spaces before the second 'text'
    block = doc.firstBlock().next();
    //qDebug() << block.text();
    QCOMPARE(block.text(), QString("foo"));

    block = block.next();
    //qDebug() << block.text();
    QCOMPARE(block.text(), QString("text"));
}

void tst_BOBUIextDocument::setFragmentMarkersInHtmlExport()
{
    {
        CREATE_DOC_AND_CURSOR();

        cursor.insertText("Leadin");
        const int startPos = cursor.position();

        cursor.insertText("Test");
        BOBUIextCharFormat fmt;
        fmt.setForeground(QColor("#00ff00"));
        cursor.insertText("Blah", fmt);

        const int endPos = cursor.position();
        cursor.insertText("Leadout", BOBUIextCharFormat());

        cursor.setPosition(startPos);
        cursor.setPosition(endPos, BOBUIextCursor::KeepAnchor);
        BOBUIextDocumentFragment fragment(cursor);

        QString expected = htmlHead;
        expected.replace(QRegularExpression("<body.*>"), QString("<body>"));
        expected += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\"><!--StartFragment-->Test<span style=\" color:#00ff00;\">Blah</span><!--EndFragment--></p>") + htmlTail;
        QCOMPARE(fragment.toHtml(), expected);
    }
    {
        CREATE_DOC_AND_CURSOR();

        cursor.insertText("Leadin");
        const int startPos = cursor.position();

        cursor.insertText("Test");

        const int endPos = cursor.position();
        cursor.insertText("Leadout", BOBUIextCharFormat());

        cursor.setPosition(startPos);
        cursor.setPosition(endPos, BOBUIextCursor::KeepAnchor);
        BOBUIextDocumentFragment fragment(cursor);

        QString expected = htmlHead;
        expected.replace(QRegularExpression("<body.*>"), QString("<body>"));
        expected += QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\"><!--StartFragment-->Test<!--EndFragment--></p>") + htmlTail;
        QCOMPARE(fragment.toHtml(), expected);
    }
}

void tst_BOBUIextDocument::setMediaRule()
{
    {
        CREATE_DOC_AND_CURSOR();
        doc.setDefaultStyleSheet("@media screen { p { background:#000000 } } @media print { p { background:#ffffff } }");
        doc.setHtml("<p>Hello World</p>");

        QString expected = htmlHead;
        expected += QString("<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px; background-color:#000000;\"><span style=\" background-color:#000000;\">Hello World</span></p>") + htmlTail;
        QCOMPARE(doc.toHtml(), expected);
    }
    {
        CREATE_DOC_AND_CURSOR();
        doc.setDefaultStyleSheet("@media screen { p { background:#000000 } } @media print { p { background:#ffffff } }");
        doc.setMetaInformation(BOBUIextDocument::CssMedia, "screen");
        doc.setHtml("<p>Hello World</p>");

        QString expected = htmlHead;
        expected += QString("<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px; background-color:#000000;\"><span style=\" background-color:#000000;\">Hello World</span></p>") + htmlTail;
        QCOMPARE(doc.toHtml(), expected);
    }
    {
        CREATE_DOC_AND_CURSOR();
        doc.setDefaultStyleSheet("@media screen { p { background:#000000 } } @media print { p { background:#ffffff } }");
        doc.setMetaInformation(BOBUIextDocument::CssMedia, "print");
        doc.setHtml("<p>Hello World</p>");

        QString expected = htmlHead;
        expected += QString("<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" background-color:#ffffff;\">Hello World</span></p>") + htmlTail;
        QCOMPARE(doc.toHtml(), expected);
    }
}

void tst_BOBUIextDocument::toHtmlBodyBgColor()
{
    CREATE_DOC_AND_CURSOR();

    cursor.insertText("Blah");

    BOBUIextFrameFormat fmt = doc.rootFrame()->frameFormat();
    fmt.setBackground(QColor("#0000ff"));
    doc.rootFrame()->setFrameFormat(fmt);

    QString expectedHtml = htmlHead;
    expectedHtml.insert(htmlHead.size() - 2, " bgcolor=\"#0000ff\"");
    expectedHtml += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Blah</p>"
             + htmlTail;

    writeActualAndExpected(BOBUIest::currentDataTag(), doc.toHtml(), expectedHtml);

    QCOMPARE(doc.toHtml(), expectedHtml);
}

void tst_BOBUIextDocument::toHtmlBodyBgColorRgba()
{
    CREATE_DOC_AND_CURSOR();

    cursor.insertText("Blah");

    BOBUIextFrameFormat fmt = doc.rootFrame()->frameFormat();
    fmt.setBackground(QColor(255, 0, 0, 51));
    doc.rootFrame()->setFrameFormat(fmt);

    QString expectedHtml = htmlHead;
    expectedHtml.insert(htmlHead.size() - 2, " bgcolor=\"rgba(255,0,0,0.2)\"");
    expectedHtml += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Blah</p>"
             + htmlTail;

    writeActualAndExpected(BOBUIest::currentDataTag(), doc.toHtml(), expectedHtml);

    QCOMPARE(doc.toHtml(), expectedHtml);
}

void tst_BOBUIextDocument::toHtmlBodyBgColorTransparent()
{
    CREATE_DOC_AND_CURSOR();

    cursor.insertText("Blah");

    BOBUIextFrameFormat fmt = doc.rootFrame()->frameFormat();
    fmt.setBackground(QColor(255, 0, 0, 0));
    doc.rootFrame()->setFrameFormat(fmt);

    QString expectedHtml = htmlHead;
    expectedHtml.insert(htmlHead.size() - 2, " bgcolor=\"transparent\"");
    expectedHtml += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Blah</p>"
             + htmlTail;

    writeActualAndExpected(BOBUIest::currentDataTag(), doc.toHtml(), expectedHtml);

    QCOMPARE(doc.toHtml(), expectedHtml);
}

void tst_BOBUIextDocument::toHtmlRootFrameProperties()
{
    CREATE_DOC_AND_CURSOR();

    BOBUIextFrameFormat fmt = doc.rootFrame()->frameFormat();
    fmt.setTopMargin(10);
    fmt.setLeftMargin(10);
    fmt.setBorder(2);
    doc.rootFrame()->setFrameFormat(fmt);

    cursor.insertText("Blah");

    QString expectedOutput("<table border=\"2\" style=\"-bobui-table-type: root; margin-top:10px; "
                           "margin-bottom:4px; margin-left:10px; margin-right:4px;\">\n"
                           "<tr>\n<td style=\"border: none;\">\n"
                           "<p DEFAULTBLOCKSTYLE>Blah</p></td></tr></table>");

    expectedOutput.prepend(htmlHead);
    expectedOutput.replace("DEFAULTBLOCKSTYLE", "style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\"");
    expectedOutput.append(htmlTail);

    writeActualAndExpected(BOBUIest::currentTestFunction(), doc.toHtml(), expectedOutput);

    QCOMPARE(doc.toHtml(), expectedOutput);
}

void tst_BOBUIextDocument::toHtmlLineHeightProperties()
{
    CREATE_DOC_AND_CURSOR();

    BOBUIextBlock block = doc.firstBlock();
    BOBUIextBlockFormat blockFormat = block.blockFormat();
    blockFormat.setLineHeight(200, BOBUIextBlockFormat::ProportionalHeight);
    cursor.setBlockFormat(blockFormat);

    cursor.insertText("Blah");
    QString expectedOutput("<p DEFAULTBLOCKSTYLE line-height:200%;\">Blah</p>");

    expectedOutput.prepend(htmlHead);
    expectedOutput.replace("DEFAULTBLOCKSTYLE", "style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;");
    expectedOutput.append(htmlTail);

    QCOMPARE(doc.toHtml(), expectedOutput);
}

void tst_BOBUIextDocument::toHtmlDefaultFontSpacingProperties()
{
    CREATE_DOC_AND_CURSOR();

    cursor.insertText("Blah");

    QFont fnt = doc.defaultFont();
    fnt.setLetterSpacing(QFont::AbsoluteSpacing, 13);
    fnt.setWordSpacing(15);
    doc.setDefaultFont(fnt);

    QString expectedOutput = htmlHead;
    expectedOutput.insert(htmlHead.size() - 3, " letter-spacing:13px; word-spacing:15px;");
    expectedOutput +=
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Blah</p>"
            + htmlTail;

    writeActualAndExpected(BOBUIest::currentTestFunction(), doc.toHtml(), expectedOutput);

    QCOMPARE(doc.toHtml(), expectedOutput);
}

void tst_BOBUIextDocument::toHtmlTextDecorationUnderline()
{
    CREATE_DOC_AND_CURSOR();

    cursor.insertText("Some text");
    QFont fnt = doc.defaultFont();
    fnt.setUnderline(true);
    doc.setDefaultFont(fnt);

    QString expectedOutput = htmlHead;
    expectedOutput.insert(htmlHead.size() - 3, " text-decoration: underline;");
    expectedOutput +=
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Some text</p>"
            + htmlTail;

    writeActualAndExpected("toHtmlTextDecorationUnderline1", doc.toHtml(), expectedOutput);

    QCOMPARE(doc.toHtml(), expectedOutput);

    BOBUIextCharFormat format;
    format.setFontUnderline(false);
    cursor.select(BOBUIextCursor::Document);
    cursor.mergeCharFormat(format);

    expectedOutput = htmlHead;
    expectedOutput.insert(htmlHead.size() - 3, " text-decoration: underline;");
    expectedOutput +=
            "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; "
            "margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">"
            "<span style=\" text-decoration:none;\">Some text</span></p>"
            + htmlTail;

    writeActualAndExpected("toHtmlTextDecorationUnderline2", doc.toHtml(), expectedOutput);

    QCOMPARE(doc.toHtml(), expectedOutput);
}

void tst_BOBUIextDocument::capitalizationHtmlInExport()
{
    doc->setPlainText("Test");

    QRegularExpression re(".*span style=\"(.*)\">Test.*");
    QCOMPARE(re.captureCount(), 1);
    QVERIFY(!re.match(doc->toHtml()).hasMatch()); // no span

    BOBUIextCursor cursor(doc);
    cursor.setPosition(4, BOBUIextCursor::KeepAnchor);
    BOBUIextCharFormat cf;
    cf.setFontCapitalization(QFont::SmallCaps);
    cursor.mergeCharFormat(cf);

    const QString smallcaps = doc->toHtml();
    auto match = re.match(doc->toHtml());
    QVERIFY(match.hasMatch());
    QCOMPARE(match.captured(1).trimmed(), QString("font-variant:small-caps;"));

    cf.setFontCapitalization(QFont::AllUppercase);
    cursor.mergeCharFormat(cf);
    const QString uppercase = doc->toHtml();
    match = re.match(doc->toHtml());
    QVERIFY(match.hasMatch());
    QCOMPARE(match.captured(1).trimmed(), QString("text-transform:uppercase;"));

    cf.setFontCapitalization(QFont::AllLowercase);
    cursor.mergeCharFormat(cf);
    const QString lowercase = doc->toHtml();
    match = re.match(doc->toHtml());
    QVERIFY(match.hasMatch());
    QCOMPARE(match.captured(1).trimmed(), QString("text-transform:lowercase;"));

    doc->setHtml(smallcaps);
    cursor.setPosition(1);
    QCOMPARE(cursor.charFormat().fontCapitalization(), QFont::SmallCaps);
    doc->setHtml(uppercase);
    QCOMPARE(cursor.charFormat().fontCapitalization(), QFont::AllUppercase);
    doc->setHtml(lowercase);
    QCOMPARE(cursor.charFormat().fontCapitalization(), QFont::AllLowercase);
}

void tst_BOBUIextDocument::wordspacingHtmlExport()
{
    doc->setPlainText("Test");

    QRegularExpression re(".*span style=\"(.*)\">Test.*");
    QCOMPARE(re.captureCount(), 1);
    QVERIFY(!re.match(doc->toHtml()).hasMatch()); // no span

    BOBUIextCursor cursor(doc);
    cursor.setPosition(4, BOBUIextCursor::KeepAnchor);
    BOBUIextCharFormat cf;
    cf.setFontWordSpacing(4);
    cursor.mergeCharFormat(cf);

    auto match = re.match(doc->toHtml());
    QVERIFY(match.hasMatch());
    QCOMPARE(match.captured(1).trimmed(), QString("word-spacing:4px;"));

    cf.setFontWordSpacing(-8.5);
    cursor.mergeCharFormat(cf);

    match = re.match(doc->toHtml());
    QVERIFY(match.hasMatch());
    QCOMPARE(match.captured(1).trimmed(), QString("word-spacing:-8.5px;"));
}

class CursorPosSignalSpy : public QObject
{
    Q_OBJECT
public:
    CursorPosSignalSpy(BOBUIextDocument *doc)
    {
        calls = 0;
        connect(doc, SIGNAL(cursorPositionChanged(BOBUIextCursor)),
                this, SLOT(cursorPositionChanged(BOBUIextCursor)));
    }

    int calls;

private slots:
    void cursorPositionChanged(const BOBUIextCursor &)
    {
        ++calls;
    }
};

void tst_BOBUIextDocument::cursorPositionChanged()
{
    CursorPosSignalSpy spy(doc);

    cursor.insertText("Test");
    QCOMPARE(spy.calls, 1);

    spy.calls = 0;
    BOBUIextCursor unrelatedCursor(doc);
    unrelatedCursor.insertText("Blah");
    QCOMPARE(spy.calls, 2);

    spy.calls = 0;
    cursor.insertText("Blah");
    QCOMPARE(spy.calls, 1);

    spy.calls = 0;
    cursor.movePosition(BOBUIextCursor::PreviousCharacter);
    QCOMPARE(spy.calls, 0);
}

void tst_BOBUIextDocument::cursorPositionChangedOnSetText()
{
    CursorPosSignalSpy spy(doc);

    // doc has one BOBUIextCursor stored in the
    // cursor member variable, thus the signal
    // gets emitted once.

    doc->setPlainText("Foo\nBar\nBaz\nBlub\nBlah");

    QCOMPARE(spy.calls, 1);

    spy.calls = 0;
    doc->setHtml("<p>Foo<p>Bar<p>Baz<p>Blah");

    QCOMPARE(spy.calls, 1);
}

void tst_BOBUIextDocument::textFrameIterator()
{
    cursor.insertTable(1, 1);

    int blockCount = 0;
    int frameCount = 0;

    for (BOBUIextFrame::Iterator frameIt = doc->rootFrame()->begin();
         !frameIt.atEnd(); ++frameIt) {
        if (frameIt.currentFrame())
            ++frameCount;
        else if (frameIt.currentBlock().isValid())
            ++blockCount;

    }

    QEXPECT_FAIL("", "This is currently worked around in the html export but needs fixing!", Continue);
    QCOMPARE(blockCount, 0);
    QCOMPARE(frameCount, 1);
}

class TestSyntaxHighlighter : public QObject
{
    Q_OBJECT
public:
    inline TestSyntaxHighlighter(BOBUIextDocument *doc) : QObject(doc), ok(false) {}

    bool ok;

private slots:
    inline void markBlockDirty(int from, int charsRemoved, int charsAdded)
    {
        Q_UNUSED(charsRemoved);
        Q_UNUSED(charsAdded);
        BOBUIextDocument *doc = static_cast<BOBUIextDocument *>(parent());
        BOBUIextBlock block = doc->findBlock(from);

        BOBUIestDocumentLayout *lout = qobject_cast<BOBUIestDocumentLayout *>(doc->documentLayout());
        lout->called = false;

        doc->markContentsDirty(block.position(), block.length());

        ok = (lout->called == false);
    }

    inline void modifyBlockAgain(int from, int charsRemoved, int charsAdded)
    {
        Q_UNUSED(charsRemoved);
        Q_UNUSED(charsAdded);
        BOBUIextDocument *doc = static_cast<BOBUIextDocument *>(parent());
        BOBUIextBlock block = doc->findBlock(from);
        BOBUIextCursor cursor(block);

        BOBUIestDocumentLayout *lout = qobject_cast<BOBUIestDocumentLayout *>(doc->documentLayout());
        lout->called = false;

        cursor.insertText("Foo");

        ok = (lout->called == true);
    }
};

void tst_BOBUIextDocument::markContentsDirty()
{
    BOBUIestDocumentLayout *lout = new BOBUIestDocumentLayout(doc);
    doc->setDocumentLayout(lout);
    TestSyntaxHighlighter *highlighter = new TestSyntaxHighlighter(doc);
    connect(doc, SIGNAL(contentsChange(int,int,int)),
            highlighter, SLOT(markBlockDirty(int,int,int)));

    highlighter->ok = false;
    cursor.insertText("Some dummy text blah blah");
    QVERIFY(highlighter->ok);

    disconnect(doc, SIGNAL(contentsChange(int,int,int)),
               highlighter, SLOT(markBlockDirty(int,int,int)));
    connect(doc, SIGNAL(contentsChange(int,int,int)),
            highlighter, SLOT(modifyBlockAgain(int,int,int)));
    highlighter->ok = false;
    cursor.insertText("FooBar");
    QVERIFY(highlighter->ok);

    lout->called = false;

    doc->markContentsDirty(1, 4);

    QVERIFY(lout->called);
}

void tst_BOBUIextDocument::clonePreservesMetaInformation()
{
    const QString title("Foobar");
    const QString url("about:blank");
    const QString media("print");
    doc->setHtml("<html><head><title>" + title + "</title></head><body>Hrm</body></html>");
    doc->setMetaInformation(BOBUIextDocument::DocumentUrl, url);
    doc->setMetaInformation(BOBUIextDocument::CssMedia, media);
    QCOMPARE(doc->metaInformation(BOBUIextDocument::DocumentTitle), title);
    QCOMPARE(doc->metaInformation(BOBUIextDocument::DocumentUrl), url);
    QCOMPARE(doc->metaInformation(BOBUIextDocument::CssMedia), media);

    BOBUIextDocument *clone = doc->clone();
    QCOMPARE(clone->metaInformation(BOBUIextDocument::DocumentTitle), title);
    QCOMPARE(clone->metaInformation(BOBUIextDocument::DocumentUrl), url);
    QCOMPARE(clone->metaInformation(BOBUIextDocument::CssMedia), media);
    delete clone;
}

void tst_BOBUIextDocument::clonePreservesPageSize()
{
    QSizeF sz(100., 100.);
    doc->setPageSize(sz);
    BOBUIextDocument *clone = doc->clone();
    QCOMPARE(clone->pageSize(), sz);
    delete clone;
}

void tst_BOBUIextDocument::clonePreservesPageBreakPolicies()
{
    BOBUIextTableFormat tableFmt;
    tableFmt.setPageBreakPolicy(BOBUIextFormat::PageBreak_AlwaysAfter);

    BOBUIextBlockFormat blockFmt;
    blockFmt.setPageBreakPolicy(BOBUIextFormat::PageBreak_AlwaysBefore);

    BOBUIextCursor cursor(doc);

    cursor.setBlockFormat(blockFmt);
    cursor.insertText("foo");
    cursor.insertTable(2, 2, tableFmt);

    BOBUIextDocument *clone = doc->clone();
    QCOMPARE(clone->begin().blockFormat().pageBreakPolicy(), BOBUIextFormat::PageBreak_AlwaysBefore);
    QVERIFY(!clone->rootFrame()->childFrames().isEmpty());
    QCOMPARE(clone->rootFrame()->childFrames().first()->frameFormat().pageBreakPolicy(), BOBUIextFormat::PageBreak_AlwaysAfter);
    delete clone;
}

void tst_BOBUIextDocument::clonePreservesDefaultFont()
{
    QFont f = doc->defaultFont();
    QVERIFY(f.pointSize() != 100);
    f.setPointSize(100);
    doc->setDefaultFont(f);
    BOBUIextDocument *clone = doc->clone();
    QCOMPARE(clone->defaultFont(), f);
    delete clone;
}

void tst_BOBUIextDocument::clonePreservesResources()
{
    QUrl testUrl(":/foobar");
    QVariant testResource("hello world");

    doc->addResource(BOBUIextDocument::ImageResource, testUrl, testResource);
    BOBUIextDocument *clone = doc->clone();
    QVERIFY(clone->resource(BOBUIextDocument::ImageResource, testUrl) == testResource);
    delete clone;
}

void tst_BOBUIextDocument::clonePreservesUserStates()
{
    BOBUIextCursor cursor(doc);
    cursor.insertText("bla bla bla");
    cursor.block().setUserState(1);
    cursor.insertBlock();
    cursor.insertText("foo bar");
    cursor.block().setUserState(2);
    cursor.insertBlock();
    cursor.insertText("no user state");

    BOBUIextDocument *clone = doc->clone();
    BOBUIextBlock b1 = doc->begin(), b2 = clone->begin();
    while (b1 != doc->end()) {
        b1 = b1.next();
        b2 = b2.next();
        QCOMPARE(b1.userState(), b2.userState());
    }
    QCOMPARE(b2, clone->end());
    delete clone;
}

void tst_BOBUIextDocument::clonePreservesRootFrameFormat()
{
    doc->setPlainText("Hello");
    BOBUIextFrameFormat fmt = doc->rootFrame()->frameFormat();
    fmt.setMargin(200);
    doc->rootFrame()->setFrameFormat(fmt);
    QCOMPARE(doc->rootFrame()->frameFormat().margin(), qreal(200));
    BOBUIextDocument *copy = doc->clone();
    QCOMPARE(copy->rootFrame()->frameFormat().margin(), qreal(200));
    delete copy;
}

void tst_BOBUIextDocument::clonePreservesIndentWidth()
{
    doc->setIndentWidth(42);
    BOBUIextDocument *clone = doc->clone();
    QCOMPARE(clone->indentWidth(), qreal(42));
    delete clone;
}

void tst_BOBUIextDocument::clonePreservesFormatsWhenEmpty()
{
    BOBUIextDocument document;
    BOBUIextCursor cursor(&document);

    // Change a few char format attributes
    BOBUIextCharFormat charFormat;
    charFormat.setFontPointSize(charFormat.fontPointSize() + 1);
    charFormat.setFontWeight(charFormat.fontWeight() + 1);
    cursor.setBlockCharFormat(charFormat);

    // Change a few block format attributes
    BOBUIextBlockFormat blockFormat;
    blockFormat.setAlignment(BobUI::AlignRight); // The default is BobUI::AlignLeft
    blockFormat.setIndent(blockFormat.indent() + 1);
    cursor.setBlockFormat(blockFormat);

    auto clone = document.clone();
    BOBUIextCursor cloneCursor(clone);

    QCOMPARE(cloneCursor.blockCharFormat().fontPointSize(), charFormat.fontPointSize());
    QCOMPARE(cloneCursor.blockCharFormat().fontWeight(), charFormat.fontWeight());
    QCOMPARE(cloneCursor.blockFormat().alignment(), blockFormat.alignment());
    QCOMPARE(cloneCursor.blockFormat().indent(), blockFormat.indent());
    delete clone;
}

void tst_BOBUIextDocument::blockCount()
{
    QCOMPARE(doc->blockCount(), 1);
    cursor.insertBlock();
    QCOMPARE(doc->blockCount(), 2);
    cursor.insertBlock();
    QCOMPARE(doc->blockCount(), 3);
    cursor.insertText("blah blah");
    QCOMPARE(doc->blockCount(), 3);
    doc->undo();
    doc->undo();
    QCOMPARE(doc->blockCount(), 2);
    doc->undo();
    QCOMPARE(doc->blockCount(), 1);
}

void tst_BOBUIextDocument::resolvedFontInEmptyFormat()
{
    QFont font;
    font.setPointSize(42);
    doc->setDefaultFont(font);
    BOBUIextCharFormat fmt = doc->begin().charFormat();
    QVERIFY(fmt.properties().isEmpty());
    QCOMPARE(fmt.font(), font);
}

void tst_BOBUIextDocument::defaultRootFrameMargin()
{
    QCOMPARE(doc->rootFrame()->frameFormat().margin(), 4.0);
}

class TestDocument : public BOBUIextDocument
{
public:
    inline TestDocument(const QUrl &testUrl, const QString &testString)
       : url(testUrl), string(testString), resourceLoaded(false) {}

    bool hasResourceCached();

protected:
    virtual QVariant loadResource(int type, const QUrl &name) override;

private:
    QUrl url;
    QString string;
    bool resourceLoaded;
};

bool TestDocument::hasResourceCached()
{
    resourceLoaded = false;
    resource(BOBUIextDocument::ImageResource, url);
    return !resourceLoaded;
}

QVariant TestDocument::loadResource(int type, const QUrl &name)
{
    if (type == BOBUIextDocument::ImageResource
        && name == url) {
        resourceLoaded = true;
        return string;
    }
    return BOBUIextDocument::loadResource(type, name);
}

void tst_BOBUIextDocument::clearResources()
{
    // regular resource for BOBUIextDocument
    QUrl testUrl(":/foobar");
    QVariant testResource("hello world");

    // implicitly cached resource, initially loaded through TestDocument::loadResource()
    QUrl cacheUrl(":/blub");
    QString cacheResource("mah");

    TestDocument doc(cacheUrl, cacheResource);
    doc.addResource(BOBUIextDocument::ImageResource, testUrl, testResource);

    QVERIFY(doc.resource(BOBUIextDocument::ImageResource, testUrl) == testResource);

    doc.setPlainText("Hah");
    QVERIFY(doc.resource(BOBUIextDocument::ImageResource, testUrl) == testResource);

    doc.setHtml("<b>Mooo</b><img src=\":/blub\"/>");
    QVERIFY(doc.resource(BOBUIextDocument::ImageResource, testUrl) == testResource);
    QVERIFY(doc.resource(BOBUIextDocument::ImageResource, cacheUrl) == cacheResource);

    doc.clear();
    QVERIFY(!doc.resource(BOBUIextDocument::ImageResource, testUrl).isValid());
    QVERIFY(!doc.hasResourceCached());
    doc.clear();

    doc.setHtml("<b>Mooo</b><img src=\":/blub\"/>");
    QVERIFY(doc.resource(BOBUIextDocument::ImageResource, cacheUrl) == cacheResource);

    doc.setPlainText("Foob");
    QVERIFY(!doc.hasResourceCached());
}

void tst_BOBUIextDocument::setPlainText()
{
    doc->setPlainText("Hello World");
    QString s("");
    doc->setPlainText(s);
    QCOMPARE(doc->toPlainText(), s);
}

void tst_BOBUIextDocument::toPlainText_data()
{
    BOBUIest::addColumn<QString>("html");
    BOBUIest::addColumn<QString>("expectedPlainText");

    BOBUIest::newRow("nbsp") << "Hello&nbsp;World" << "Hello World";
    BOBUIest::newRow("empty_div") << "<div></div>hello" << "hello";
    BOBUIest::newRow("br_and_p") << "<p>first<br></p><p>second<br></p>" << "first\n\nsecond\n";
    BOBUIest::newRow("div") << "first<div>second<br>third</div>fourth" << "first\nsecond\nthird\nfourth";                             // <div> and </div> become newlines...
    BOBUIest::newRow("br_text_end_of_div") << "<div><div>first<br>moretext</div>second<br></div>" << "first\nmoretext\nsecond\n";     // ... when there is text before <div>
    BOBUIest::newRow("br_end_of_div_like_gmail") << "<div><div><div>first<br></div>second<br></div>third<br></div>" << "first\nsecond\nthird\n"; // ... and when there is text before </div>
    BOBUIest::newRow("p_and_div") << "<div><div>first<p>second</p></div>third</div>" << "first\nsecond\nthird";
}

void tst_BOBUIextDocument::toPlainText()
{
    QFETCH(QString, html);
    QFETCH(QString, expectedPlainText);

    doc->setHtml(html);
    QCOMPARE(doc->toPlainText(), expectedPlainText);
}

void tst_BOBUIextDocument::toRawText()
{
    doc->setHtml("&nbsp;");

    QString rawText = doc->toRawText();
    QCOMPARE(rawText.size(), 1);
    QCOMPARE(rawText.at(0).unicode(), ushort(QChar::Nbsp));
}


void tst_BOBUIextDocument::deleteTextObjectsOnClear()
{
    QPointer<BOBUIextTable> table = cursor.insertTable(2, 2);
    QVERIFY(!table.isNull());
    doc->clear();
    QVERIFY(table.isNull());
}

void tst_BOBUIextDocument::defaultStyleSheet()
{
    const QColor green("green");
    const QString sheet("p { background-color: green; }");
    QVERIFY(doc->defaultStyleSheet().isEmpty());
    doc->setDefaultStyleSheet(sheet);
    QCOMPARE(doc->defaultStyleSheet(), sheet);

    cursor.insertHtml("<p>test");
    BOBUIextBlockFormat fmt = doc->begin().blockFormat();
    QCOMPARE(fmt.background().color(), green);

    doc->clear();
    cursor.insertHtml("<p>test");
    fmt = doc->begin().blockFormat();
    QCOMPARE(fmt.background().color(), green);

    BOBUIextDocument *clone = doc->clone();
    QCOMPARE(clone->defaultStyleSheet(), sheet);
    cursor = BOBUIextCursor(clone);
    cursor.insertHtml("<p>test");
    fmt = clone->begin().blockFormat();
    QCOMPARE(fmt.background().color(), green);
    delete clone;

    cursor = BOBUIextCursor(doc);
    cursor.insertHtml("<p>test");
    fmt = doc->begin().blockFormat();
    QCOMPARE(fmt.background().color(), green);

    doc->clear();
    cursor.insertHtml("<style>p { background-color: red; }</style><p>test");
    fmt = doc->begin().blockFormat();
    QCOMPARE(fmt.background().color(), QColor(BobUI::red));

    doc->clear();
    doc->setDefaultStyleSheet("invalid style sheet....");
    cursor.insertHtml("<p>test");
    fmt = doc->begin().blockFormat();
    QVERIFY(fmt.background().color() != QColor("green"));
}

void tst_BOBUIextDocument::defaultTableStyle_data()
{
    BOBUIest::addColumn<QString>("css");
    BOBUIest::addColumn<QString>("html");
    BOBUIest::addColumn<QList<QBrush>>("borderBrushes");

    const QString htmlHeader(R"(
        <tr>
            <th>1</th> <th>2</th>
        </tr>
    )");

    const QString htmlCells(R"(
        <tr>
            <td>A</td> <td>B</td>
        </tr>
    )");

    const QString cssEachSide = R"({
        border-top-color: red;
        border-bottom-color: blue;
        border-left-color: green;
        border-right-color: yellow;
    })";
    const QString cssOneColor = R"({ border-color: red; })";
    const QString cssFourColors = R"({ border-color: red blue green yellow; })";

    BOBUIest::addRow("td, each side") << QString("td ") + cssEachSide
        << htmlCells
        << QList<QBrush>{BobUI::red, BobUI::blue, QColor("green"), BobUI::yellow};

    BOBUIest::addRow("th, each side") << QString("th ") + cssEachSide
        << htmlHeader
        << QList<QBrush>{BobUI::red, BobUI::blue, QColor("green"), BobUI::yellow};

    BOBUIest::addRow("th+td, each side") << QString("th %1 td %1").arg(cssEachSide)
        << htmlHeader + htmlCells
        << QList<QBrush>{BobUI::red, BobUI::blue, QColor("green"), BobUI::yellow};

    BOBUIest::addRow("td, one color") << QString("td ") + cssOneColor
        << htmlCells
        << QList<QBrush>{BobUI::red, BobUI::red, BobUI::red, BobUI::red};

    BOBUIest::addRow("th, one color") << QString("th ") + cssOneColor
        << htmlHeader
        << QList<QBrush>{BobUI::red, BobUI::red, BobUI::red, BobUI::red};

    BOBUIest::addRow("th+td, one color") << QString("th %1 td %1").arg(cssOneColor)
        << htmlHeader + htmlCells
        << QList<QBrush>{BobUI::red, BobUI::red, BobUI::red, BobUI::red};

    // css order is top, right, bottom, left
    BOBUIest::addRow("td, four colors") << QString("td ") + cssFourColors
        << htmlCells
        << QList<QBrush>{BobUI::red, QColor("green"), BobUI::yellow, BobUI::blue};

}

void tst_BOBUIextDocument::defaultTableStyle()
{
    QFETCH(QString, css);
    QFETCH(QString, html);
    QFETCH(QList<QBrush>, borderBrushes);

    CREATE_DOC_AND_CURSOR();
    doc.setDefaultStyleSheet(css);
    doc.setHtml(QString("<html><body><table>%1</table></body></html>").arg(html));

    const BOBUIextFrame *frame = doc.rootFrame();
    const BOBUIextTable *table = nullptr;
    for (auto it = frame->begin(); !table && !it.atEnd(); ++it)
        table = qobject_cast<const BOBUIextTable*>(it.currentFrame());
    QVERIFY(table);

    const QList<BOBUIextFormat::Property> brushProperties = {
        BOBUIextFormat::TableCellTopBorderBrush,
        BOBUIextFormat::TableCellBottomBorderBrush,
        BOBUIextFormat::TableCellLeftBorderBrush,
        BOBUIextFormat::TableCellRightBorderBrush
    };

    for (int row = 0; row < table->rows(); ++row) {
        for (int column = 0; column < table->columns(); ++column) {
            auto cellDetails = qScopeGuard([&]{
                qWarning("Failure was in cell %d/%d", row, column);
            });
            const BOBUIextTableCell cell = table->cellAt(row, column);
            const BOBUIextTableCellFormat cellFormat = cell.format().toTableCellFormat();
            QList<QBrush> brushes;
            for (const auto side : brushProperties) {
                QVariant sideProperty = cellFormat.property(side);
                QVERIFY(sideProperty.isValid());
                QVERIFY(sideProperty.typeId() == qMetaTypeId<QBrush>());
                brushes << sideProperty.value<QBrush>();
            }
            auto errorDetails = qScopeGuard([&]{
                if (brushes.size() != borderBrushes.size()) {
                    qWarning("Different count: %lld vs %lld", brushes.size(), borderBrushes.size());
                    return;
                }
                for (int i = 0; i < brushes.size(); ++i) {
                    QString side;
                    QDebug(&side) << BOBUIextFormat::Property(BOBUIextFormat::TableCellTopBorderBrush + i);
                    QString actual;
                    QDebug(&actual) << brushes.at(i);
                    QString expected;
                    QDebug(&expected) << borderBrushes.at(i);
                    if (expected != actual) {
                        qWarning("\n  %s:\n\tActual:  %s\n\tExpected:%s", qPrintable(side),
                                qPrintable(actual), qPrintable(expected));
                    }
                }
            });
            QVERIFY2(borderBrushes == brushes, // QCOMPARE doesn't generate helpful output anyway
                     qPrintable(QString("for cell %1/%2").arg(row).arg(column)));
            errorDetails.dismiss();
            cellDetails.dismiss();
        }
    }
}

void tst_BOBUIextDocument::maximumBlockCount()
{
    QCOMPARE(doc->maximumBlockCount(), 0);
    QVERIFY(doc->isUndoRedoEnabled());

    cursor.insertBlock();
    cursor.insertText("Blah");
    cursor.insertBlock();
    cursor.insertText("Foo");
    QCOMPARE(doc->blockCount(), 3);
    QCOMPARE(doc->toPlainText(), QString("\nBlah\nFoo"));

    doc->setMaximumBlockCount(1);
    QVERIFY(!doc->isUndoRedoEnabled());

    QCOMPARE(doc->blockCount(), 1);
    QCOMPARE(doc->toPlainText(), QString("Foo"));

    cursor.insertBlock();
    cursor.insertText("Hello");
    doc->setMaximumBlockCount(1);
    QCOMPARE(doc->blockCount(), 1);
    QCOMPARE(doc->toPlainText(), QString("Hello"));

    doc->setMaximumBlockCount(100);
    for (int i = 0; i < 1000; ++i) {
        cursor.insertBlock();
        cursor.insertText("Blah)");
        QVERIFY(doc->blockCount() <= 100);
    }

    cursor.movePosition(BOBUIextCursor::End);
    QCOMPARE(cursor.blockNumber(), 99);
    BOBUIextCharFormat fmt;
    fmt.setFontItalic(true);
    cursor.setBlockCharFormat(fmt);
    cursor.movePosition(BOBUIextCursor::Start);
    QVERIFY(!cursor.blockCharFormat().fontItalic());

    doc->setMaximumBlockCount(1);
    QVERIFY(cursor.blockCharFormat().fontItalic());

    cursor.insertTable(2, 2);
    QCOMPARE(doc->blockCount(), 6);
    cursor.insertBlock();
    QCOMPARE(doc->blockCount(), 1);
}

void tst_BOBUIextDocument::adjustSize()
{
    // avoid ugly tooltips like in task 125583
    QString text("Test Text");
    doc->setPlainText(text);
    doc->rootFrame()->setFrameFormat(BOBUIextFrameFormat());
    doc->adjustSize();
    QCOMPARE(doc->size().width(), doc->idealWidth());
}

void tst_BOBUIextDocument::initialUserData()
{
    doc->setPlainText("Hello");
    BOBUIextBlock block = doc->begin();
    block.setUserData(new BOBUIextBlockUserData);
    QVERIFY(block.userData());
    doc->documentLayout();
    QVERIFY(block.userData());
    doc->setDocumentLayout(new BOBUIestDocumentLayout(doc));
    QVERIFY(!block.userData());
}

void tst_BOBUIextDocument::html_defaultFont()
{
    QFont f;
    f.setItalic(true);
    f.setWeight(QFont::Bold);
    doc->setDefaultFont(f);
    doc->setPlainText("Test");

    QString bodyPart = QString::fromLatin1("<body style=\" font-family:'%1'; font-size:%2; "
                                           "font-weight:%3; font-style:italic;\">")
                               .arg(f.family())
                               .arg(cssFontSizeString(f))
                               .arg(f.weight());

    QString html = doc->toHtml();
    if (!html.contains(bodyPart)) {
        qDebug() << "html:" << html;
        qDebug() << "expected body:" << bodyPart;
        QVERIFY(html.contains(bodyPart));
    }

    if (html.contains("span"))
        qDebug() << "html:" << html;
    QVERIFY(!html.contains("<span style"));
}

void tst_BOBUIextDocument::blockCountChanged()
{
    QSignalSpy spy(doc, SIGNAL(blockCountChanged(int)));

    doc->setPlainText("Foo");

    QCOMPARE(doc->blockCount(), 1);
    QCOMPARE(spy.size(), 0);

    spy.clear();

    doc->setPlainText("Foo\nBar");
    QCOMPARE(doc->blockCount(), 2);
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).value(0).toInt(), 2);

    spy.clear();

    cursor.movePosition(BOBUIextCursor::End);
    cursor.insertText("Blahblah");

    QCOMPARE(spy.size(), 0);

    cursor.insertBlock();
    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).value(0).toInt(), 3);

    spy.clear();
    doc->undo();

    QCOMPARE(spy.size(), 1);
    QCOMPARE(spy.at(0).value(0).toInt(), 2);
}

void tst_BOBUIextDocument::nonZeroDocumentLengthOnClear()
{
    BOBUIestDocumentLayout *lout = new BOBUIestDocumentLayout(doc);
    doc->setDocumentLayout(lout);

    doc->clear();
    QVERIFY(lout->called);
    QVERIFY(!lout->lastDocumentLengths.contains(0));
}

void tst_BOBUIextDocument::setTextPreservesUndoRedoEnabled()
{
    QVERIFY(doc->isUndoRedoEnabled());

    doc->setPlainText("Test");

    QVERIFY(doc->isUndoRedoEnabled());

    doc->setUndoRedoEnabled(false);
    QVERIFY(!doc->isUndoRedoEnabled());
    doc->setPlainText("Test2");
    QVERIFY(!doc->isUndoRedoEnabled());

    doc->setHtml("<p>hello");
    QVERIFY(!doc->isUndoRedoEnabled());
}

void tst_BOBUIextDocument::firstLast()
{
    QCOMPARE(doc->blockCount(), 1);
    QCOMPARE(doc->firstBlock(), doc->lastBlock());

    doc->setPlainText("Hello\nTest\nWorld");

    QCOMPARE(doc->blockCount(), 3);
    QVERIFY(doc->firstBlock() != doc->lastBlock());

    QCOMPARE(doc->firstBlock().text(), QString("Hello"));
    QCOMPARE(doc->lastBlock().text(), QString("World"));

    // manual forward loop
    BOBUIextBlock block = doc->firstBlock();

    QVERIFY(block.isValid());
    QCOMPARE(block.text(), QString("Hello"));

    block = block.next();

    QVERIFY(block.isValid());
    QCOMPARE(block.text(), QString("Test"));

    block = block.next();

    QVERIFY(block.isValid());
    QCOMPARE(block.text(), QString("World"));

    block = block.next();
    QVERIFY(!block.isValid());

    // manual backward loop
    block = doc->lastBlock();

    QVERIFY(block.isValid());
    QCOMPARE(block.text(), QString("World"));

    block = block.previous();

    QVERIFY(block.isValid());
    QCOMPARE(block.text(), QString("Test"));

    block = block.previous();

    QVERIFY(block.isValid());
    QCOMPARE(block.text(), QString("Hello"));

    block = block.previous();
    QVERIFY(!block.isValid());
}

const QString backgroundImage_html("<body><table><tr><td background=\"foo.png\">Blah</td></tr></table></body>");

void tst_BOBUIextDocument::backgroundImage_checkExpectedHtml(const BOBUIextDocument &doc)
{
    QString expectedHtml = htmlHead +
            "<table border=\"0\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"2\" cellpadding=\"0\">"
            "\n<tr>\n<td background=\"foo.png\">"
            "\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -bobui-block-indent:0; text-indent:0px;\">Blah</p>"
            "</td></tr></table>" + htmlTail;

    writeActualAndExpected(BOBUIest::currentTestFunction(), doc.toHtml(), expectedHtml);

    QCOMPARE(doc.toHtml(), expectedHtml);
}

void tst_BOBUIextDocument::buildRegExpData()
{
    BOBUIest::addColumn<QString>("haystack");
    BOBUIest::addColumn<QString>("needle");
    BOBUIest::addColumn<int>("flags");
    BOBUIest::addColumn<int>("from");
    BOBUIest::addColumn<int>("anchor");
    BOBUIest::addColumn<int>("position");

    // match integers 0 to 99
    BOBUIest::newRow("1") << "23" << "^\\d\\d?$" << int(BOBUIextDocument::FindCaseSensitively) << 0 << 0 << 2;
    // match ampersands but not &amp;
    BOBUIest::newRow("2") << "His &amp; hers & theirs" << "&(?!amp;)"<< int(BOBUIextDocument::FindCaseSensitively) << 0 << 15 << 16;
    //backward search
    BOBUIest::newRow("3") << QString::fromLatin1("HelloBlahWorld Blah Hah")
                              << "h" << int(BOBUIextDocument::FindBackward) << 18 << 8 << 9;
}

void tst_BOBUIextDocument::backgroundImage_toHtml()
{
    CREATE_DOC_AND_CURSOR();

    doc.setHtml(backgroundImage_html);
    backgroundImage_checkExpectedHtml(doc);
}

void tst_BOBUIextDocument::backgroundImage_toHtml2()
{
    CREATE_DOC_AND_CURSOR();

    cursor.insertHtml(backgroundImage_html);
    backgroundImage_checkExpectedHtml(doc);
}

void tst_BOBUIextDocument::backgroundImage_clone()
{
    CREATE_DOC_AND_CURSOR();

    doc.setHtml(backgroundImage_html);
    BOBUIextDocument *clone = doc.clone();
    backgroundImage_checkExpectedHtml(*clone);
    delete clone;
}

void tst_BOBUIextDocument::backgroundImage_copy()
{
    CREATE_DOC_AND_CURSOR();

    doc.setHtml(backgroundImage_html);
    BOBUIextDocumentFragment fragment(&doc);

    {
        CREATE_DOC_AND_CURSOR();

        cursor.insertFragment(fragment);
        backgroundImage_checkExpectedHtml(doc);
    }
}

void tst_BOBUIextDocument::documentCleanup()
{
    BOBUIextDocument doc;
    BOBUIextCursor cursor(&doc);
    cursor.insertText("d\nfoo\nbar\n");
    doc.documentLayout(); // forces relayout

    // remove char 1
    cursor.setPosition(0);
    QSizeF size = doc.documentLayout()->documentSize();
    cursor.deleteChar();
    // the size should be unchanged.
    QCOMPARE(doc.documentLayout()->documentSize(), size);
}

void tst_BOBUIextDocument::characterAt()
{
    BOBUIextDocument doc;
    BOBUIextCursor cursor(&doc);
    QString text("12345\n67890");
    cursor.insertText(text);
    int length = doc.characterCount();
    QCOMPARE(length, text.size() + 1);
    QCOMPARE(doc.characterAt(length-1), QChar(QChar::ParagraphSeparator));
    QCOMPARE(doc.characterAt(-1), QChar());
    QCOMPARE(doc.characterAt(length), QChar());
    QCOMPARE(doc.characterAt(length + 1), QChar());
    for (int i = 0; i < text.size(); ++i) {
        QChar c = text.at(i);
        if (c == QLatin1Char('\n'))
            c = QChar(QChar::ParagraphSeparator);
        QCOMPARE(doc.characterAt(i), c);
    }
}

void tst_BOBUIextDocument::revisions()
{
    BOBUIextDocument doc;
    BOBUIextCursor cursor(&doc);
    QString text("Hello World");
    QCOMPARE(doc.firstBlock().revision(), 0);
    cursor.insertText(text);
    QCOMPARE(doc.firstBlock().revision(), 1);
    cursor.setPosition(6);
    cursor.insertBlock();
    QCOMPARE(cursor.block().previous().revision(), 2);
    QCOMPARE(cursor.block().revision(), 2);
    cursor.insertText("candle");
    QCOMPARE(cursor.block().revision(), 3);
    cursor.movePosition(BOBUIextCursor::EndOfBlock);
    cursor.insertBlock(); // we are at the block end
    QCOMPARE(cursor.block().previous().revision(), 3);
    QCOMPARE(cursor.block().revision(), 4);
    cursor.insertText("lightbulb");
    QCOMPARE(cursor.block().revision(), 5);
    cursor.movePosition(BOBUIextCursor::StartOfBlock);
    cursor.insertBlock(); // we are the block start
    QCOMPARE(cursor.block().previous().revision(), 6);
    QCOMPARE(cursor.block().revision(), 5);
}

void tst_BOBUIextDocument::revisionWithUndoCompressionAndUndo()
{
    BOBUIextDocument doc;
    BOBUIextCursor cursor(&doc);
    cursor.insertText("This is the beginning of it all.");
    QCOMPARE(doc.firstBlock().revision(), 1);
    QCOMPARE(doc.revision(), 1);
    cursor.insertBlock();
    QCOMPARE(doc.revision(), 2);
    cursor.insertText("this");
    QCOMPARE(doc.revision(), 3);
    cursor.insertText("is");
    QCOMPARE(doc.revision(), 4);
    cursor.insertText("compressed");
    QCOMPARE(doc.revision(), 5);
    doc.undo();
    QCOMPARE(doc.revision(), 6);
    QCOMPARE(doc.toPlainText(), QString("This is the beginning of it all.\n"))  ;
    cursor.setPosition(0);
    QCOMPARE(doc.firstBlock().revision(), 1);
    cursor.insertText("Very beginnig");
    QCOMPARE(doc.firstBlock().revision(), 7);
    doc.undo();
    QCOMPARE(doc.revision(), 8);
    QCOMPARE(doc.firstBlock().revision(), 1);

    cursor.beginEditBlock();
    cursor.insertText("Hello");
    cursor.insertBlock();
    cursor.insertText("world");
    cursor.endEditBlock();
    QCOMPARE(doc.revision(), 9);
    doc.undo();
    QCOMPARE(doc.revision(), 10);


}

void tst_BOBUIextDocument::testUndoCommandAdded()
{
    QVERIFY(doc);
    QSignalSpy spy(doc, SIGNAL(undoCommandAdded()));
    QVERIFY(spy.isValid());
    QVERIFY(spy.isEmpty());

    cursor.insertText("a");
    QCOMPARE(spy.size(), 1);
    cursor.insertText("b"); // should be merged
    QCOMPARE(spy.size(), 1);
    cursor.insertText("c"); // should be merged
    QCOMPARE(spy.size(), 1);
    QCOMPARE(doc->toPlainText(), QString("abc"));
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString(""));

    doc->clear();
    spy.clear();
    cursor.insertText("aaa");
    QCOMPARE(spy.size(), 1);

    spy.clear();
    cursor.insertText("aaaa\nbcd");
    QCOMPARE(spy.size(), 1);

    spy.clear();
    cursor.beginEditBlock();
    cursor.insertText("aa");
    cursor.insertText("bbb\n");
    cursor.setCharFormat(BOBUIextCharFormat());
    cursor.insertText("\nccc");
    QVERIFY(spy.isEmpty());
    cursor.endEditBlock();
    QCOMPARE(spy.size(), 1);

    spy.clear();
    cursor.insertBlock();
    QCOMPARE(spy.size(), 1);

    spy.clear();
    cursor.setPosition(5);
    QVERIFY(spy.isEmpty());
    cursor.setCharFormat(BOBUIextCharFormat());
    QVERIFY(spy.isEmpty());
    cursor.setPosition(10, BOBUIextCursor::KeepAnchor);
    QVERIFY(spy.isEmpty());
    BOBUIextCharFormat cf;
    cf.setFontItalic(true);
    cursor.mergeCharFormat(cf);
    QCOMPARE(spy.size(), 1);

    spy.clear();
    doc->undo();
    QCOMPARE(spy.size(), 0);
    doc->undo();
    QCOMPARE(spy.size(), 0);
    spy.clear();
    doc->redo();
    QCOMPARE(spy.size(), 0);
    doc->redo();
    QCOMPARE(spy.size(), 0);
}

void tst_BOBUIextDocument::testUndoBlocks()
{
    QVERIFY(doc);
    cursor.insertText("Hello World");
    cursor.insertText("period");
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString(""));
    cursor.insertText("Hello World");
    cursor.insertText("One\nTwo\nThree");
    QCOMPARE(doc->toPlainText(), QString("Hello WorldOne\nTwo\nThree"));
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString("Hello World"));
    cursor.insertText("One\nTwo\nThree");
    cursor.insertText("Trailing text");
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString("Hello WorldOne\nTwo\nThree"));
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString("Hello World"));
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString(""));

    cursor.insertText("town");
    cursor.beginEditBlock(); // Edit block 1 - Deletion/Insertion
    cursor.setPosition(0, BOBUIextCursor::KeepAnchor);
    cursor.insertText("r");
    cursor.endEditBlock();
    cursor.insertText("est"); // Merged into edit block 1
    QCOMPARE(doc->toPlainText(), QString("rest"));
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString("town"));
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString(""));

    // This case would not happen in practice. If the user typed out this text, it would all be part of one
    // edit block. This would cause the undo to clear all text. But for the purpose of testing the beginEditBlock
    // and endEditBlock calls with respect to bobuiextdocument this is tested.
    cursor.insertText("quod");
    cursor.beginEditBlock(); // Edit block 1 - Insertion
    cursor.insertText(" erat");
    cursor.endEditBlock();
    cursor.insertText(" demonstrandum"); // Merged into edit block 1
    QCOMPARE(doc->toPlainText(), QString("quod erat demonstrandum"));
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString("quod"));
    doc->undo();
    QCOMPARE(doc->toPlainText(), QString(""));
}

class Receiver : public QObject
{
    Q_OBJECT
 public:
    QString first;
 public slots:
    void cursorPositionChanged() {
        if (first.isEmpty())
            first = QLatin1String("cursorPositionChanged");
    }

    void contentsChange() {
        if (first.isEmpty())
            first = QLatin1String("contentsChanged");
    }
};

void tst_BOBUIextDocument::receiveCursorPositionChangedAfterContentsChange()
{
    QVERIFY(doc);
    doc->setDocumentLayout(new MyAbstractTextDocumentLayout(doc));
    Receiver rec;
    connect(doc, SIGNAL(cursorPositionChanged(BOBUIextCursor)),
            &rec, SLOT(cursorPositionChanged()));
    connect(doc, SIGNAL(contentsChange(int,int,int)),
            &rec, SLOT(contentsChange()));
    cursor.insertText("Hello World");
    QCOMPARE(rec.first, QString("contentsChanged"));
}

void tst_BOBUIextDocument::BOBUIBUG25778_pixelSizeFromHtml()
{
    BOBUIextDocument document1;
    BOBUIextDocument document2;

    document1.setHtml("<span style=\"font-size: 24px\">Foobar</span>");
    document2.setHtml(document1.toHtml());

    BOBUIextCursor cursor(&document2);
    QCOMPARE(cursor.charFormat().font().pixelSize(), 24);
}

void tst_BOBUIextDocument::copiedFontSize()
{
    BOBUIextDocument documentInput;
    BOBUIextDocument documentOutput;

    QFont fontInput;
    fontInput.setPixelSize(24);

    BOBUIextCursor cursorInput(&documentInput);
    BOBUIextCharFormat formatInput = cursorInput.charFormat();
    formatInput.setFont(fontInput);
    cursorInput.insertText("Should be the same font", formatInput);
    cursorInput.select(BOBUIextCursor::Document);

    BOBUIextDocumentFragment fragmentInput(cursorInput);
    QString html =  fragmentInput.toHtml();

    BOBUIextCursor cursorOutput(&documentOutput);
    BOBUIextDocumentFragment fragmentOutput = BOBUIextDocumentFragment::fromHtml(html);
    cursorOutput.insertFragment(fragmentOutput);

    QCOMPARE(cursorOutput.charFormat().font().pixelSize(), 24);
}

void tst_BOBUIextDocument::htmlExportImportBlockCount()
{
    BOBUIextDocument document;
    {
        BOBUIextCursor cursor(&document);
        cursor.insertText("Foo");
        cursor.insertBlock();
        cursor.insertBlock();
        cursor.insertBlock();
        cursor.insertBlock();
        cursor.insertText("Bar");
    }

    QCOMPARE(document.blockCount(), 5);
    QString html = document.toHtml();

    document.clear();
    document.setHtml(html);

    QCOMPARE(document.blockCount(), 5);
}

void tst_BOBUIextDocument::BOBUIBUG27354_spaceAndSoftSpace()
{
    BOBUIextDocument document;
    {
        BOBUIextCursor cursor(&document);
        BOBUIextBlockFormat blockFormat;
        blockFormat.setAlignment(BobUI::AlignJustify);
        cursor.mergeBlockFormat(blockFormat);
        cursor.insertText("ac");
        cursor.insertBlock();
        cursor.insertText(" ");
        cursor.insertText(QChar(0x2028));
    }

    // Trigger justification of text
    QImage image(1000, 1000, QImage::Format_ARGB32);
    image.fill(0);
    {
        QPainter p(&image);
        document.drawContents(&p, image.rect());
    }
    {
        // If no p tag is specified it should not be inheriting it
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">p { line-height: 200% }</style></head><body>Foo<ul><li>First</li></ul></body></html>");
        BOBUIextBlock block = td.begin();
        while (block.isValid()) {
            BOBUIextBlockFormat fmt = block.blockFormat();
            QCOMPARE(fmt.lineHeightType(), int(BOBUIextBlockFormat::SingleHeight));
            QCOMPARE(fmt.lineHeight(), qreal(0));
            block = block.next();
        }
    }
    {
        BOBUIextDocument td;
        td.setHtml("<html><head></head><body><p>Foo</p><ul><li>First</li></ul></body></html>");
        QList<double> originalMargins;
        BOBUIextBlock block = td.begin();
        while (block.isValid()) {
            originalMargins << block.blockFormat().topMargin();
            block = block.next();
        }
        originalMargins[0] = 85;
        td.setHtml("<html><head><style type=\"text/css\">body { margin-top: 85px; }</style></head><body><p>Foo</p><ul><li>First</li></ul></body></html>");
        block = td.begin();
        int count = 0;
        while (block.isValid()) {
            BOBUIextBlockFormat fmt = block.blockFormat();
            QCOMPARE(fmt.topMargin(), originalMargins.at(count++));
            block = block.next();
        }
    }
}

class BaseDocument : public BOBUIextDocument
{
public:
    QUrl loadedResource() const { return resourceUrl; }

    QVariant loadResource(int type, const QUrl &name) override
    {
        resourceUrl = name;
        return BOBUIextDocument::loadResource(type, name);
    }

private:
    QUrl resourceUrl;
};

void tst_BOBUIextDocument::baseUrl_data()
{
    BOBUIest::addColumn<QUrl>("base");
    BOBUIest::addColumn<QUrl>("resource");
    BOBUIest::addColumn<QUrl>("loaded");

    BOBUIest::newRow("1") << QUrl() << QUrl("images/logo.png") << QUrl("images/logo.png");
    BOBUIest::newRow("2") << QUrl("file:///path/to/content") << QUrl("images/logo.png") << QUrl("file:///path/to/images/logo.png");
    BOBUIest::newRow("3") << QUrl("file:///path/to/content/") << QUrl("images/logo.png") << QUrl("file:///path/to/content/images/logo.png");
    BOBUIest::newRow("4") << QUrl("file:///path/to/content/images") << QUrl("images/logo.png") << QUrl("file:///path/to/content/images/logo.png");
    BOBUIest::newRow("5") << QUrl("file:///path/to/content/images/") << QUrl("images/logo.png") << QUrl("file:///path/to/content/images/images/logo.png");
    BOBUIest::newRow("6") << QUrl("file:///path/to/content/images") << QUrl("../images/logo.png") << QUrl("file:///path/to/images/logo.png");
    BOBUIest::newRow("7") << QUrl("file:///path/to/content/images/") << QUrl("../images/logo.png") << QUrl("file:///path/to/content/images/logo.png");
    BOBUIest::newRow("8") << QUrl("file:///path/to/content/index.html") << QUrl("images/logo.png") << QUrl("file:///path/to/content/images/logo.png");
}

void tst_BOBUIextDocument::baseUrl()
{
    QFETCH(QUrl, base);
    QFETCH(QUrl, resource);
    QFETCH(QUrl, loaded);

    BaseDocument document;
    QVERIFY(!document.baseUrl().isValid());
    document.setBaseUrl(base);
    QCOMPARE(document.baseUrl(), base);

    document.setHtml(QLatin1String("<img src='") + resource.toString() + QLatin1String("'/>"));
    document.resource(BOBUIextDocument::ImageResource, resource);
    QCOMPARE(document.loadedResource(), loaded);
}

void tst_BOBUIextDocument::BOBUIBUG28998_linkColor()
{
    QPalette pal;
    pal.setColor(QPalette::Link, QColor("tomato"));
    QGuiApplication::setPalette(pal);

    BOBUIextDocument doc;
    doc.setHtml("<a href=\"http://www.bobui-project.org\">BobUI</a>");

    QCOMPARE(doc.blockCount(), 1);
    BOBUIextBlock block = doc.firstBlock();
    QVERIFY(block.isValid());

    BOBUIextFragment fragment = block.begin().fragment();
    QVERIFY(fragment.isValid());

    BOBUIextCharFormat format = fragment.charFormat();
    QVERIFY(format.isValid());
    QVERIFY(format.isAnchor());
    QCOMPARE(format.anchorHref(), QStringLiteral("http://www.bobui-project.org"));

    QCOMPARE(format.foreground(), pal.link());
}

class ContentsChangeHandler : public QObject
{
    Q_OBJECT
public:
    ContentsChangeHandler(BOBUIextDocument *doc)
        : verticalMovementX(-1)
        , doc(doc)
    {
        connect(doc, SIGNAL(contentsChange(int,int,int)),
                this, SLOT(saveModifiedText(int, int, int)));
    }

private slots:
    void saveModifiedText(int from, int /*charsRemoved*/, int charsAdded)
    {
        BOBUIextCursor tmp(doc);
        tmp.setPosition(from);
        tmp.setPosition(from + charsAdded, BOBUIextCursor::KeepAnchor);
        text = tmp.selectedText();
        verticalMovementX = tmp.verticalMovementX();
    }

public:
    QString text;
    int verticalMovementX;
private:
    BOBUIextDocument *doc;
};

void tst_BOBUIextDocument::textCursorUsageWithinContentsChange()
{
    // force creation of layout
    doc->documentLayout();

    BOBUIextCursor cursor(doc);
    cursor.insertText("initial text");

    ContentsChangeHandler handler(doc);

    cursor.insertText("new text");

    QCOMPARE(handler.text, QString("new text"));
    QCOMPARE(handler.verticalMovementX, -1);
}

void tst_BOBUIextDocument::cssInheritance()
{
    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 200% }</style></head><body>"
            "<p>Foo</p><p>Bar</p><p>Baz</p></body></html>");
        BOBUIextBlock block = td.begin();
        while (block.isValid()) {
            BOBUIextBlockFormat fmt = block.blockFormat();
            QCOMPARE(fmt.lineHeightType(), int(BOBUIextBlockFormat::ProportionalHeight));
            QCOMPARE(fmt.lineHeight(), qreal(200));
            block = block.next();
        }
    }
    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 200% } p { line-height: 300% }</style></head><body>"
                   "<p style=\"line-height: 40px\">Foo</p><p>Bar</p><p>Baz</p></body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat fmt = block.blockFormat();
        QCOMPARE(fmt.lineHeightType(), int(BOBUIextBlockFormat::MinimumHeight));
        QCOMPARE(fmt.lineHeight(), qreal(40));
        block = block.next();
        fmt = block.blockFormat();
        QCOMPARE(fmt.lineHeightType(), int(BOBUIextBlockFormat::ProportionalHeight));
        QCOMPARE(fmt.lineHeight(), qreal(300));
    }
    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { font-weight: bold; background-color: #ff0000 }</style></head><body>"
            "<p>Foo</p><p>Bar</p><p>Baz</p></body></html>");
        BOBUIextBlock block = td.begin();
        while (block.isValid()) {
            QCOMPARE(block.blockFormat().background(), QBrush());
            QVERIFY(block.charFormat().font().bold());
            block = block.next();
        }
    }
    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { font-style: italic; font-weight: normal; }</style></head><body>"
            "<table><tr><th>Foo</th></tr><tr><td>Bar</td></tr></table></body></html>");
        BOBUIextBlock block = td.begin();
        // First is the table
        BOBUIextCharFormat fmt = block.charFormat();
        QVERIFY(!fmt.font().bold());
        QVERIFY(fmt.font().italic());
        // Then the th
        block = block.next();
        fmt = block.charFormat();
        QVERIFY(fmt.font().bold());
        QVERIFY(fmt.font().italic());
        // Then the td
        block = block.next();
        fmt = block.charFormat();
        QVERIFY(!fmt.font().bold());
        QVERIFY(fmt.font().italic());
    }
    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">b { font-style: italic; font-weight: normal; }</style></head><body>"
            "<p>This should be <b>bold</b></p></body></html>");
        BOBUIextBlock block = td.begin();
        // First is the p
        BOBUIextCharFormat fmt = block.charFormat();
        QVERIFY(!fmt.font().bold());
        BOBUIextBlock::iterator it = block.begin();
        // The non bold text is first
        BOBUIextFragment currentFragment = it.fragment();
        QVERIFY(currentFragment.isValid());
        fmt = currentFragment.charFormat();
        QVERIFY(!fmt.font().bold());
        ++it;
        QVERIFY(!it.atEnd());
        // Now check the "bold" text
        currentFragment = it.fragment();
        QVERIFY(currentFragment.isValid());
        fmt = currentFragment.charFormat();
        QVERIFY(!fmt.font().bold());
        QVERIFY(fmt.font().italic());
    }
    {
        BOBUIextDocument td;
        td.setHtml("<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"test.css\"></head><body>"
            "<p>This should be <b>bold</b></p></body></html>");
        BOBUIextBlock block = td.begin();
        // First is the p
        BOBUIextCharFormat fmt = block.charFormat();
        QVERIFY(!fmt.font().bold());
        BOBUIextBlock::iterator it = block.begin();
        // The non bold text is first
        BOBUIextFragment currentFragment = it.fragment();
        QVERIFY(currentFragment.isValid());
        fmt = currentFragment.charFormat();
        QVERIFY(!fmt.font().bold());
        ++it;
        QVERIFY(!it.atEnd());
        // Now check the bold text
        currentFragment = it.fragment();
        QVERIFY(currentFragment.isValid());
        fmt = currentFragment.charFormat();
        QVERIFY(fmt.font().bold());
    }
}

void tst_BOBUIextDocument::lineHeightType()
{
    {
        BOBUIextDocument td;
        td.setHtml("<html><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::SingleHeight));
        QCOMPARE(format.lineHeight(), 0.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 40px; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::MinimumHeight));
        QCOMPARE(format.lineHeight(), 40.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 200%; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::ProportionalHeight));
        QCOMPARE(format.lineHeight(), 200.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 200%; -bobui-line-height-type: single; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::SingleHeight));
        QCOMPARE(format.lineHeight(), 200.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 40px; -bobui-line-height-type: proportional; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::ProportionalHeight));
        QCOMPARE(format.lineHeight(), 40.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 10; -bobui-line-height-type: fixed; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::FixedHeight));
        QCOMPARE(format.lineHeight(), 10.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { -bobui-line-height-type: fixed; line-height: 10; -bobui-line-height-type: fixed; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::FixedHeight));
        QCOMPARE(format.lineHeight(), 10.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { -bobui-line-height-type: proportional; line-height: 3; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::ProportionalHeight));
        QCOMPARE(format.lineHeight(), 3.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 2.5; -bobui-line-height-type: proportional; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::ProportionalHeight));
        QCOMPARE(format.lineHeight(), 2.5);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 33; -bobui-line-height-type: minimum; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::MinimumHeight));
        QCOMPARE(format.lineHeight(), 33.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { -bobui-line-height-type: fixed; line-height: 200%; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::FixedHeight));
        QCOMPARE(format.lineHeight(), 200.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { -bobui-line-height-type: fixed; line-height: 200px; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::FixedHeight));
        QCOMPARE(format.lineHeight(), 200.0);
    }
}

void tst_BOBUIextDocument::cssLineHeightMultiplier()
{
    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body { line-height: 10; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::ProportionalHeight));
        QCOMPARE(format.lineHeight(), 1000.0);
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><head><style type=\"text/css\">body {line-height: 1.38; }</style></head><body>Foobar</body></html>");
        BOBUIextBlock block = td.begin();
        BOBUIextBlockFormat format = block.blockFormat();
        QCOMPARE(int(format.lineHeightType()), int(BOBUIextBlockFormat::ProportionalHeight));
        QCOMPARE(format.lineHeight(), 138.0);
    }
}

void tst_BOBUIextDocument::fontTagFace()
{
    {
        BOBUIextDocument td;
        td.setHtml("<html><body><font face='Times'>Foobar</font></body></html>");
        BOBUIextFragment fragment = td.begin().begin().fragment();
        BOBUIextCharFormat format = fragment.charFormat();
        QCOMPARE(format.fontFamilies().toStringList().value(0, QString()), QLatin1String("Times"));
    }

    {
        BOBUIextDocument td;
        td.setHtml("<html><body><font face='Times, serif'>Foobar</font></body></html>");
        BOBUIextFragment fragment = td.begin().begin().fragment();
        BOBUIextCharFormat format = fragment.charFormat();
        QCOMPARE(format.fontFamilies().toStringList().value(0, QString()), QLatin1String("Times"));
        QStringList expectedFamilies = { QLatin1String("Times"), QLatin1String("serif") };
        QCOMPARE(format.fontFamilies().toStringList(), expectedFamilies);
    }
}

void tst_BOBUIextDocument::mergeFontFamilies()
{
    BOBUIextDocument td;
    td.setHtml(QLatin1String(
                   "<html><body>"
                   "<span style=\" font-family:'MS Shell Dlg 2';\">Hello world</span>"
                   "</body></html>"));

    BOBUIextCharFormat newFormat;
    newFormat.setFontFamilies({QLatin1String("Jokerman")});

    BOBUIextCursor cursor = BOBUIextCursor(&td);
    cursor.setPosition(0);
    cursor.setPosition(QByteArray("Hello World").size(), BOBUIextCursor::KeepAnchor);
    cursor.mergeCharFormat(newFormat);

    QVERIFY(td.toHtml().contains(QLatin1String("font-family:'Jokerman';")));

    BOBUIextCharFormat newFormatFamilies;
    newFormatFamilies.setFontFamilies({ QLatin1String("Arial"), QLatin1String("Helvetica") });
    cursor.mergeCharFormat(newFormatFamilies);

    QVERIFY(td.toHtml().contains(QLatin1String("font-family:'Arial','Helvetica'")));

    newFormatFamilies.setFontFamilies({ QLatin1String("Arial"), QLatin1String("Jokerman"), QLatin1String("Helvetica") });
    cursor.mergeCharFormat(newFormatFamilies);

    QVERIFY(td.toHtml().contains(QLatin1String("font-family:'Arial','Jokerman','Helvetica'")));
}

void tst_BOBUIextDocument::clearUndoRedoStacks()
{
    BOBUIextDocument doc;
    BOBUIextCursor c(&doc);
    c.insertText(QStringLiteral("lorem ipsum"));
    QVERIFY(doc.isUndoAvailable());
    doc.clearUndoRedoStacks(BOBUIextDocument::UndoStack); // Don't crash
    QVERIFY(!doc.isUndoAvailable());
}

void tst_BOBUIextDocument::resourceProvider()
{

    int providerCalled = 0;
    QUrl providerUrl;
    auto provider = [&](const QUrl &url){
        providerUrl = url;
        ++providerCalled;
        return QVariant(42);
    };
    const QUrl url("test://img");
    const QString html = QLatin1String("<img src='%1'/>").arg(url.toString());

    BOBUIextDocument doc;
    QVERIFY(!doc.resourceProvider());
    doc.setResourceProvider(provider);
    QVERIFY(doc.resourceProvider());

    doc.setHtml(html);
    const QVariant res = doc.resource(BOBUIextDocument::UserResource, url);
    QVERIFY(res.isValid());
    QCOMPARE(providerUrl, url);
    QCOMPARE(providerCalled, 1);

    QVERIFY(!BOBUIextDocument::defaultResourceProvider());
    BOBUIextDocument::setDefaultResourceProvider(provider);
    QVERIFY(BOBUIextDocument::defaultResourceProvider());

    BOBUIextDocument doc2;
    QVERIFY(!doc2.resourceProvider());
    doc2.setHtml(html);
    QVariant res2 = doc2.resource(BOBUIextDocument::UserResource, url);
    QCOMPARE(res2, res);
    QCOMPARE(providerCalled, 2);
}

void tst_BOBUIextDocument::contentsChangeIndices_data()
{
    BOBUIest::addColumn<QString>("html");
    // adding list entries change the entire block, so change position is
    // not the same as the cursor position if this value is >= 0
    BOBUIest::addColumn<int>("expectedBegin");

    BOBUIest::addRow("text") << "Test" << -1;
    BOBUIest::addRow("unnumbered list") << "<ul><li>Test</li></ul>" << 0;
    BOBUIest::addRow("numbered list") << "<ol><li>Test</li></ol>" << 0;
    BOBUIest::addRow("table") << "<table><tr><td>Test</td></tr></table>" << -1;
}

void tst_BOBUIextDocument::contentsChangeIndices()
{
    QFETCH(QString, html);
    QFETCH(int, expectedBegin);

    BOBUIextDocument doc;
    BOBUIestDocumentLayout *layout = new BOBUIestDocumentLayout(&doc);
    doc.setDocumentLayout(layout);
    doc.setHtml(QString("<html><body>%1</body></html>").arg(html));

    int documentLength = 0;
    int cursorLength = 0;
    int changeBegin = 0;
    int changeRemoved = 0;
    int changeAdded = 0;
    connect(&doc, &BOBUIextDocument::contentsChange, this, [&](int pos, int removed, int added){
        documentLength = doc.characterCount();

        BOBUIextCursor cursor(&doc);
        cursor.movePosition(BOBUIextCursor::End);
        // includes end-of-paragraph character
        cursorLength = cursor.position() + 1;

        changeBegin = pos;
        changeRemoved = removed;
        changeAdded = added;
    });

    BOBUIextCursor cursor(&doc);
    cursor.movePosition(BOBUIextCursor::End);
    if (expectedBegin < 0)
        expectedBegin = cursor.position();
    cursor.insertBlock();

    const int changeEnd = changeBegin + changeAdded;

    QVERIFY(documentLength > 0);
    QCOMPARE(documentLength, cursorLength);
    QVERIFY(documentLength >= changeEnd);
    QCOMPARE(changeBegin, expectedBegin);
    QCOMPARE(changeAdded - changeRemoved, 1);
}

void tst_BOBUIextDocument::insertHtmlWithComments_data()
{
    BOBUIest::addColumn<QString>("html");
    BOBUIest::addColumn<QStringList>("expectedBlocks");

    BOBUIest::newRow("commentless") << "<p>first</p><p>second</p><p>third</p>"
                                 << QStringList { "first", "second", "third" };
    BOBUIest::newRow("normal") << "<p>first</p><!--<p>second</p>--><p>third</p>"
                            << QStringList { "first", "third" };
    BOBUIest::newRow("nonClosing") << "<p>first</p><!--<p>second</p><p>third</p>"
                                << QStringList { "first" };
    BOBUIest::newRow("immediatelyClosing") << "<p>first</p><!----><p>second</p><p>third</p>"
                                        << QStringList { "first", "second", "third" };
    BOBUIest::newRow("fake") << "<p>first</p><!-<p>second</p><p>third</p>"
                          << QStringList { "first", "second", "third" };
    BOBUIest::newRow("endingNonExistant") << "<p>first</p>--><p>second</p><p>third</p>"
                                       << QStringList { "first", "-->", "second", "third" };
}

void tst_BOBUIextDocument::insertHtmlWithComments()
{
    QFETCH(QString, html);
    QFETCH(QStringList, expectedBlocks);

    BOBUIextDocument doc;
    doc.setHtml(html);

    QCOMPARE(doc.blockCount(), expectedBlocks.size());

    QStringList blockContent;
    auto currentBlock = doc.begin();
    while (currentBlock != doc.end()) {
        blockContent.append(currentBlock.text());
        currentBlock = currentBlock.next();
    }

    QCOMPARE(blockContent, expectedBlocks);
}

void tst_BOBUIextDocument::delayedLayout()
{
    BOBUIextDocument doc;
    doc.setHtml("<html>Foobar</html>");
    QCOMPARE(doc.blockCount(), 1);

    doc.setLayoutEnabled(false);

    // Force creation of a layout
    QVERIFY(doc.documentLayout());

    BOBUIextBlock block = doc.begin();
    BOBUIextLayout *layout = block.layout();
    QCOMPARE(layout->lineCount(), 0); // layout didn't happen yet

    doc.setLayoutEnabled(true);
    QCOMPARE(layout->lineCount(), 1); // layout happened
}

void tst_BOBUIextDocument::undoContentChangeIndices() // BOBUIBUG-113865
{
    BOBUIextDocument doc;
    BOBUIestDocumentLayout *layout = new BOBUIestDocumentLayout(&doc);
    QString content = QString("<html><body>"
                              "<ul><li>Undo</li></ul>"
                              "<ul><li>operation</li></ul>"
                              "<ul><li>of</li></ul>"
                              "<ul><li>unnumbered</li></ul>"
                              "<ul><li>lists</li></ul>"
                              "<ul><li>shows</li></ul>"
                              "<ul><li>invalid</li></ul>"
                              "<ul><li>content</li></ul>"
                              "<ul><li>indices</li></ul>"
                              "</body></html>");
    doc.setDocumentLayout(layout);
    doc.setHtml(content);

    // Select the entire document content
    BOBUIextCursor cursor(&doc);
    cursor.select(BOBUIextCursor::Document);
    cursor.removeSelectedText();

    // Undo above operation
    doc.undo();

    // Move the cursor to the end
    cursor.movePosition(BOBUIextCursor::End);
    cursor.insertHtml(content);

    // Select the whole document and remove the content
    cursor.select(BOBUIextCursor::Document);
    cursor.removeSelectedText();

    int documentLength = 0;
    int changeRemoved = 0;
    int changeAdded = 0;
    int changePos = 0;
    connect(&doc, &BOBUIextDocument::contentsChange, this, [&](int pos, int removed, int added){
        documentLength = doc.characterCount();
        changeRemoved = removed;
        changeAdded = added;
        changePos = pos;
    });

    // Undo above operation
    doc.undo();

    const int changeEnd = changeAdded + changeRemoved;

    QVERIFY(documentLength > 0);
    QCOMPARE(changePos, 0);
    QVERIFY(changeRemoved >= 0);
    QVERIFY(documentLength >= changeEnd);
}

void tst_BOBUIextDocument::restoreStrokeFromHtml()
{
    BOBUIextDocument document;
    BOBUIextCursor textCursor(&document);
    BOBUIextCharFormat textOutline;

    // Set stroke color and width
    {
        QPen pen(BobUI::red, 2.3, BobUI::SolidLine);
        textOutline.setTextOutline(pen);
        textCursor.insertText("Outlined text", textOutline);
    }

    // Set Cap and Join styles
    {
        QPen pen;
        pen.setCapStyle(BobUI::FlatCap);
        pen.setJoinStyle(BobUI::RoundJoin);
        textOutline.setTextOutline(pen);
        textCursor.insertBlock();
        textCursor.insertText("Cap and Join Style", textOutline);
    }

    // Set Miter limit
    {
        QPen pen;
        pen.setJoinStyle(BobUI::MiterJoin);
        pen.setMiterLimit(4);
        textOutline.setTextOutline(pen);
        textCursor.insertBlock();
        textCursor.insertText("Miter Limit", textOutline);
    }

    // Set Dash Array and Dash Offset
    {
        QPen pen;
        QList<qreal> pattern;
        const int dash = 2;
        const int gap = 4;
        pattern << dash << gap << dash << gap << dash << gap;
        pen.setDashPattern(pattern);
        pen.setDashOffset(3);
        textOutline.setTextOutline(pen);
        textCursor.insertBlock();
        textCursor.insertText("Dash Pattern", textOutline);
    }

    {
        BOBUIextDocument otherDocument;
        otherDocument.setHtml(document.toHtml());
        QCOMPARE(otherDocument.blockCount(), document.blockCount());

        BOBUIextBlock block;
        BOBUIextFragment fragment;
        BOBUIextCharFormat fmt;
        QPen pen;

        {
            block = otherDocument.findBlockByNumber(0);
            fragment = block.begin().fragment();
            QCOMPARE(fragment.text(), QStringLiteral("Outlined text"));
            fmt = fragment.charFormat();
            QVERIFY(fmt.hasProperty(BOBUIextCharFormat::TextOutline));
            pen = fmt.textOutline();
            QCOMPARE(pen.color(), QColor(BobUI::red));
            QCOMPARE(pen.widthF(), 2.3);
        }

        {
            block = otherDocument.findBlockByNumber(1);
            qDebug() << block.text();
            fragment = block.begin().fragment();
            QCOMPARE(fragment.text(), QStringLiteral("Cap and Join Style"));
            fmt = fragment.charFormat();
            QVERIFY(fmt.hasProperty(BOBUIextCharFormat::TextOutline));
            pen = fmt.textOutline();
            QCOMPARE(pen.capStyle(), BobUI::FlatCap);
            QCOMPARE(pen.joinStyle(), BobUI::RoundJoin);
        }

        {
            block = otherDocument.findBlockByNumber(2);
            fragment = block.begin().fragment();
            QCOMPARE(fragment.text(), QStringLiteral("Miter Limit"));
            fmt = fragment.charFormat();
            QVERIFY(fmt.hasProperty(BOBUIextCharFormat::TextOutline));
            pen = fmt.textOutline();
            QCOMPARE(pen.joinStyle(), BobUI::MiterJoin);
            QCOMPARE(pen.miterLimit(), 4);
        }


        {
            block = otherDocument.findBlockByNumber(3);
            fragment = block.begin().fragment();
            QCOMPARE(fragment.text(), QStringLiteral("Dash Pattern"));
            fmt = fragment.charFormat();
            QVERIFY(fmt.hasProperty(BOBUIextCharFormat::TextOutline));
            pen = fmt.textOutline();
            QCOMPARE(pen.dashOffset(), 3);
            QList<qreal> pattern;
            const int dash = 2;
            const int gap = 4;
            pattern << dash << gap << dash << gap << dash << gap;
            QCOMPARE(pen.dashPattern(), pattern);
        }
    }
}

void tst_BOBUIextDocument::restoreForegroundGradientFromHtml()
{
    BOBUIextDocument document;

    BOBUIextCursor textCursor(&document);

    BOBUIextCharFormat foregroundGradient;
    QLinearGradient lg;
    lg.setColorAt(0.0, BobUI::green);
    lg.setColorAt(1.0, BobUI::blue);
    lg.setStart(QPointF(0,0));
    lg.setFinalStop(QPointF(800, 1000));
    foregroundGradient.setForeground(QBrush(lg));
    textCursor.insertText("Linear gradient text\n", foregroundGradient);

    QRadialGradient rg;
    rg.setCoordinateMode(QGradient::ObjectBoundingMode);
    rg.setSpread(QGradient::ReflectSpread);
    rg.setColorAt(0.0, BobUI::green);
    rg.setColorAt(1.0, BobUI::blue);
    QPointF center(0.5, 0.5);
    rg.setCenter(center);
    rg.setFocalPoint(center);
    rg.setRadius(0.5);
    foregroundGradient.setForeground(QBrush(rg));
    textCursor.insertText("Radial gradient text\n", foregroundGradient);

    QConicalGradient cg;
    cg.setCoordinateMode(QGradient::ObjectMode);
    cg.setSpread(QGradient::RepeatSpread);
    cg.setColorAt(0.0, BobUI::green);
    cg.setColorAt(1.0, BobUI::blue);
    cg.setCenter(QPointF(0.5, 0.5));
    cg.setAngle(0.0);
    foregroundGradient.setForeground(QBrush(cg));
    textCursor.insertText("Conical gradient text\n", foregroundGradient);

    {
        BOBUIextDocument otherDocument;
        otherDocument.setHtml(document.toHtml());

        QCOMPARE(otherDocument.blockCount(), document.blockCount());

        BOBUIextBlock block = otherDocument.firstBlock();
        BOBUIextFragment fragment = block.begin().fragment();

        QCOMPARE(fragment.text(), QStringLiteral("Linear gradient text"));

        BOBUIextCharFormat fmt = fragment.charFormat();
        QVERIFY(fmt.hasProperty(BOBUIextCharFormat::ForegroundBrush));

        QBrush brush = fmt.foreground();
        QCOMPARE(brush.style(), BobUI::LinearGradientPattern);
        QCOMPARE(brush.gradient()->coordinateMode(), lg.coordinateMode());
        QCOMPARE(brush.gradient()->spread(), lg.spread());
        QCOMPARE(brush.gradient()->stops().size(), lg.stops().size());
        QCOMPARE(static_cast<const QLinearGradient *>(brush.gradient())->start(), lg.start());
        QCOMPARE(static_cast<const QLinearGradient *>(brush.gradient())->finalStop(), lg.finalStop());

        block = block.next();
        fragment = block.begin().fragment();

        fmt = fragment.charFormat();
        QVERIFY(fmt.hasProperty(BOBUIextCharFormat::ForegroundBrush));

        brush = fmt.foreground();
        QCOMPARE(brush.style(), BobUI::RadialGradientPattern);
        QCOMPARE(brush.gradient()->coordinateMode(), rg.coordinateMode());
        QCOMPARE(brush.gradient()->spread(), rg.spread());
        QCOMPARE(static_cast<const QRadialGradient *>(brush.gradient())->center(), rg.center());
        QCOMPARE(static_cast<const QRadialGradient *>(brush.gradient())->focalPoint(), rg.focalPoint());
        QCOMPARE(static_cast<const QRadialGradient *>(brush.gradient())->radius(), rg.radius());

        block = block.next();
        fragment = block.begin().fragment();

        fmt = fragment.charFormat();
        QVERIFY(fmt.hasProperty(BOBUIextCharFormat::ForegroundBrush));

        brush = fmt.foreground();
        QCOMPARE(brush.style(), BobUI::ConicalGradientPattern);
        QCOMPARE(brush.gradient()->coordinateMode(), cg.coordinateMode());
        QCOMPARE(brush.gradient()->spread(), cg.spread());
        QCOMPARE(static_cast<const QConicalGradient *>(brush.gradient())->center(), cg.center());
        QCOMPARE(static_cast<const QConicalGradient *>(brush.gradient())->angle(), cg.angle());
    }
}

BOBUIEST_MAIN(tst_BOBUIextDocument)
#include "tst_bobuiextdocument.moc"
