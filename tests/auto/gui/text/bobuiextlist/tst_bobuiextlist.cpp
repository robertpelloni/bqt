// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include <bobuiextdocument.h>
#include <bobuiextdocumentfragment.h>
#include <bobuiextlist.h>
#include <qabstracttextdocumentlayout.h>
#include <bobuiextcursor.h>
#include "../bobuiextdocument/common.h"

class tst_BOBUIextList : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void item();
    void autoNumbering();
    void autoNumberingRTL();
    void autoNumberingPrefixAndSuffix();
    void autoNumberingPrefixAndSuffixRTL();
    void autoNumberingPrefixAndSuffixHtmlExportImport();
    void romanNumbering();
    void romanNumberingLimit();
    void formatChange();
    void cursorNavigation();
    void partialRemoval();
    void formatReferenceChange();
    void ensureItemOrder();
    void add();
    void defaultIndent();
    void blockUpdate();
    void numbering_data();
    void numbering();
    void start_data();
    void start();

private:
    BOBUIextDocument *doc;
    BOBUIextCursor cursor;
    BOBUIestDocumentLayout *layout;
};

void tst_BOBUIextList::init()
{
    doc = new BOBUIextDocument();
    layout = new BOBUIestDocumentLayout(doc);
    doc->setDocumentLayout(layout);
    cursor = BOBUIextCursor(doc);
}

void tst_BOBUIextList::cleanup()
{
    cursor = BOBUIextCursor();
    delete doc;
    doc = 0;
}

void tst_BOBUIextList::item()
{
    // this is basically a test for the key() + 1 in BOBUIextList::item.
    BOBUIextList *list = cursor.createList(BOBUIextListFormat());
    QVERIFY(list->item(0).blockFormat().objectIndex() != -1);
}

void tst_BOBUIextList::autoNumbering()
{
    BOBUIextListFormat fmt;
    fmt.setStyle(BOBUIextListFormat::ListLowerAlpha);
    BOBUIextList *list = cursor.createList(fmt);
    QVERIFY(list);

    for (int i = 0; i < 27; ++i)
        cursor.insertBlock();

    QCOMPARE(list->count(), 28);

    QVERIFY(cursor.currentList());
    QCOMPARE(cursor.currentList()->itemNumber(cursor.block()), 27);
    QCOMPARE(cursor.currentList()->itemText(cursor.block()), QLatin1String("ab."));
}

void tst_BOBUIextList::autoNumberingPrefixAndSuffix()
{
    BOBUIextListFormat fmt;
    fmt.setStyle(BOBUIextListFormat::ListLowerAlpha);
    fmt.setNumberPrefix("-");
    fmt.setNumberSuffix(")");
    BOBUIextList *list = cursor.createList(fmt);
    QVERIFY(list);

    for (int i = 0; i < 27; ++i)
        cursor.insertBlock();

    QCOMPARE(list->count(), 28);

    QVERIFY(cursor.currentList());
    QCOMPARE(cursor.currentList()->itemNumber(cursor.block()), 27);
    QCOMPARE(cursor.currentList()->itemText(cursor.block()), QLatin1String("-ab)"));
}

void tst_BOBUIextList::autoNumberingPrefixAndSuffixRTL()
{
    BOBUIextBlockFormat bfmt;
    bfmt.setLayoutDirection(BobUI::RightToLeft);
    cursor.setBlockFormat(bfmt);

    BOBUIextListFormat fmt;
    fmt.setStyle(BOBUIextListFormat::ListUpperAlpha);
    fmt.setNumberPrefix("-");
    fmt.setNumberSuffix("*");
    BOBUIextList *list = cursor.createList(fmt);
    QVERIFY(list);

    cursor.insertBlock();

    QCOMPARE(list->count(), 2);

    QCOMPARE(cursor.currentList()->itemText(cursor.block()), QLatin1String("*B-"));
}

void tst_BOBUIextList::autoNumberingPrefixAndSuffixHtmlExportImport()
{
    BOBUIextListFormat fmt;
    fmt.setStyle(BOBUIextListFormat::ListLowerAlpha);
    fmt.setNumberPrefix("\"");
    fmt.setNumberSuffix("#");
    fmt.setIndent(10);
    // FIXME: Would like to test "'" but there's a problem in the css parser (Scanner::preprocess
    // is called before the values are being parsed), so the quoting does not work.
    BOBUIextList *list = cursor.createList(fmt);
    QVERIFY(list);

    for (int i = 0; i < 27; ++i)
        cursor.insertBlock();

    QCOMPARE(list->count(), 28);

#ifndef BOBUI_NO_TEXTHTMLPARSER
    QString htmlExport = doc->toHtml();
    BOBUIextDocument importDoc;
    importDoc.setHtml(htmlExport);

    BOBUIextCursor importCursor(&importDoc);
    for (int i = 0; i < 27; ++i)
        importCursor.movePosition(BOBUIextCursor::NextBlock);

    QVERIFY(importCursor.currentList());
    QCOMPARE(importCursor.currentList()->itemNumber(importCursor.block()), 27);
    QCOMPARE(importCursor.currentList()->itemText(importCursor.block()), QLatin1String("\"ab#"));
    QCOMPARE(importCursor.currentList()->format().indent(), 10);
#endif
}

void tst_BOBUIextList::autoNumberingRTL()
{
    BOBUIextBlockFormat bfmt;
    bfmt.setLayoutDirection(BobUI::RightToLeft);
    cursor.setBlockFormat(bfmt);

    BOBUIextListFormat fmt;
    fmt.setStyle(BOBUIextListFormat::ListUpperAlpha);
    BOBUIextList *list = cursor.createList(fmt);
    QVERIFY(list);

    cursor.insertBlock();

    QCOMPARE(list->count(), 2);

    QCOMPARE(cursor.currentList()->itemText(cursor.block()), QLatin1String(".B"));
}

void tst_BOBUIextList::romanNumbering()
{
    BOBUIextListFormat fmt;
    fmt.setStyle(BOBUIextListFormat::ListUpperRoman);
    BOBUIextList *list = cursor.createList(fmt);
    QVERIFY(list);

    for (int i = 0; i < 4998; ++i)
      cursor.insertBlock();

    QCOMPARE(list->count(), 4999);

    QVERIFY(cursor.currentList());
    QCOMPARE(cursor.currentList()->itemNumber(cursor.block()), 4998);
    QCOMPARE(cursor.currentList()->itemText(cursor.block()), QLatin1String("MMMMCMXCIX."));
}

void tst_BOBUIextList::romanNumberingLimit()
{
    BOBUIextListFormat fmt;
    fmt.setStyle(BOBUIextListFormat::ListLowerRoman);
    BOBUIextList *list = cursor.createList(fmt);
    QVERIFY(list);

    for (int i = 0; i < 4999; ++i)
      cursor.insertBlock();

    QCOMPARE(list->count(), 5000);

    QVERIFY(cursor.currentList());
    QCOMPARE(cursor.currentList()->itemNumber(cursor.block()), 4999);
    QCOMPARE(cursor.currentList()->itemText(cursor.block()), QLatin1String("?."));
}

void tst_BOBUIextList::formatChange()
{
    // testing the formatChanged slot in BOBUIextListManager

    /* <initial block>
     * 1.
     * 2.
     */
    BOBUIextList *list = cursor.insertList(BOBUIextListFormat::ListDecimal);
    BOBUIextList *firstList = list;
    cursor.insertBlock();

    QVERIFY(list && list->count() == 2);

    BOBUIextBlockFormat bfmt = cursor.blockFormat();
//     QCOMPARE(bfmt.object(), list);

    bfmt.setObjectIndex(-1);
    cursor.setBlockFormat(bfmt);

    QCOMPARE(firstList->count(), 1);
}

void tst_BOBUIextList::cursorNavigation()
{
    // testing some cursor list methods

    /* <initial block>
     * 1.
     * 2.
     */
    cursor.insertList(BOBUIextListFormat::ListDecimal);
    cursor.insertBlock();

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextBlock);
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QVERIFY(cursor.currentList());
    cursor.movePosition(BOBUIextCursor::PreviousBlock);
    QVERIFY(cursor.currentList());
    QCOMPARE(cursor.currentList()->itemNumber(cursor.block()), 0);
}

void tst_BOBUIextList::partialRemoval()
{
    /* this is essentially a test for PieceTable::removeBlock to not miss any
       blocks with the blockChanged signal emission that actually get removed.

       It creates two lists, like this:

       1. Hello World
       a. Foobar

       and then removes from within the 'Hello World' into the 'Foobar' .
       There used to be no emission for the removal of the second (a.) block,
       causing list inconsistencies.

       */

    BOBUIextList *firstList = cursor.insertList(BOBUIextListFormat::ListDecimal);

    BOBUIextCursor selStart = cursor;
    selStart.movePosition(BOBUIextCursor::PreviousCharacter);

    cursor.insertText("Hello World");

    // position it well into the 'hello world' text.
    selStart.movePosition(BOBUIextCursor::NextCharacter);
    selStart.movePosition(BOBUIextCursor::NextCharacter);
    selStart.clearSelection();

    QPointer<BOBUIextList> secondList = cursor.insertList(BOBUIextListFormat::ListCircle);
    cursor.insertText("Foobar");

    // position it into the 'foo bar' text.
    cursor.movePosition(BOBUIextCursor::PreviousCharacter);
    BOBUIextCursor selEnd = cursor;

    // this creates a selection that includes parts of both text-fragments and also the list item of the second list.
    BOBUIextCursor selection = selStart;
    selection.setPosition(selEnd.position(),  BOBUIextCursor::KeepAnchor);

    selection.deleteChar(); // deletes the second list

    QVERIFY(!secondList);
    QVERIFY(firstList->count() > 0);

    doc->undo();
}

void tst_BOBUIextList::formatReferenceChange()
{
    BOBUIextList *list = cursor.insertList(BOBUIextListFormat::ListDecimal);
    cursor.insertText("Some Content...");
    cursor.insertBlock(BOBUIextBlockFormat());

    cursor.setPosition(list->item(0).position());
    int listItemStartPos = cursor.position();
    cursor.movePosition(BOBUIextCursor::NextBlock);
    int listItemLen = cursor.position() - listItemStartPos;
    layout->expect(listItemStartPos, listItemLen, listItemLen);

    BOBUIextListFormat fmt = list->format();
    fmt.setStyle(BOBUIextListFormat::ListCircle);
    list->setFormat(fmt);

    QVERIFY(layout->called);
    QVERIFY(!layout->error);
}

void tst_BOBUIextList::ensureItemOrder()
{
    /*
     * Insert a new list item before the first one and verify the blocks
     * are sorted after that.
     */
    BOBUIextList *list = cursor.insertList(BOBUIextListFormat::ListDecimal);

    BOBUIextBlockFormat fmt = cursor.blockFormat();
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.insertBlock(fmt);

    QCOMPARE(list->item(0).position(), 1);
    QCOMPARE(list->item(1).position(), 2);
}

void tst_BOBUIextList::add()
{
    BOBUIextList *list = cursor.insertList(BOBUIextListFormat::ListDecimal);
    cursor.insertBlock(BOBUIextBlockFormat());
    QCOMPARE(list->count(), 1);
    cursor.insertBlock(BOBUIextBlockFormat());
    list->add(cursor.block());
    QCOMPARE(list->count(), 2);
}

// Task #72036
void tst_BOBUIextList::defaultIndent()
{
    BOBUIextListFormat fmt;
    QCOMPARE(fmt.indent(), 1);
}

void tst_BOBUIextList::blockUpdate()
{
    // three items
    BOBUIextList *list = cursor.insertList(BOBUIextListFormat::ListDecimal);
    cursor.insertBlock();
    cursor.insertBlock();

    // remove second, needs also update on the third
    // since the numbering might have changed
    const int len = cursor.position() + cursor.block().length() - 1;
    layout->expect(1, len, len);
    list->remove(list->item(1));
    QVERIFY(!layout->error);
}

void tst_BOBUIextList::numbering_data()
{
    BOBUIest::addColumn<int>("format");
    BOBUIest::addColumn<int>("number");
    BOBUIest::addColumn<QString>("result");

    BOBUIest::newRow("E.") << int(BOBUIextListFormat::ListUpperAlpha) << 5 << "E.";
    BOBUIest::newRow("abc.") << int(BOBUIextListFormat::ListLowerAlpha) << (26 + 2) * 26 + 3 << "abc.";
    BOBUIest::newRow("12.") << int(BOBUIextListFormat::ListDecimal) << 12 << "12.";
    BOBUIest::newRow("XXIV.") << int(BOBUIextListFormat::ListUpperRoman) << 24 << "XXIV.";
    BOBUIest::newRow("VIII.") << int(BOBUIextListFormat::ListUpperRoman) << 8 << "VIII.";
    BOBUIest::newRow("xxx.") << int(BOBUIextListFormat::ListLowerRoman) << 30 << "xxx.";
    BOBUIest::newRow("xxix.") << int(BOBUIextListFormat::ListLowerRoman) << 29 << "xxix.";
//    BOBUIest::newRow("xxx. alpha") << int(BOBUIextListFormat::ListLowerAlpha) << (24 * 26 + 24) * 26 + 24  << "xxx."; //Too slow
}

void tst_BOBUIextList::numbering()
{
    QFETCH(int, format);
    QFETCH(int, number);
    QFETCH(QString, result);


    BOBUIextListFormat fmt;
    fmt.setStyle(BOBUIextListFormat::Style(format));
    BOBUIextList *list = cursor.createList(fmt);
    QVERIFY(list);

    for (int i = 1; i < number; ++i)
        cursor.insertBlock();

    QCOMPARE(list->count(), number);

    QVERIFY(cursor.currentList());
    QCOMPARE(cursor.currentList()->itemNumber(cursor.block()), number - 1);
    QCOMPARE(cursor.currentList()->itemText(cursor.block()), result);
}

void tst_BOBUIextList::start_data()
{
    BOBUIest::addColumn<int>("format");
    BOBUIest::addColumn<int>("start");
    BOBUIest::addColumn<QStringList>("expectedItemTexts");

    BOBUIest::newRow("-1.") << int(BOBUIextListFormat::ListDecimal) << -1
                         << QStringList{ "-1.", "0.", "1." };
    BOBUIest::newRow("0.") << int(BOBUIextListFormat::ListDecimal) << 0
                        << QStringList{ "0.", "1.", "2." };
    BOBUIest::newRow("1.") << int(BOBUIextListFormat::ListDecimal) << 1
                        << QStringList{ "1.", "2.", "3." };

    BOBUIest::newRow("A. -1") << int(BOBUIextListFormat::ListUpperAlpha) << -1
                           << QStringList{ "-1.", "0.", "A." };
    BOBUIest::newRow("A. 0.") << int(BOBUIextListFormat::ListUpperAlpha) << 0
                           << QStringList{ "0.", "A.", "B." };
    BOBUIest::newRow("a. -1") << int(BOBUIextListFormat::ListLowerAlpha) << -1
                           << QStringList{ "-1.", "0.", "a." };
    BOBUIest::newRow("a. 0.") << int(BOBUIextListFormat::ListLowerAlpha) << 0
                           << QStringList{ "0.", "a.", "b." };
    BOBUIest::newRow("d. 4.") << int(BOBUIextListFormat::ListLowerAlpha) << 4
                           << QStringList{ "d.", "e.", "f." };

    BOBUIest::newRow("I. -1") << int(BOBUIextListFormat::ListUpperRoman) << -1
                           << QStringList{ "-1.", "0.", "I." };
    BOBUIest::newRow("I. 0.") << int(BOBUIextListFormat::ListUpperRoman) << 0
                           << QStringList{ "0.", "I.", "II." };
    BOBUIest::newRow("i. -1") << int(BOBUIextListFormat::ListLowerRoman) << -1
                           << QStringList{ "-1.", "0.", "i." };
    BOBUIest::newRow("i. 0.") << int(BOBUIextListFormat::ListLowerRoman) << 0
                           << QStringList{ "0.", "i.", "ii." };
}

void tst_BOBUIextList::start()
{
    QFETCH(int, format);
    QFETCH(int, start);
    QFETCH(QStringList, expectedItemTexts);

    BOBUIextListFormat fmt;
    fmt.setStyle(BOBUIextListFormat::Style(format));
    fmt.setStart(start);
    BOBUIextList *list = cursor.createList(fmt);
    QVERIFY(list);

    while (list->count() < int(expectedItemTexts.size()))
        cursor.insertBlock();

    QCOMPARE(list->count(), expectedItemTexts.size());

    for (int i = 0; i < list->count(); ++i)
        QCOMPARE(cursor.currentList()->itemText(cursor.currentList()->item(i)),
                 expectedItemTexts[i]);
}

BOBUIEST_MAIN(tst_BOBUIextList)
#include "tst_bobuiextlist.moc"
