// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QLoggingCategory>

#include <qfontinfo.h>
#include <bobuiextdocument.h>
#include <bobuiexttable.h>
#include <qvariant.h>
#include <bobuiextdocumentfragment.h>
#include <qabstracttextdocumentlayout.h>
#include <bobuiextlayout.h>
#include <bobuiextcursor.h>
#include <bobuiextobject.h>
#include <qdebug.h>

#include <private/bobuiextcursor_p.h>

Q_LOGGING_CATEGORY(lcTests, "bobui.gui.tests")

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextDocument)

class tst_BOBUIextCursor : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void navigation1();
    void navigation2_data();
    void navigation2();
    void navigation3();
    void navigation4();
    void navigation5();
    void navigation6();
    void navigation7();
    void navigation8();
    void navigation9();
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void navigation10();
#endif
    void movePositionEndOfLine();
    void insertBlock();
    void insertWithBlockSeparator1();
    void insertWithBlockSeparator2();
    void insertWithBlockSeparator3();
    void insertWithBlockSeparator4();
    void clearObjectType1();
    void clearObjectType2();
    void clearObjectType3();
    void comparisonOperators1();
    void comparisonOperators2();
    void selection1();
    void dontCopyTableAttributes();

    void checkFrame1();
    void checkFrame2();

    void tableMovement();
    void selectionsInTable();

    void insertBlockToUseCharFormat();

    void selectedText();

    void insertBlockShouldRemoveSelection();
    void insertBlockShouldRemoveSelection2();
    void mergeCellShouldUpdateSelection();

    void joinPreviousEditBlock();

    void setBlockFormatInTable();

    void blockCharFormat();
    void blockCharFormat2();
    void blockCharFormat3();
    void blockCharFormatOnSelection();

    void anchorInitialized1();
    void anchorInitialized2();
    void anchorInitialized3();

    void selectWord();
    void selectWordWithSeparators_data();
    void selectWordWithSeparators();
    void startOfWord();
    void selectBlock();
    void selectVisually();

    void insertText();
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void insertHtml_data();
    void insertHtml();
#endif
#if BOBUI_CONFIG(textmarkdownreader)
    void insertMarkdown_data();
    void insertMarkdown();
#endif

    void insertFragmentShouldUseCurrentCharFormat();

    void endOfLine();

    void editBlocksDuringRemove();
    void selectAllDuringRemove();

    void update_data();
    void update();

    void disallowSettingObjectIndicesOnCharFormats();

    void blockAndColumnNumber();

    void clearCells();

    void task244408_wordUnderCursor_data();
    void task244408_wordUnderCursor();

    void adjustCursorsOnInsert();

    void cursorPositionWithBlockUndoAndRedo();
    void cursorPositionWithBlockUndoAndRedo2();
    void cursorPositionWithBlockUndoAndRedo3();

    void joinNonEmptyRemovedBlockUserState();
    void crashOnDetachingDanglingCursor();

private:
    int blockCount();

    BOBUIextDocument *doc;
    BOBUIextCursor cursor;
};

void tst_BOBUIextCursor::init()
{
    doc = new BOBUIextDocument;
    cursor = BOBUIextCursor(doc);
}

void tst_BOBUIextCursor::cleanup()
{
    cursor = BOBUIextCursor();
    delete doc;
    doc = 0;
}

void tst_BOBUIextCursor::navigation1()
{

    cursor.insertText("Hello World");
    QCOMPARE(doc->toPlainText(), QLatin1String("Hello World"));

    cursor.movePosition(BOBUIextCursor::End);
    QCOMPARE(cursor.position(), 11);
    cursor.deletePreviousChar();
    QCOMPARE(cursor.position(), 10);
    cursor.deletePreviousChar();
    cursor.deletePreviousChar();
    cursor.deletePreviousChar();
    cursor.deletePreviousChar();
    cursor.deletePreviousChar();
    QCOMPARE(doc->toPlainText(), QLatin1String("Hello"));

    BOBUIextCursor otherCursor(doc);
    otherCursor.movePosition(BOBUIextCursor::Start);
    otherCursor.movePosition(BOBUIextCursor::Right);
    cursor = otherCursor;
    cursor.movePosition(BOBUIextCursor::Right);
    QVERIFY(cursor != otherCursor);
    otherCursor.insertText("Hey");
    QCOMPARE(cursor.position(), 5);

    doc->undo();
    QCOMPARE(cursor.position(), 2);
    doc->redo();
    QCOMPARE(cursor.position(), 5);

    doc->undo();

    doc->undo();
    QCOMPARE(doc->toPlainText(), QLatin1String("Hello World"));

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::Right, BOBUIextCursor::MoveAnchor, 6);
    QCOMPARE(cursor.position(), 6);
    otherCursor = cursor;
    otherCursor.movePosition(BOBUIextCursor::Right, BOBUIextCursor::MoveAnchor, 2);
    otherCursor.deletePreviousChar();
    otherCursor.deletePreviousChar();
    otherCursor.deletePreviousChar();
    QCOMPARE(cursor.position(), 5);

    cursor.movePosition(BOBUIextCursor::End);
    cursor.insertBlock();
    {
        int oldPos = cursor.position();
        cursor.movePosition(BOBUIextCursor::End);
        QCOMPARE(cursor.position(), oldPos);
    }
    QVERIFY(cursor.atBlockStart());
    QCOMPARE(cursor.position(), 9);

    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::blue);
    cursor.insertText("Test", fmt);
    QCOMPARE(fmt, cursor.charFormat());
    QCOMPARE(cursor.position(), 13);
}

void tst_BOBUIextCursor::navigation2_data()
{
    BOBUIest::addColumn<QStringList>("sl");
    BOBUIest::addColumn<QList<QVariant> >("movement");
    BOBUIest::addColumn<int>("finalPos");

    BOBUIest::newRow("startBlock1") << QStringList("Happy happy happy joy joy joy")
                              << (QList<QVariant>() << QVariant(BOBUIextCursor::StartOfBlock)) << 0;
    BOBUIest::newRow("endBlock1") << QStringList("Happy happy happy joy joy joy")
                            << (QList<QVariant>() << QVariant(BOBUIextCursor::StartOfBlock)
                                     << QVariant(BOBUIextCursor::EndOfBlock)) << 29;
    BOBUIest::newRow("startBlock2") << QStringList("Happy happy happy joy joy joy")
                              << (QList<QVariant>() << QVariant(BOBUIextCursor::StartOfBlock)
                                     << QVariant(BOBUIextCursor::EndOfBlock)
                                     << QVariant(BOBUIextCursor::StartOfBlock)) << 0;
    BOBUIest::newRow("endBlock2") << QStringList("Happy happy happy joy joy joy")
                            << (QList<QVariant>() << QVariant(BOBUIextCursor::StartOfBlock)
                                     << QVariant(BOBUIextCursor::EndOfBlock)
                                     << QVariant(BOBUIextCursor::StartOfBlock)
                                     << QVariant(BOBUIextCursor::EndOfBlock)
                                     ) << 29;
    BOBUIest::newRow("multiBlock1") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::StartOfBlock))
                             << 18;
    BOBUIest::newRow("multiBlock2") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::StartOfBlock)
                                                   << QVariant(BOBUIextCursor::EndOfBlock))
                             << 29;
    BOBUIest::newRow("multiBlock3") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::StartOfBlock)
                                                   << QVariant(BOBUIextCursor::StartOfBlock))
                             << 18;
    BOBUIest::newRow("multiBlock4") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::Start)
                                                   << QVariant(BOBUIextCursor::EndOfBlock))
                             << 17;
    BOBUIest::newRow("multiBlock5") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::Start)
                                                   << QVariant(BOBUIextCursor::EndOfBlock)
                                                   << QVariant(BOBUIextCursor::EndOfBlock))
                             << 17;
    BOBUIest::newRow("multiBlock6") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::End)
                                                   << QVariant(BOBUIextCursor::StartOfBlock))
                             << 18;
    BOBUIest::newRow("multiBlock7") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousBlock))
                             << 0;
    BOBUIest::newRow("multiBlock8") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousBlock)
                                                   << QVariant(BOBUIextCursor::EndOfBlock))
                             << 17;
    BOBUIest::newRow("multiBlock9") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousBlock)
                                                   << QVariant(BOBUIextCursor::NextBlock))
                             << 18;
    BOBUIest::newRow("multiBlock10") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                               << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousBlock)
                                                     << QVariant(BOBUIextCursor::NextBlock)
                                                     << QVariant(BOBUIextCursor::NextBlock))
                               << 18;
    BOBUIest::newRow("multiBlock11") << (QStringList() << QString("Happy happy happy")
                                                << QString("Joy Joy Joy"))
                               << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousBlock)
                                                     << QVariant(BOBUIextCursor::NextBlock)
                                                     << QVariant(BOBUIextCursor::EndOfBlock))
                               << 29;
    BOBUIest::newRow("PreviousWord1") << (QStringList() << QString("Happy happy happy Joy Joy Joy"))
                                << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousWord))
                                << 26;
    BOBUIest::newRow("PreviousWord2") << (QStringList() << QString("Happy happy happy Joy Joy Joy"))
                                << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousWord)
                                                      << QVariant(BOBUIextCursor::PreviousWord))
                                << 22;
    BOBUIest::newRow("EndWord1") << (QStringList() << QString("Happy happy happy Joy Joy Joy"))
                                << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousWord)
                                                      << QVariant(BOBUIextCursor::PreviousWord)
                                                      << QVariant(BOBUIextCursor::EndOfWord))
                                << 25;
    BOBUIest::newRow("NextWord1") << (QStringList() << QString("Happy happy happy Joy Joy Joy"))
                                << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousWord)
                                                      << QVariant(BOBUIextCursor::PreviousWord)
                                                      << QVariant(BOBUIextCursor::NextWord))
                                << 26;
    BOBUIest::newRow("NextWord2") << (QStringList() << QString("Happy happy happy Joy Joy Joy"))
                                << (QList<QVariant>() << QVariant(BOBUIextCursor::Start)
                                                      << QVariant(BOBUIextCursor::NextWord)
                                                      << QVariant(BOBUIextCursor::EndOfWord))
                                << 11;
    BOBUIest::newRow("StartWord1") << (QStringList() << QString("Happy happy happy Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousWord)
                                                   << QVariant(BOBUIextCursor::PreviousWord)
                                                   << QVariant(BOBUIextCursor::StartOfWord))
                             << 22;
    BOBUIest::newRow("StartWord3") << (QStringList() << QString("Happy happy happy Joy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::Start)
                                                   << QVariant(BOBUIextCursor::NextWord)
                                                   << QVariant(BOBUIextCursor::EndOfWord)
                                                   << QVariant(BOBUIextCursor::StartOfWord))
                             << 6;

    BOBUIest::newRow("PreviousCharacter") << (QStringList() << QString("Happy happy Joy Joy"))
                             << (QList<QVariant>() << QVariant(BOBUIextCursor::PreviousCharacter)
                                                   << QVariant(BOBUIextCursor::PreviousCharacter))
                             << 17;
}

void tst_BOBUIextCursor::navigation2()
{
    QFETCH(QStringList, sl);
    QFETCH(QList<QVariant>, movement);
    int i;
    for (i = 0; i < sl.size(); ++i) {
        cursor.insertText(sl.at(i));
        if (i < sl.size() - 1)
            cursor.insertBlock();
    }

    for (i = 0; i < movement.size(); ++i)
        cursor.movePosition(BOBUIextCursor::MoveOperation(movement.at(i).toInt()));
    BOBUIEST(cursor.position(), "finalPos");
}

void tst_BOBUIextCursor::navigation3()
{
    cursor.insertText("a");
    cursor.deletePreviousChar();
    QCOMPARE(cursor.position(), 0);
    QVERIFY(doc->toPlainText().isEmpty());
}

void tst_BOBUIextCursor::navigation4()
{
    cursor.insertText("  Test  ");

    cursor.setPosition(4);
    cursor.movePosition(BOBUIextCursor::EndOfWord);
    QCOMPARE(cursor.position(), 6);
}

void tst_BOBUIextCursor::navigation5()
{
    cursor.insertText("Test");
    cursor.insertBlock();
    cursor.insertText("Test");

    cursor.setPosition(0);
    cursor.movePosition(BOBUIextCursor::EndOfBlock);
    QCOMPARE(cursor.position(), 4);
}

void tst_BOBUIextCursor::navigation6()
{
    // triger creation of document layout, so that BOBUIextLines are there
    doc->documentLayout();
    doc->setTextWidth(1000);

    cursor.insertText("Test    ");

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::EndOfLine);
    QCOMPARE(cursor.position(), 8);
}

void tst_BOBUIextCursor::navigation7()
{
    QVERIFY(doc->isEmpty());
    for (int i = BOBUIextCursor::Start; i <= BOBUIextCursor::WordRight; ++i)
        QVERIFY(!cursor.movePosition(BOBUIextCursor::MoveOperation(i)));

    doc->setPlainText("Hello World");
    cursor.movePosition(BOBUIextCursor::Start);
    do {
    } while (cursor.movePosition(BOBUIextCursor::NextCharacter));
    QVERIFY(true /*reached*/);
}

void tst_BOBUIextCursor::navigation8()
{
    cursor.insertList(BOBUIextListFormat::ListDecimal);
    QCOMPARE(cursor.position(), 1);
    cursor.insertText("foo");
    QCOMPARE(cursor.position(), 4);

    cursor.insertList(BOBUIextListFormat::ListCircle);
    QCOMPARE(cursor.position(), 5);
    cursor.insertText("something");
    QCOMPARE(cursor.position(), 14);

    cursor.movePosition(BOBUIextCursor::PreviousCharacter);
    QCOMPARE(cursor.position(), 13);

    cursor.setPosition(2);
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.position(), 3);
}

void tst_BOBUIextCursor::navigation9()
{
    cursor.insertText("Hello  &-=+\t   World");
    cursor.movePosition(BOBUIextCursor::PreviousWord);
    QCOMPARE(cursor.position(), 15);
    cursor.movePosition(BOBUIextCursor::PreviousWord);
    QCOMPARE(cursor.position(), 7);
    cursor.movePosition(BOBUIextCursor::PreviousWord);
    QCOMPARE(cursor.position(), 0);
    cursor.movePosition(BOBUIextCursor::NextWord);
    QCOMPARE(cursor.position(), 7);
    cursor.movePosition(BOBUIextCursor::NextWord);
    QCOMPARE(cursor.position(), 15);
}

#ifndef BOBUI_NO_TEXTHTMLPARSER
void tst_BOBUIextCursor::navigation10()
{
    doc->setHtml("<html><p>just a simple paragraph.</p>"
        "<table>"
          "<tr><td>Cell number 1</td><td>another cell</td><td></td><td>previous</br>is</br>empty</td></tr>"
          "<tr><td>row 2</td><td colspan=\"2\">foo bar</td><td>last cell</td></tr>"
          "<tr><td colspan=\"3\">row 3</td><td>a</td></tr>"
        "</table></html");
    QCOMPARE(cursor.position(), 101); // end of document
    cursor.setPosition(0);
    QCOMPARE(cursor.position(), 0);
    bool ok = cursor.movePosition(BOBUIextCursor::EndOfLine);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 24);
    ok = cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.position(), 25); // cell 1
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 39); // another..
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 52); // empty
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 53); // last on row 1
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 69); // row 2
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 75);
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 83);
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 93); // row 3
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 99);
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(!ok);
    QCOMPARE(cursor.position(), 99); // didn't move.
    QVERIFY(cursor.currentTable());

    // same thing in reverse...
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 93);
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 83);
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 75);
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 69);
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 53);
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 52);
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 39);
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 25);
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(!ok);
    QCOMPARE(cursor.position(), 25); // can't leave the table

    ok = cursor.movePosition(BOBUIextCursor::NextRow);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 69);
    ok = cursor.movePosition(BOBUIextCursor::NextRow);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 93);
    ok = cursor.movePosition(BOBUIextCursor::NextRow);
    QVERIFY(!ok);
    QCOMPARE(cursor.position(), 93); // didn't move

    ok = cursor.movePosition(BOBUIextCursor::PreviousRow);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 83); // last col in row 2
    ok = cursor.movePosition(BOBUIextCursor::PreviousRow);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 53); // last col in row 1
    ok = cursor.movePosition(BOBUIextCursor::PreviousRow);
    QVERIFY(!ok);
    QCOMPARE(cursor.position(), 53);

    // test usecase of jumping over a cell
    doc->clear();
    doc->setHtml("<html><table>tr><td rowspan=\"2\">a</td><td>b</td></tr><tr><td>c</td></tr></table></html>");
    cursor.setPosition(1); // a
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 3); // b
    ok = cursor.movePosition(BOBUIextCursor::NextCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 5); // c
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 3); // b
    ok = cursor.movePosition(BOBUIextCursor::PreviousCell);
    QVERIFY(ok);
    QCOMPARE(cursor.position(), 1); // a
}
#endif

void tst_BOBUIextCursor::insertBlock()
{
    BOBUIextBlockFormat fmt;
    fmt.setTopMargin(100);
    cursor.insertBlock(fmt);
    QCOMPARE(cursor.position(), 1);
    QCOMPARE(cursor.blockFormat(), fmt);
}

void tst_BOBUIextCursor::insertWithBlockSeparator1()
{
    QString text = "Hello" + QString(QChar::ParagraphSeparator) + "World";

    cursor.insertText(text);

    cursor.movePosition(BOBUIextCursor::PreviousBlock);
    QCOMPARE(cursor.position(), 0);

    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.position(), 6);
}

void tst_BOBUIextCursor::insertWithBlockSeparator2()
{
    cursor.insertText(QString(QChar::ParagraphSeparator));
    QCOMPARE(cursor.position(), 1);
}

void tst_BOBUIextCursor::insertWithBlockSeparator3()
{
    cursor.insertText(QString(QChar::ParagraphSeparator) + "Hi" + QString(QChar::ParagraphSeparator));
    QCOMPARE(cursor.position(), 4);
}

void tst_BOBUIextCursor::insertWithBlockSeparator4()
{
    cursor.insertText(QString(QChar::ParagraphSeparator) + QString(QChar::ParagraphSeparator));
    QCOMPARE(cursor.position(), 2);
}

void tst_BOBUIextCursor::clearObjectType1()
{
    cursor.insertImage("test.png");
    QVERIFY(cursor.charFormat().isValid());
    QVERIFY(cursor.charFormat().isImageFormat());
    cursor.insertText("Hey");
    QVERIFY(cursor.charFormat().isValid());
    QVERIFY(!cursor.charFormat().isImageFormat());
}

void tst_BOBUIextCursor::clearObjectType2()
{
    cursor.insertImage("test.png");
    QVERIFY(cursor.charFormat().isValid());
    QVERIFY(cursor.charFormat().isImageFormat());
    cursor.insertBlock();
    QVERIFY(cursor.charFormat().isValid());
    QVERIFY(!cursor.charFormat().isImageFormat());
}

void tst_BOBUIextCursor::clearObjectType3()
{
    // like clearObjectType2 but tests different insertBlock overload
    cursor.insertImage("test.png");
    QVERIFY(cursor.charFormat().isValid());
    QVERIFY(cursor.charFormat().isImageFormat());
    BOBUIextBlockFormat bfmt;
    bfmt.setAlignment(BobUI::AlignRight);
    cursor.insertBlock(bfmt);
    QVERIFY(cursor.charFormat().isValid());
    QVERIFY(!cursor.charFormat().isImageFormat());
}

void tst_BOBUIextCursor::comparisonOperators1()
{
    cursor.insertText("Hello World");

    cursor.movePosition(BOBUIextCursor::PreviousWord);

    BOBUIextCursor startCursor = cursor;
    startCursor.movePosition(BOBUIextCursor::Start);

    QVERIFY(startCursor < cursor);

    BOBUIextCursor midCursor = startCursor;
    midCursor.movePosition(BOBUIextCursor::NextWord);

    QVERIFY(midCursor <= cursor);
    QCOMPARE(midCursor, cursor);
    QVERIFY(midCursor >= cursor);

    QVERIFY(midCursor > startCursor);

    QVERIFY(midCursor != startCursor);
    QVERIFY(!(midCursor == startCursor));

    BOBUIextCursor nullCursor;

    QVERIFY(!(startCursor < nullCursor));
    QVERIFY(!(nullCursor < nullCursor));
    QVERIFY(nullCursor < startCursor);

    QVERIFY(nullCursor <= startCursor);
    QVERIFY(!(startCursor <= nullCursor));

    QVERIFY(!(nullCursor >= startCursor));
    QVERIFY(startCursor >= nullCursor);

    QVERIFY(!(nullCursor > startCursor));
    QVERIFY(!(nullCursor > nullCursor));
    QVERIFY(startCursor > nullCursor);
}

void tst_BOBUIextCursor::comparisonOperators2()
{
    BOBUIextDocument doc1;
    BOBUIextDocument doc2;

    BOBUIextCursor cursor1(&doc1);
    BOBUIextCursor cursor2(&doc2);

    QVERIFY(cursor1 != cursor2);
    QCOMPARE(cursor1, BOBUIextCursor(&doc1));
}

void tst_BOBUIextCursor::selection1()
{
    cursor.insertText("Hello World");

    cursor.setPosition(0);
    cursor.clearSelection();
    cursor.setPosition(4, BOBUIextCursor::KeepAnchor);

    QCOMPARE(cursor.selectionStart(), 0);
    QCOMPARE(cursor.selectionEnd(), 4);
}

void tst_BOBUIextCursor::dontCopyTableAttributes()
{
    /* when pressing 'enter' inside a cell it shouldn't
     * enlarge the table by adding another cell but just
     * extend the cell */
    BOBUIextTable *table = cursor.insertTable(2, 2);
    QVERIFY(cursor == table->cellAt(0, 0).firstCursorPosition());
    cursor.insertBlock();
    QCOMPARE(table->columns(), 2);
}

void tst_BOBUIextCursor::checkFrame1()
{
    QCOMPARE(cursor.position(), 0);
    QPointer<BOBUIextFrame> frame = cursor.insertFrame(BOBUIextFrameFormat());
    QVERIFY(frame != nullptr);

    BOBUIextFrame *root = frame->parentFrame();
    QVERIFY(root != nullptr);

    QCOMPARE(frame->firstPosition(), 1);
    QCOMPARE(frame->lastPosition(), 1);
    QVERIFY(frame->parentFrame() != nullptr);
    QCOMPARE(root->childFrames().size(), 1);

    QCOMPARE(cursor.position(), 1);
    QCOMPARE(cursor.selectionStart(), 1);
    QCOMPARE(cursor.selectionEnd(), 1);

    doc->undo();

    QVERIFY(!frame);
    QCOMPARE(root->childFrames().size(), 0);

    QCOMPARE(cursor.position(), 0);
    QCOMPARE(cursor.selectionStart(), 0);
    QCOMPARE(cursor.selectionEnd(), 0);

    doc->redo();

    frame = doc->frameAt(1);

    QVERIFY(frame);
    QCOMPARE(frame->firstPosition(), 1);
    QCOMPARE(frame->lastPosition(), 1);
    QVERIFY(frame->parentFrame() != 0);
    QCOMPARE(root->childFrames().size(), 1);

    QCOMPARE(cursor.position(), 1);
    QCOMPARE(cursor.selectionStart(), 1);
    QCOMPARE(cursor.selectionEnd(), 1);

//     cursor.movePosition(BOBUIextCursor::Right, BOBUIextCursor::KeepAnchor);
//     QCOMPARE(cursor.position(), 2);
//     QCOMPARE(cursor.selectionStart(), 0);
//     QCOMPARE(cursor.selectionEnd(), 2);
}

void tst_BOBUIextCursor::checkFrame2()
{
    QCOMPARE(cursor.position(), 0);
    cursor.insertText("A");
    QCOMPARE(cursor.position(), 1);
    cursor.movePosition(BOBUIextCursor::Start, BOBUIextCursor::KeepAnchor);

    QPointer<BOBUIextFrame> frame = cursor.insertFrame(BOBUIextFrameFormat());
    BOBUIextFrame *root = frame->parentFrame();

    QCOMPARE(frame->firstPosition(), 1);
    QCOMPARE(frame->lastPosition(), 2);
    QVERIFY(frame->parentFrame() != 0);
    QCOMPARE(root->childFrames().size(), 1);

    QCOMPARE(cursor.position(), 1);
    QCOMPARE(cursor.selectionStart(), 1);
    QCOMPARE(cursor.selectionEnd(), 2);

    doc->undo();

    QVERIFY(!frame);
    QCOMPARE(root->childFrames().size(), 0);

    QCOMPARE(cursor.position(), 0);
    QCOMPARE(cursor.selectionStart(), 0);
    QCOMPARE(cursor.selectionEnd(), 1);

    doc->redo();

    frame = doc->frameAt(1);

    QVERIFY(frame);
    QCOMPARE(frame->firstPosition(), 1);
    QCOMPARE(frame->lastPosition(), 2);
    QVERIFY(frame->parentFrame() != 0);
    QCOMPARE(root->childFrames().size(), 1);

    QCOMPARE(cursor.position(), 1);
    QCOMPARE(cursor.selectionStart(), 1);
    QCOMPARE(cursor.selectionEnd(), 2);

    cursor.movePosition(BOBUIextCursor::Left, BOBUIextCursor::KeepAnchor);
    QCOMPARE(cursor.position(), 0);
    QCOMPARE(cursor.selectionStart(), 0);
    QCOMPARE(cursor.selectionEnd(), 3);
}

void tst_BOBUIextCursor::insertBlockToUseCharFormat()
{
    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::blue);
    cursor.insertText("Hello", fmt);
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::blue));

    cursor.insertBlock();
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::blue));

    fmt.setForeground(BobUI::red);
    cursor.insertText("Hello\nWorld", fmt);
    cursor.insertText("Blah");
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::red));

    // ### we might want a testcase for createTable, too, as it calls insertBlock, too,
    // and we might want to have the char format copied (the one that gets inserted
    // as table separators, that are undeletable)
}

void tst_BOBUIextCursor::tableMovement()
{
    QCOMPARE(cursor.position(), 0);
    cursor.insertText("AA");
    QCOMPARE(cursor.position(), 2);
    cursor.movePosition(BOBUIextCursor::Left);

    cursor.insertTable(3, 3);
    QCOMPARE(cursor.position(), 2);

    cursor.movePosition(BOBUIextCursor::Down);
    QCOMPARE(cursor.position(), 5);

    cursor.movePosition(BOBUIextCursor::Right);
    QCOMPARE(cursor.position(), 6);

    cursor.movePosition(BOBUIextCursor::Up);
    QCOMPARE(cursor.position(), 3);

    cursor.movePosition(BOBUIextCursor::Right);
    QCOMPARE(cursor.position(), 4);

    cursor.movePosition(BOBUIextCursor::Right);
    QCOMPARE(cursor.position(), 5);

    cursor.movePosition(BOBUIextCursor::Up);
    QCOMPARE(cursor.position(), 2);

    cursor.movePosition(BOBUIextCursor::Up);
    QCOMPARE(cursor.position(), 0);

}

void tst_BOBUIextCursor::selectionsInTable()
{
    BOBUIextTable *table = cursor.insertTable(3, 3);
    table->cellAt(0, 0).firstCursorPosition().insertText("A a"); // first = 1
    table->cellAt(0, 1).firstCursorPosition().insertText("B b"); // first = 5
    table->cellAt(0, 2).firstCursorPosition().insertText("C c"); // first = 9
    table->cellAt(1, 0).firstCursorPosition().insertText("D d"); // first = 13
    table->cellAt(1, 1).firstCursorPosition().insertText("E e"); // first = 17
    table->cellAt(1, 2).firstCursorPosition().insertText("F f"); // first = 21
    table->cellAt(2, 0).firstCursorPosition().insertText("G g"); // first = 25
    table->cellAt(2, 1).firstCursorPosition().insertText("H h"); // first = 29
    table->cellAt(2, 2).firstCursorPosition().insertText("I i"); // first = 33

    cursor = table->cellAt(0, 0).lastCursorPosition();
    QCOMPARE(cursor.position(), 4);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousWord, BOBUIextCursor::KeepAnchor));
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousWord, BOBUIextCursor::KeepAnchor));
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousWord, BOBUIextCursor::KeepAnchor) == false);
    QCOMPARE(cursor.position(), 1);

    cursor = table->cellAt(1, 0).lastCursorPosition();
    QCOMPARE(cursor.position(), 16);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousWord, BOBUIextCursor::KeepAnchor));
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousWord, BOBUIextCursor::KeepAnchor));
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousWord, BOBUIextCursor::KeepAnchor) == false);
    QCOMPARE(cursor.position(), 13);

    cursor = table->cellAt(0, 2).firstCursorPosition();
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextWord, BOBUIextCursor::KeepAnchor));
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextWord, BOBUIextCursor::KeepAnchor));
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextWord, BOBUIextCursor::KeepAnchor) == false);

    cursor = table->cellAt(1, 2).firstCursorPosition();
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextWord, BOBUIextCursor::KeepAnchor));
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextWord, BOBUIextCursor::KeepAnchor));
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextWord, BOBUIextCursor::KeepAnchor) == false);

    // Next let's test selecting entire cells one at a time
    cursor = table->cellAt(0, 0).firstCursorPosition();
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 5);
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 9);
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 13);
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 17);
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 21);
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 25);
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 29);
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 33);
    QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor) == false);

    // And now lets walk all the way back
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 29);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 25);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 21);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 17);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 13);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 9);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 5);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousCell, BOBUIextCursor::KeepAnchor));
    QCOMPARE(cursor.position(), 1);
    QVERIFY(cursor.movePosition(BOBUIextCursor::PreviousCell, BOBUIextCursor::KeepAnchor) == false);

    BOBUIextCursor::MoveOperation leftMovements[5] = {
          BOBUIextCursor::PreviousBlock
        , BOBUIextCursor::PreviousCharacter
        , BOBUIextCursor::PreviousWord
        , BOBUIextCursor::Left
        , BOBUIextCursor::WordLeft
    };

    BOBUIextCursor::MoveOperation rightMovements[5] = {
        BOBUIextCursor::NextBlock
        , BOBUIextCursor::NextCharacter
        , BOBUIextCursor::NextWord
        , BOBUIextCursor::Right
        , BOBUIextCursor::WordRight
    };

    for (int i = 0; i < 5; ++i) {
        BOBUIextCursor::MoveOperation left = leftMovements[i];
        BOBUIextCursor::MoveOperation right = rightMovements[i];

        // Lets walk circle around anchor placed at 1,1 using up, down, left and right
        cursor = table->cellAt(1, 1).firstCursorPosition();
        QCOMPARE(cursor.position(), 17);
        QVERIFY(cursor.movePosition(BOBUIextCursor::Right, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 18); // First right should not jump more than one char
        QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 21); // Lets jump to the next cell
        QVERIFY(cursor.movePosition(BOBUIextCursor::Down, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 33);
        QVERIFY(cursor.movePosition(left, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 29);
        QVERIFY(cursor.movePosition(left, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 25);
        QVERIFY(cursor.movePosition(BOBUIextCursor::Up, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 13);
        QVERIFY(cursor.movePosition(BOBUIextCursor::Up, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 1);
        QVERIFY(cursor.movePosition(right, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 5);
        QVERIFY(cursor.movePosition(right, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 9);
        QVERIFY(cursor.movePosition(BOBUIextCursor::Down, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 21);

        // Lets walk to the side 2 cells and back, first right
        cursor = table->cellAt(0, 0).firstCursorPosition();
        QVERIFY(cursor.movePosition(BOBUIextCursor::NextCell, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 5); // Lets jump to the next cell
        QVERIFY(cursor.movePosition(right, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 9);
        QVERIFY(cursor.movePosition(left, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 5);
        QVERIFY(cursor.movePosition(left, BOBUIextCursor::KeepAnchor));
        QVERIFY(cursor.position() < 5);

        // Then left
        cursor = table->cellAt(0, 2).firstCursorPosition();
        QCOMPARE(cursor.position(), 9);
        QVERIFY(cursor.movePosition(left, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 5); // A single left should do
        QVERIFY(cursor.movePosition(left, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 1);
        QVERIFY(cursor.movePosition(right, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 5);
        QVERIFY(cursor.movePosition(right, BOBUIextCursor::KeepAnchor));
        QCOMPARE(cursor.position(), 9);
    }
}

void tst_BOBUIextCursor::selectedText()
{
    cursor.insertText("Hello World");
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::End, BOBUIextCursor::KeepAnchor);

    QCOMPARE(cursor.selectedText(), QString("Hello World"));
}

void tst_BOBUIextCursor::insertBlockShouldRemoveSelection()
{
    cursor.insertText("Hello World");
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::EndOfWord, BOBUIextCursor::KeepAnchor);

    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selectedText(), QString("Hello"));

    cursor.insertBlock();

    QVERIFY(!cursor.hasSelection());
    QCOMPARE(doc->toPlainText().indexOf("Hello"), -1);
}

void tst_BOBUIextCursor::insertBlockShouldRemoveSelection2()
{
    cursor.insertText("Hello World");
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::EndOfWord, BOBUIextCursor::KeepAnchor);

    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selectedText(), QString("Hello"));

    BOBUIextBlockFormat fmt = cursor.blockFormat();
    cursor.insertBlock(fmt);

    QVERIFY(!cursor.hasSelection());
    QCOMPARE(doc->toPlainText().indexOf("Hello"), -1);
}

void tst_BOBUIextCursor::mergeCellShouldUpdateSelection()
{
    BOBUIextTable *table = cursor.insertTable(4, 4);
    cursor.setPosition(table->cellAt(0, 0).firstPosition());
    cursor.setPosition(table->cellAt(3, 0).firstPosition(), BOBUIextCursor::KeepAnchor); // aka bottom left
    int firstRow, numRows, firstColumn, numColumns;
    cursor.selectedTableCells(&firstRow, &numRows, &firstColumn, &numColumns);
    QCOMPARE(firstRow, 0);
    QCOMPARE(numRows, 4);
    QCOMPARE(firstColumn, 0);
    QCOMPARE(numColumns, 1);

    table->removeColumns(firstColumn, numColumns);

    QCOMPARE(cursor.anchor(), table->cellAt(0, 0).firstPosition());
    QCOMPARE(cursor.position(), table->cellAt(0, 0).firstPosition());
    QCOMPARE(cursor.position(), cursor.anchor()); // empty. I don't really care where it ends up.

    // prepare for another test with multiple cursors.
    // note we have a 4 rows, 3 cols table now.
    cursor.setPosition(table->cellAt(0, 0).firstPosition());
    cursor.setPosition(table->cellAt(0, 2).firstPosition(), BOBUIextCursor::KeepAnchor);

    // now create a selection of a whole row.
    BOBUIextCursor c2 = table->cellAt(2, 0).firstCursorPosition();
    c2.setPosition(table->cellAt(2, 2).firstPosition(), BOBUIextCursor::KeepAnchor);

    // just for good measure, another one for a block of cells.
    BOBUIextCursor c3 = table->cellAt(2, 1).firstCursorPosition();
    c3.setPosition(table->cellAt(3, 2).firstPosition(), BOBUIextCursor::KeepAnchor);

    table->removeRows(2, 1);

    QCOMPARE(cursor.anchor(), table->cellAt(0, 0).firstPosition());
    QCOMPARE(cursor.position(), table->cellAt(0, 2).firstPosition());

    QCOMPARE(c2.position(), c2.anchor()); // empty. I don't really care where it ends up.

    QCOMPARE(c3.anchor(), table->cellAt(2, 1).firstPosition());
    QCOMPARE(c3.position(), table->cellAt(2, 2).firstPosition());


    // prepare for another test where we remove a column
    // note we have a 3 rows, 3 cols table now.
    cursor.setPosition(table->cellAt(0, 0).firstPosition());
    cursor.setPosition(table->cellAt(2, 1).firstPosition(), BOBUIextCursor::KeepAnchor);

    c2.setPosition(table->cellAt(0, 1).firstPosition());
    c2.setPosition(table->cellAt(2, 2).firstPosition(), BOBUIextCursor::KeepAnchor);

    table->removeColumns(1, 1);

    QCOMPARE(cursor.anchor(), table->cellAt(0, 0).firstPosition());
    QCOMPARE(cursor.position(), table->cellAt(2, 0).firstPosition());

    QCOMPARE(c2.anchor(), table->cellAt(0, 1).firstPosition());
    QCOMPARE(c2.position(), table->cellAt(2, 1).firstPosition());

    // test for illegal cursor positions.
    // note we have a 3 rows, 2 cols table now.
    cursor.setPosition(table->cellAt(2, 0).firstPosition());
    cursor.setPosition(table->cellAt(2, 1).firstPosition(), BOBUIextCursor::KeepAnchor);

    c2.setPosition(table->cellAt(0, 0).firstPosition());
    c2.setPosition(table->cellAt(2, 1).firstPosition(), BOBUIextCursor::KeepAnchor);

    c3.setPosition(table->cellAt(2, 1).firstPosition());

    table->removeRows(2, 1);

    QCOMPARE(cursor.anchor(), table->cellAt(1, 1).lastPosition()+1);
    QCOMPARE(cursor.position(), cursor.anchor());

    QCOMPARE(c2.anchor(), table->cellAt(0, 0).firstPosition());
    QCOMPARE(c2.position(), table->cellAt(1, 1).firstPosition());

    QCOMPARE(c3.anchor(), table->cellAt(1, 1).firstPosition());
    QCOMPARE(c3.position(), table->cellAt(1, 1).firstPosition());
}

void tst_BOBUIextCursor::joinPreviousEditBlock()
{
    cursor.beginEditBlock();
    cursor.insertText("Hello");
    cursor.insertText("World");
    cursor.endEditBlock();
    QVERIFY(doc->toPlainText().startsWith("HelloWorld"));

    cursor.joinPreviousEditBlock();
    cursor.insertText("Hey");
    cursor.endEditBlock();
    QVERIFY(doc->toPlainText().startsWith("HelloWorldHey"));

    doc->undo();
    QVERIFY(!doc->toPlainText().contains("HelloWorldHey"));
}

void tst_BOBUIextCursor::setBlockFormatInTable()
{
    // someone reported this on bobui4-preview-feedback
    BOBUIextBlockFormat fmt;
    fmt.setBackground(BobUI::blue);
    cursor.setBlockFormat(fmt);

    BOBUIextTable *table = cursor.insertTable(2, 2);
    cursor = table->cellAt(0, 0).firstCursorPosition();
    fmt.setBackground(BobUI::red);
    cursor.setBlockFormat(fmt);

    cursor.movePosition(BOBUIextCursor::Start);
    QCOMPARE(cursor.blockFormat().background().color(), QColor(BobUI::blue));
}

void tst_BOBUIextCursor::blockCharFormat2()
{
    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::green);
    cursor.mergeBlockCharFormat(fmt);

    fmt.setForeground(BobUI::red);

    cursor.insertText("Test", fmt);
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.insertText("Red");
    cursor.movePosition(BOBUIextCursor::PreviousCharacter);
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::red));
}

void tst_BOBUIextCursor::blockCharFormat3()
{
    QVERIFY(cursor.atBlockStart());
    QVERIFY(cursor.atBlockEnd());
    QVERIFY(cursor.atStart());

    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::green);
    cursor.setBlockCharFormat(fmt);
    cursor.insertText("Test");
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    const QColor red(BobUI::red);
    const QColor green(BobUI::green);
    QCOMPARE(cursor.charFormat().foreground().color(), green);

    cursor.movePosition(BOBUIextCursor::Start);
    QCOMPARE(cursor.charFormat().foreground().color(), green);

    fmt.setForeground(BobUI::red);
    cursor.setBlockCharFormat(fmt);
    QCOMPARE(cursor.blockCharFormat().foreground().color(), red);

    cursor.movePosition(BOBUIextCursor::End);
    cursor.movePosition(BOBUIextCursor::Start);
    QCOMPARE(cursor.charFormat().foreground().color(), green);

    cursor.insertText("Test");
    QCOMPARE(cursor.charFormat().foreground().color(), green);

    cursor.select(BOBUIextCursor::Document);
    cursor.removeSelectedText();
    QVERIFY(cursor.atBlockStart());
    QVERIFY(cursor.atBlockEnd());
    QVERIFY(cursor.atStart());

    cursor.insertText("Test");
    QCOMPARE(cursor.charFormat().foreground().color(), red);
}

void tst_BOBUIextCursor::blockCharFormat()
{
    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::blue);
    cursor.insertBlock(BOBUIextBlockFormat(), fmt);
    cursor.insertText("Hm");

    QCOMPARE(cursor.blockCharFormat().foreground().color(), QColor(BobUI::blue));

    fmt.setForeground(BobUI::red);

    cursor.setBlockCharFormat(fmt);
    QCOMPARE(cursor.blockCharFormat().foreground().color(), QColor(BobUI::red));
}

void tst_BOBUIextCursor::blockCharFormatOnSelection()
{
    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::blue);
    cursor.insertBlock(BOBUIextBlockFormat(), fmt);

    fmt.setForeground(BobUI::green);
    cursor.insertText("Hm", fmt);

    fmt.setForeground(BobUI::red);
    cursor.insertBlock(BOBUIextBlockFormat(), fmt);
    cursor.insertText("Ah");

    fmt.setForeground(BobUI::white);
    cursor.insertBlock(BOBUIextBlockFormat(), fmt);
    cursor.insertText("bleh");

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.blockCharFormat().foreground().color(), QColor(BobUI::blue));
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.blockCharFormat().foreground().color(), QColor(BobUI::red));
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.blockCharFormat().foreground().color(), QColor(BobUI::white));

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextBlock);
    cursor.movePosition(BOBUIextCursor::NextBlock, BOBUIextCursor::KeepAnchor);

    fmt.setForeground(BobUI::cyan);
    cursor.setBlockCharFormat(fmt);

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.blockCharFormat().foreground().color(), QColor(BobUI::cyan));

    cursor.movePosition(BOBUIextCursor::Right);
    cursor.movePosition(BOBUIextCursor::Right);
    QCOMPARE(cursor.charFormat().foreground().color(), QColor(BobUI::green));

    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.blockCharFormat().foreground().color(), QColor(BobUI::cyan));

    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.blockCharFormat().foreground().color(), QColor(BobUI::white));
}

void tst_BOBUIextCursor::anchorInitialized1()
{
    cursor.insertBlock();
    cursor = BOBUIextCursor(cursor.block());
    QCOMPARE(cursor.position(), 1);
    QCOMPARE(cursor.anchor(), 1);
    QCOMPARE(cursor.selectionStart(), 1);
    QCOMPARE(cursor.selectionEnd(), 1);
}

void tst_BOBUIextCursor::anchorInitialized2()
{
    cursor.insertBlock();
    cursor = BOBUIextCursorPrivate::fromPosition(const_cast<BOBUIextDocumentPrivate *>(BOBUIextDocumentPrivate::get(cursor.block())), 1);
    QCOMPARE(cursor.position(), 1);
    QCOMPARE(cursor.anchor(), 1);
    QCOMPARE(cursor.selectionStart(), 1);
    QCOMPARE(cursor.selectionEnd(), 1);
}

void tst_BOBUIextCursor::anchorInitialized3()
{
    BOBUIextFrame *frame = cursor.insertFrame(BOBUIextFrameFormat());
    cursor = BOBUIextCursor(frame);
    QCOMPARE(cursor.position(), 1);
    QCOMPARE(cursor.anchor(), 1);
    QCOMPARE(cursor.selectionStart(), 1);
    QCOMPARE(cursor.selectionEnd(), 1);
}

void tst_BOBUIextCursor::selectWord()
{
    cursor.insertText("first second     third");
    cursor.insertBlock();
    cursor.insertText("words in second paragraph");

    cursor.setPosition(9);
    cursor.select(BOBUIextCursor::WordUnderCursor);
    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selectionStart(), 6);
    QCOMPARE(cursor.selectionEnd(), 12);

    cursor.setPosition(5);
    cursor.select(BOBUIextCursor::WordUnderCursor);
    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selectionStart(), 0);
    QCOMPARE(cursor.selectionEnd(), 5);

    cursor.setPosition(6);
    cursor.select(BOBUIextCursor::WordUnderCursor);
    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selectionStart(), 6);
    QCOMPARE(cursor.selectionEnd(), 12);

    cursor.setPosition(14);
    cursor.select(BOBUIextCursor::WordUnderCursor);
    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selectionStart(), 6);
    QCOMPARE(cursor.selectionEnd(), 12);

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.select(BOBUIextCursor::WordUnderCursor);
    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selectionStart(), 0);
    QCOMPARE(cursor.selectionEnd(), 5);

    cursor.movePosition(BOBUIextCursor::EndOfBlock);
    cursor.select(BOBUIextCursor::WordUnderCursor);
    QVERIFY(cursor.hasSelection());
    QCOMPARE(cursor.selectionStart(), 17);
    QCOMPARE(cursor.selectionEnd(), 22);
}

void tst_BOBUIextCursor::selectWordWithSeparators_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<int>("initialPosition");
    BOBUIest::addColumn<QString>("expectedSelectedText");

    BOBUIest::newRow("dereference") << QString::fromLatin1("foo->bar()") << 1 << QString::fromLatin1("foo");
    BOBUIest::newRow("funcsignature") << QString::fromLatin1("bar(int x);") << 1 << QString::fromLatin1("bar");
    BOBUIest::newRow("def") << QString::fromLatin1("foo *f;") << 1 << QString::fromLatin1("foo");
}

void tst_BOBUIextCursor::selectWordWithSeparators()
{
    QFETCH(QString, text);
    QFETCH(int, initialPosition);
    QFETCH(QString, expectedSelectedText);

    cursor.insertText(text);
    cursor.setPosition(initialPosition);
    cursor.select(BOBUIextCursor::WordUnderCursor);

    QCOMPARE(cursor.selectedText(), expectedSelectedText);
}

void tst_BOBUIextCursor::startOfWord()
{
    cursor.insertText("first     second");
    cursor.setPosition(7);
    cursor.movePosition(BOBUIextCursor::StartOfWord);
    QCOMPARE(cursor.position(), 0);
}

void tst_BOBUIextCursor::selectBlock()
{
    cursor.insertText("foobar");
    BOBUIextBlockFormat blockFmt;
    blockFmt.setAlignment(BobUI::AlignHCenter);
    cursor.insertBlock(blockFmt);
    cursor.insertText("blah");
    cursor.insertBlock(BOBUIextBlockFormat());

    cursor.movePosition(BOBUIextCursor::PreviousBlock);
    QCOMPARE(cursor.block().text(), QString("blah"));

    cursor.select(BOBUIextCursor::BlockUnderCursor);
    QVERIFY(cursor.hasSelection());

    BOBUIextDocumentFragment fragment(cursor);
    doc->clear();
    cursor.insertFragment(fragment);
    QCOMPARE(blockCount(), 2);

    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.blockFormat().alignment(), BobUI::AlignHCenter);
    QCOMPARE(cursor.block().text(), QString("blah"));
}

void tst_BOBUIextCursor::selectVisually()
{
    cursor.insertText("Foo\nlong line which is probably going to be cut in two when shown in a widget\nparagraph 3\n");

    cursor.setPosition(6); // somewhere in the long paragraph.
    cursor.select(BOBUIextCursor::LineUnderCursor);
    // since we are not yet laid-out, we expect the whole paragraph to be selected.
    QCOMPARE(cursor.position(), 77);
    QCOMPARE(cursor.anchor(), 4);
}

void tst_BOBUIextCursor::insertText()
{
    QString txt = "Foo\nBar\r\nMeep";
    txt += QChar::LineSeparator;
    txt += "Baz";
    txt += QChar::ParagraphSeparator;
    txt += "yoyodyne";
    cursor.insertText(txt);
    QCOMPARE(blockCount(), 4);
    cursor.movePosition(BOBUIextCursor::Start);
    QCOMPARE(cursor.block().text(), QString("Foo"));
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.block().text(), QString("Bar"));
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.block().text(), QString(QString("Meep") + QChar(QChar::LineSeparator) + QString("Baz")));
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.block().text(), QString("yoyodyne"));
}


#ifndef BOBUI_NO_TEXTHTMLPARSER

void tst_BOBUIextCursor::insertHtml_data()
{
    BOBUIest::addColumn<QString>("initialText");
    BOBUIest::addColumn<int>("expectedInitialBlockCount");
    BOBUIest::addColumn<bool>("insertBlock");
    BOBUIest::addColumn<bool>("insertAsPlainText");
    BOBUIest::addColumn<int>("insertPosition");
    BOBUIest::addColumn<QString>("insertText");
    BOBUIest::addColumn<QString>("expectedSelText");
    BOBUIest::addColumn<QString>("expectedText");
    BOBUIest::addColumn<QString>("expectedMarkdown");

    const QString htmlHeadingString("<h1>Hello World</h1>");

    BOBUIest::newRow("insert as html at end of heading")
            << htmlHeadingString << 1
            << false << false << 11 << QString("Other\ntext")
            << QString("Hello WorldOther text")
            << QString("Hello WorldOther text")
            << QString("# Hello WorldOther text\n\n");

    BOBUIest::newRow("insert as html in new block at end of heading")
            << htmlHeadingString << 1
            << false << true << 11 << QString("Other\ntext")
            << QString("Hello WorldOther\u2029text")
            << QString("Hello WorldOther\ntext")
            << QString("# Hello WorldOther\n\n# text\n\n");

    BOBUIest::newRow("insert as html in middle of heading")
            << htmlHeadingString << 1
            << false << false << 6 << QString("\n\nOther\ntext\n\n")
            << QString("Hello Other text World")
            << QString("Hello Other text World")
            << QString("# Hello Other text World\n\n");

    BOBUIest::newRow("insert as text at end of heading")
            << htmlHeadingString << 1
            << true << false << 11 << QString("\n\nOther\ntext")
            << QString("Hello World\u2029Other text")
            << QString("Hello World\nOther text")
            << QString("# Hello World\n\nOther text\n\n");

    BOBUIest::newRow("insert as text in new block at end of heading")
            << htmlHeadingString << 1
            << true << true << 11 << QString("\n\nOther\ntext")
            << QString("Hello World\u2029\u2029\u2029Other\u2029text")
            << QString("Hello World\n\n\nOther\ntext")
            << QString("# Hello World\n\n**Other**\n\n**text**\n\n");

    BOBUIest::newRow("insert as text in middle of heading")
            << htmlHeadingString << 1
            << true << false << 6 << QString("Other\ntext")
            << QString("Hello \u2029Other textWorld")
            << QString("Hello \nOther textWorld")
            << QString("# Hello \n\nOther text**World**\n\n");
}

void tst_BOBUIextCursor::insertHtml()
{
    QFETCH(QString, initialText);
    QFETCH(int, expectedInitialBlockCount);
    QFETCH(bool, insertBlock);
    QFETCH(bool, insertAsPlainText);
    QFETCH(int, insertPosition);
    QFETCH(QString, insertText);
    QFETCH(QString, expectedSelText);
    QFETCH(QString, expectedText);
    QFETCH(QString, expectedMarkdown);

    cursor.insertHtml(initialText);
    QCOMPARE(blockCount(), expectedInitialBlockCount);
    cursor.setPosition(insertPosition);
    if (insertBlock)
        cursor.insertBlock(BOBUIextBlockFormat());
    qCDebug(lcTests) << "pos" << cursor.position() << "block" << cursor.blockNumber()
                     << "heading" << cursor.blockFormat().headingLevel();
    if (insertAsPlainText)
        cursor.insertText(insertText);
    else
        cursor.insertHtml(insertText);
    cursor.select(BOBUIextCursor::Document);
    qCDebug(lcTests) << "sel text after insertion" << cursor.selectedText();
    qCDebug(lcTests) << "text after insertion" << cursor.document()->toPlainText();
    qCDebug(lcTests) << "html after insertion" << cursor.document()->toHtml();
#if BOBUI_CONFIG(textmarkdownwriter)
    qCDebug(lcTests) << "markdown after insertion" << cursor.document()->toMarkdown();
#endif
    QCOMPARE(cursor.selectedText(), expectedSelText);
    QCOMPARE(cursor.document()->toPlainText(), expectedText);
    if (auto defaultFont = QFontDatabase::systemFont(QFontDatabase::GeneralFont); QFontInfo(defaultFont).fixedPitch()) {
        qWarning() << defaultFont << "is QFontDatabase::GeneralFont, and is fixedPitch";
        QSKIP("cannot reliably distinguish normal and monospace markdown spans on this system (BOBUIBUG-103484)");
    }
#if BOBUI_CONFIG(textmarkdownwriter)
    QCOMPARE(cursor.document()->toMarkdown(), expectedMarkdown);
#endif
}

#endif // BOBUI_NO_TEXTHTMLPARSER

#if BOBUI_CONFIG(textmarkdownreader)

void tst_BOBUIextCursor::insertMarkdown_data()
{
    BOBUIest::addColumn<QString>("initialText");
    BOBUIest::addColumn<int>("expectedInitialBlockCount");
    BOBUIest::addColumn<int>("insertPosition");
    BOBUIest::addColumn<QString>("insertText");
    BOBUIest::addColumn<QString>("expectedSelText");
    BOBUIest::addColumn<QString>("expectedText");
    BOBUIest::addColumn<QString>("expectedMarkdown");

    BOBUIest::newRow("bold fragment in italic span")
            << "someone said *hello world*" << 1
            << 19 << QString(" **crazy** ")
            << QString("someone said hello crazyworld")
            << QString("someone said hello crazyworld")
            << QString("someone said *hello ***crazy***world*\n\n"); // explicit B+I: not necessary but OK

    BOBUIest::newRow("list in a paragraph")
            << "hello list with 3 items" << 1
            << 10 << QString("1. one\n2. two\n")
            << QString("hello list\u2029one\u2029two\u2029 with 3 items")
            << QString("hello list\none\ntwo\n with 3 items")
            << QString("hello list\n\n1.  one\n2.  two\n3.   with 3 items\n");

    BOBUIest::newRow("list in a list")
            << "1) bread\n2) milk\n" << 2
            << 6 << QString("0) eggs\n1) maple syrup\n")
            << QString("bread\u2029eggs\u2029maple syrup\u2029milk")
            << QString("bread\neggs\nmaple syrup\nmilk")
            << QString("1)  bread\n2)  eggs\n0)  maple syrup\n1)  milk\n");
    // Renumbering would happen if we re-read the whole document.
    // Currently insertion only uses the new list format after a paragraph separator.
    // For that reason "bread" and "eggs" use the original list format, while "maple syrup" and
    // "milk" use the format from the just inserted list.

    BOBUIest::newRow("list after a list")
            << "1) bread\n2) milk\n\n" << 2
            << 13 << QString("\n0) eggs\n1) maple syrup\n")
            << QString("bread\u2029milk\u2029eggs\u2029maple syrup")
            << QString("bread\nmilk\neggs\nmaple syrup")
            << QString("1)  bread\n2)  milk\n3)  eggs\n0)  maple syrup\n");
    // Same behavior as above. "eggs" uses the original list format, but "maple syrup" uses the
    // format of the inserted list, which means "maple syrup" now has a start of 0.

    const QString markdownHeadingString("# Hello\nWorld\n");

    BOBUIest::newRow("markdown heading at end of markdown heading")
            << markdownHeadingString << 2
            << 11 << QString("\n\n## Other text")
            << QString("Hello\u2029World\u2029Other text")
            << QString("Hello\nWorld\nOther text")
            << QString("# Hello\n\nWorld\n\n## Other text\n\n");

    BOBUIest::newRow("markdown heading into middle of markdown heading")
            << markdownHeadingString << 2
            << 6 << QString("## Other\ntext\n\n")
            << QString("Hello\u2029Other\u2029text\u2029World")
            << QString("Hello\nOther\ntext\nWorld")
            << QString("# Hello\n\n**Other**\n\ntext\n\nWorld\n\n");

    BOBUIest::newRow("markdown heading without trailing newline into middle of markdown heading")
            << markdownHeadingString << 2
            << 6 << QString("## Other\ntext")
            << QString("Hello\u2029Other\u2029textWorld")
            << QString("Hello\nOther\ntextWorld")
            << QString("# Hello\n\n**Other**\n\ntextWorld\n\n");

    BOBUIest::newRow("text into middle of markdown heading after newline")
            << markdownHeadingString << 2
            << 6 << QString("Other ")
            << QString("Hello\u2029OtherWorld")
            << QString("Hello\nOtherWorld")
            << QString("# Hello\n\nOtherWorld\n\n");

    BOBUIest::newRow("text into middle of markdown heading before newline")
            << markdownHeadingString << 2
            << 5 << QString(" Other ")
            << QString("HelloOther\u2029World")
            << QString("HelloOther\nWorld")
            << QString("# HelloOther\n\nWorld\n\n");
}

void tst_BOBUIextCursor::insertMarkdown()
{
    QFETCH(QString, initialText);
    QFETCH(int, expectedInitialBlockCount);
    QFETCH(int, insertPosition);
    QFETCH(QString, insertText);
    QFETCH(QString, expectedSelText);
    QFETCH(QString, expectedText);
    QFETCH(QString, expectedMarkdown);

    cursor.insertMarkdown(initialText);
    QCOMPARE(blockCount(), expectedInitialBlockCount);
    cursor.setPosition(insertPosition);
    qCDebug(lcTests) << "pos" << cursor.position() << "block" << cursor.blockNumber()
                     << "heading" << cursor.blockFormat().headingLevel();
    cursor.insertMarkdown(insertText);
    cursor.select(BOBUIextCursor::Document);
    qCDebug(lcTests) << "sel text after insertion" << cursor.selectedText();
    qCDebug(lcTests) << "text after insertion" << cursor.document()->toPlainText();
    qCDebug(lcTests) << "html after insertion" << cursor.document()->toHtml();
    qCDebug(lcTests) << "markdown after insertion" << cursor.document()->toMarkdown();
    QCOMPARE(cursor.selectedText(), expectedSelText);
    QCOMPARE(cursor.document()->toPlainText(), expectedText);
    if (auto defaultFont = QFontDatabase::systemFont(QFontDatabase::GeneralFont); QFontInfo(defaultFont).fixedPitch()) {
        qWarning() << defaultFont << "is QFontDatabase::GeneralFont, and is fixedPitch";
        QSKIP("cannot reliably distinguish normal and monospace markdown spans on this system (BOBUIBUG-103484)");
    }
    QCOMPARE(cursor.document()->toMarkdown(), expectedMarkdown);
}

#endif // textmarkdownreader

void tst_BOBUIextCursor::insertFragmentShouldUseCurrentCharFormat()
{
    BOBUIextDocumentFragment fragment = BOBUIextDocumentFragment::fromPlainText("Hello World");
    BOBUIextCharFormat fmt;
    fmt.setFontUnderline(true);

    cursor.clearSelection();
    cursor.setCharFormat(fmt);
    cursor.insertFragment(fragment);
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.charFormat(), fmt);
}

int tst_BOBUIextCursor::blockCount()
{
    int cnt = 0;
    for (BOBUIextBlock blk = doc->begin(); blk.isValid(); blk = blk.next())
        ++cnt;
    return cnt;
}

void tst_BOBUIextCursor::endOfLine()
{
    doc->setPageSize(QSizeF(100000, INT_MAX));

    QString text("First Line    \nSecond Line  ");
    text.replace(QLatin1Char('\n'), QChar(QChar::LineSeparator));
    cursor.insertText(text);

    // ensure layouted
    doc->documentLayout()->documentSize();

    cursor.movePosition(BOBUIextCursor::Start);

    QCOMPARE(cursor.block().layout()->lineCount(), 2);

    cursor.movePosition(BOBUIextCursor::EndOfLine);
    QCOMPARE(cursor.position(), 14);
    cursor.movePosition(BOBUIextCursor::NextCharacter);
    QCOMPARE(cursor.position(), 15);
    cursor.movePosition(BOBUIextCursor::EndOfLine);
    QCOMPARE(cursor.position(), 28);
}

class CursorListener : public QObject
{
    Q_OBJECT
public:
    CursorListener(BOBUIextCursor *_cursor) : lastRecordedPosition(-1), lastRecordedAnchor(-1), recordingCount(0), cursor(_cursor) {}

    int lastRecordedPosition;
    int lastRecordedAnchor;
    int recordingCount;

public slots:
    void recordCursorPosition()
    {
        lastRecordedPosition = cursor->position();
        lastRecordedAnchor = cursor->anchor();
        ++recordingCount;
    }

    void selectAllContents()
    {
        // Only test the first time
        if (!recordingCount) {
            recordingCount++;
            cursor->select(BOBUIextCursor::Document);
            lastRecordedPosition = cursor->position();
            lastRecordedAnchor = cursor->anchor();
        }
    }

private:
    BOBUIextCursor *cursor;
};

void tst_BOBUIextCursor::editBlocksDuringRemove()
{
    CursorListener listener(&cursor);

    cursor.insertText("Hello World");
    cursor.movePosition(BOBUIextCursor::Start, BOBUIextCursor::KeepAnchor);
    QCOMPARE(cursor.selectedText(), QString("Hello World"));

    connect(doc, SIGNAL(contentsChanged()), &listener, SLOT(recordCursorPosition()));
    listener.recordingCount = 0;
    cursor.deleteChar();

    QCOMPARE(listener.recordingCount, 1);
    QCOMPARE(listener.lastRecordedPosition, 0);
    QCOMPARE(listener.lastRecordedAnchor, 0);

    QVERIFY(doc->toPlainText().isEmpty());
}

void tst_BOBUIextCursor::selectAllDuringRemove()
{
    CursorListener listener(&cursor);

    cursor.insertText("Hello World");
    cursor.movePosition(BOBUIextCursor::End);

    connect(doc, SIGNAL(contentsChanged()), &listener, SLOT(selectAllContents()));
    listener.recordingCount = 0;
    BOBUIextCursor localCursor = cursor;
    localCursor.deletePreviousChar();

    QCOMPARE(listener.lastRecordedPosition, 10);
    QCOMPARE(listener.lastRecordedAnchor, 0);
}

void tst_BOBUIextCursor::update_data()
{
    BOBUIest::addColumn<QString>("text");
    BOBUIest::addColumn<int>("position");
    BOBUIest::addColumn<int>("anchor");
    BOBUIest::addColumn<int>("modifyPosition");
    BOBUIest::addColumn<int>("modifyAnchor");
    BOBUIest::addColumn<QString>("insertText");
    BOBUIest::addColumn<int>("expectedPosition");
    BOBUIest::addColumn<int>("expectedAnchor");

    QString text("Hello big world");
    int charsToDelete = 3;
    BOBUIest::newRow("removeInsideSelection")
        << text
        << /*position*/ 0
        << /*anchor*/ int(text.size())
        // delete 'big'
        << 6
        << 6 + charsToDelete
        << QString() // don't insert anything, just remove
        << /*expectedPosition*/ 0
        << /*expectedAnchor*/ int(text.size() - charsToDelete)
        ;

    text = "Hello big world";
    charsToDelete = 3;
    BOBUIest::newRow("removeInsideSelectionWithSwappedAnchorAndPosition")
        << text
        << /*position*/ int(text.size())
        << /*anchor*/ 0
        // delete 'big'
        << 6
        << 6 + charsToDelete
        << QString() // don't insert anything, just remove
        << /*expectedPosition*/ int(text.size() - charsToDelete)
        << /*expectedAnchor*/ 0
        ;


    text = "Hello big world";
    charsToDelete = 3;
    QString textToInsert("small");
    BOBUIest::newRow("replaceInsideSelection")
        << text
        << /*position*/ 0
        << /*anchor*/ int(text.size())
        // delete 'big' ...
        << 6
        << 6 + charsToDelete
        << textToInsert // ... and replace 'big' with 'small'
        << /*expectedPosition*/ 0
        << /*expectedAnchor*/ int(text.size() - charsToDelete + textToInsert.size())
        ;

    text = "Hello big world";
    charsToDelete = 3;
    textToInsert = "small";
    BOBUIest::newRow("replaceInsideSelectionWithSwappedAnchorAndPosition")
        << text
        << /*position*/ int(text.size())
        << /*anchor*/ 0
        // delete 'big' ...
        << 6
        << 6 + charsToDelete
        << textToInsert // ... and replace 'big' with 'small'
        << /*expectedPosition*/ int(text.size() - charsToDelete + textToInsert.size())
        << /*expectedAnchor*/ 0
        ;


    text = "Hello big world";
    charsToDelete = 3;
    BOBUIest::newRow("removeBeforeSelection")
        << text
        << /*position*/ int(text.size() - 5)
        << /*anchor*/ int(text.size())
        // delete 'big'
        << 6
        << 6 + charsToDelete
        << QString() // don't insert anything, just remove
        << /*expectedPosition*/ int(text.size() - 5 - charsToDelete)
        << /*expectedAnchor*/ int(text.size() - charsToDelete)
        ;

    text = "Hello big world";
    charsToDelete = 3;
    BOBUIest::newRow("removeAfterSelection")
        << text
        << /*position*/ 0
        << /*anchor*/ 4
        // delete 'big'
        << 6
        << 6 + charsToDelete
        << QString() // don't insert anything, just remove
        << /*expectedPosition*/ 0
        << /*expectedAnchor*/ 4
        ;

}

void tst_BOBUIextCursor::update()
{
    QFETCH(QString, text);

    doc->setPlainText(text);

    QFETCH(int, position);
    QFETCH(int, anchor);

    cursor.setPosition(anchor);
    cursor.setPosition(position, BOBUIextCursor::KeepAnchor);

    QCOMPARE(cursor.position(), position);
    QCOMPARE(cursor.anchor(), anchor);

    QFETCH(int, modifyPosition);
    QFETCH(int, modifyAnchor);

    BOBUIextCursor modifyCursor = cursor;
    modifyCursor.setPosition(modifyAnchor);
    modifyCursor.setPosition(modifyPosition, BOBUIextCursor::KeepAnchor);

    QCOMPARE(modifyCursor.position(), modifyPosition);
    QCOMPARE(modifyCursor.anchor(), modifyAnchor);

    QFETCH(QString, insertText);
    modifyCursor.insertText(insertText);

    QFETCH(int, expectedPosition);
    QFETCH(int, expectedAnchor);

    QCOMPARE(cursor.position(), expectedPosition);
    QCOMPARE(cursor.anchor(), expectedAnchor);
}

void tst_BOBUIextCursor::disallowSettingObjectIndicesOnCharFormats()
{
    BOBUIextCharFormat fmt;
    fmt.setObjectIndex(42);
    cursor.insertText("Hey", fmt);
    QCOMPARE(cursor.charFormat().objectIndex(), -1);

    cursor.select(BOBUIextCursor::Document);
    cursor.mergeCharFormat(fmt);
    QCOMPARE(doc->begin().begin().fragment().charFormat().objectIndex(), -1);

    cursor.select(BOBUIextCursor::Document);
    cursor.setCharFormat(fmt);
    QCOMPARE(doc->begin().begin().fragment().charFormat().objectIndex(), -1);

    cursor.setBlockCharFormat(fmt);
    QCOMPARE(cursor.blockCharFormat().objectIndex(), -1);

    cursor.movePosition(BOBUIextCursor::End);
    cursor.insertBlock(BOBUIextBlockFormat(), fmt);
    QCOMPARE(cursor.blockCharFormat().objectIndex(), -1);

    doc->clear();

    BOBUIextTable *table = cursor.insertTable(1, 1);
    cursor.select(BOBUIextCursor::Document);
    cursor.setCharFormat(fmt);

    cursor = table->cellAt(0, 0).firstCursorPosition();
    QVERIFY(!cursor.isNull());
    QCOMPARE(cursor.blockCharFormat().objectIndex(), table->objectIndex());
}

void tst_BOBUIextCursor::blockAndColumnNumber()
{
    QCOMPARE(BOBUIextCursor().columnNumber(), 0);
    QCOMPARE(BOBUIextCursor().blockNumber(), 0);

    QCOMPARE(cursor.columnNumber(), 0);
    QCOMPARE(cursor.blockNumber(), 0);
    cursor.insertText("Hello");
    QCOMPARE(cursor.columnNumber(), 5);
    QCOMPARE(cursor.blockNumber(), 0);

    cursor.insertBlock();
    QCOMPARE(cursor.columnNumber(), 0);
    QCOMPARE(cursor.blockNumber(), 1);
    cursor.insertText("Blah");
    QCOMPARE(cursor.blockNumber(), 1);

    // trigger a layout
    doc->documentLayout();

    cursor.insertBlock();
    QCOMPARE(cursor.columnNumber(), 0);
    QCOMPARE(cursor.blockNumber(), 2);
    cursor.insertText("Test");
    QCOMPARE(cursor.columnNumber(), 4);
    QCOMPARE(cursor.blockNumber(), 2);
    cursor.insertText(QString(QChar(QChar::LineSeparator)));
    QCOMPARE(cursor.columnNumber(), 0);
    QCOMPARE(cursor.blockNumber(), 2);
    cursor.insertText("A");
    QCOMPARE(cursor.columnNumber(), 1);
    QCOMPARE(cursor.blockNumber(), 2);
}

void tst_BOBUIextCursor::movePositionEndOfLine()
{
    cursor.insertText("blah\nblah\n");
    // Select part of the second line ("la")
    cursor.setPosition(6);
    cursor.movePosition(BOBUIextCursor::NextCharacter, BOBUIextCursor::KeepAnchor, 2);
    QCOMPARE(cursor.selectedText(), QLatin1String("la"));

    // trigger a layout
    doc->documentLayout();

    // Remove "la" and append "something" to the end in one undo operation
    cursor.beginEditBlock();
    cursor.removeSelectedText();
    BOBUIextCursor c2(doc);
    c2.setPosition(7);
    c2.insertText("foo"); // append to doc without touching the cursor.

    QCOMPARE(cursor.position(), 6);
    cursor.movePosition(BOBUIextCursor::EndOfLine); // in an edit block visual movement is moved to the end of the paragraph
    QCOMPARE(cursor.position(), 10);
    cursor.endEditBlock();
}

void tst_BOBUIextCursor::clearCells()
{
    BOBUIextTable *table = cursor.insertTable(3, 5);
    cursor.setPosition(table->cellAt(0,0).firstPosition()); // select cell 1 and cell 2
    cursor.setPosition(table->cellAt(0,1).firstPosition(), BOBUIextCursor::KeepAnchor);
    cursor.deleteChar(); // should clear the cells, and not crash ;)
}

void tst_BOBUIextCursor::task244408_wordUnderCursor_data()
{
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<QString>("expected");
    BOBUIest::newRow("trailingSpace") << QString::fromLatin1("foo ") << QString::fromLatin1("");
    BOBUIest::newRow("noTrailingSpace") << QString::fromLatin1("foo") << QString::fromLatin1("foo");
}

void tst_BOBUIextCursor::task244408_wordUnderCursor()
{
    QFETCH(QString, input);
    QFETCH(QString, expected);
    cursor.insertText(input);
    cursor.movePosition(BOBUIextCursor::End);
    cursor.select(BOBUIextCursor::WordUnderCursor);
    QCOMPARE(cursor.selectedText(), expected);
}

void tst_BOBUIextCursor::adjustCursorsOnInsert()
{
    cursor.insertText("Some text before ");
    int posBefore = cursor.position();
    cursor.insertText("selected text");
    int posAfter = cursor.position();
    cursor.insertText(" some text afterwards");

    BOBUIextCursor selection = cursor;
    selection.setPosition(posBefore);
    selection.setPosition(posAfter, BOBUIextCursor::KeepAnchor);

    cursor.setPosition(posBefore-1);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.anchor(), posBefore+1);
    QCOMPARE(selection.position(), posAfter+1);
    doc->undo();

    cursor.setPosition(posBefore);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.anchor(), posBefore+1);
    QCOMPARE(selection.position(), posAfter+1);
    doc->undo();

    cursor.setPosition(posBefore+1);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.anchor(), posBefore);
    QCOMPARE(selection.position(), posAfter+1);
    doc->undo();

    cursor.setPosition(posAfter-1);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.anchor(), posBefore);
    QCOMPARE(selection.position(), posAfter+1);
    doc->undo();

    selection.setKeepPositionOnInsert(true);
    cursor.setPosition(posAfter);
    cursor.insertText(QLatin1String("x"));
    selection.setKeepPositionOnInsert(false);
    QCOMPARE(selection.anchor(), posBefore);
    QCOMPARE(selection.position(), posAfter);
    doc->undo();

    cursor.setPosition(posAfter+1);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.anchor(), posBefore);
    QCOMPARE(selection.position(), posAfter);
    doc->undo();

    selection.setPosition(posAfter);
    selection.setPosition(posBefore, BOBUIextCursor::KeepAnchor);

    cursor.setPosition(posBefore-1);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.position(), posBefore+1);
    QCOMPARE(selection.anchor(), posAfter+1);
    doc->undo();

    cursor.setPosition(posBefore);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.position(), posBefore+1);
    QCOMPARE(selection.anchor(), posAfter+1);
    doc->undo();

    cursor.setPosition(posBefore+1);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.position(), posBefore);
    QCOMPARE(selection.anchor(), posAfter+1);
    doc->undo();

    cursor.setPosition(posAfter-1);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.position(), posBefore);
    QCOMPARE(selection.anchor(), posAfter+1);
    doc->undo();

    cursor.setPosition(posAfter);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.position(), posBefore);
    QCOMPARE(selection.anchor(), posAfter+1);
    doc->undo();

    cursor.setPosition(posAfter+1);
    cursor.insertText(QLatin1String("x"));
    QCOMPARE(selection.position(), posBefore);
    QCOMPARE(selection.anchor(), posAfter);
    doc->undo();

}
void tst_BOBUIextCursor::cursorPositionWithBlockUndoAndRedo()
{
    cursor.insertText("AAAABBBBCCCCDDDD");
    cursor.setPosition(12);
    int cursorPositionBefore = cursor.position();
    cursor.beginEditBlock();
    cursor.insertText("*");
    cursor.setPosition(8);
    cursor.insertText("*");
    cursor.setPosition(4);
    cursor.insertText("*");
    cursor.setPosition(0);
    cursor.insertText("*");
    int cursorPositionAfter = cursor.position();
    cursor.endEditBlock();

    QCOMPARE(doc->toPlainText(), QLatin1String("*AAAA*BBBB*CCCC*DDDD"));
    QCOMPARE(12, cursorPositionBefore);
    QCOMPARE(1, cursorPositionAfter);

    doc->undo(&cursor);
    QCOMPARE(doc->toPlainText(), QLatin1String("AAAABBBBCCCCDDDD"));
    QCOMPARE(cursor.position(), cursorPositionBefore);
    doc->redo(&cursor);
    QCOMPARE(doc->toPlainText(), QLatin1String("*AAAA*BBBB*CCCC*DDDD"));
    QCOMPARE(cursor.position(), cursorPositionAfter);
}

void tst_BOBUIextCursor::cursorPositionWithBlockUndoAndRedo2()
{
    cursor.insertText("AAAABBBB");
    int cursorPositionBefore = cursor.position();
    cursor.setPosition(0, BOBUIextCursor::KeepAnchor);
    cursor.beginEditBlock();
    cursor.removeSelectedText();
    cursor.insertText("AAAABBBBCCCCDDDD");
    cursor.endEditBlock();
    doc->undo(&cursor);
    QCOMPARE(doc->toPlainText(), QLatin1String("AAAABBBB"));
    QCOMPARE(cursor.position(), cursorPositionBefore);

    cursor.insertText("CCCC");
    QCOMPARE(doc->toPlainText(), QLatin1String("AAAABBBBCCCC"));

    cursorPositionBefore = cursor.position();
    cursor.setPosition(0, BOBUIextCursor::KeepAnchor);
    cursor.beginEditBlock();
    cursor.removeSelectedText();
    cursor.insertText("AAAABBBBCCCCDDDD");
    cursor.endEditBlock();

    /* this undo now implicitely reinserts two segments, first "CCCCC", then
       "AAAABBBB". The test ensures that the two are combined in order to
       reconstruct the correct cursor position */
    doc->undo(&cursor);


    QCOMPARE(doc->toPlainText(), QLatin1String("AAAABBBBCCCC"));
    QCOMPARE(cursor.position(), cursorPositionBefore);
}

void tst_BOBUIextCursor::cursorPositionWithBlockUndoAndRedo3()
{
    // verify that it's the position of the beginEditBlock that counts, and not the last edit position
    cursor.insertText("AAAABBBB");
    int cursorPositionBefore = cursor.position();
    cursor.beginEditBlock();
    cursor.setPosition(4);
    QVERIFY(cursor.position() != cursorPositionBefore);
    cursor.insertText("*");
    cursor.endEditBlock();
    QCOMPARE(cursor.position(), 5);
    doc->undo(&cursor);
    QCOMPARE(cursor.position(), cursorPositionBefore);
}

void tst_BOBUIextCursor::joinNonEmptyRemovedBlockUserState()
{
    cursor.insertText("Hello");
    cursor.insertBlock();
    cursor.insertText("World");
    cursor.block().setUserState(10);

    cursor.movePosition(BOBUIextCursor::EndOfBlock);
    cursor.movePosition(BOBUIextCursor::PreviousBlock, BOBUIextCursor::KeepAnchor);
    cursor.movePosition(BOBUIextCursor::EndOfBlock, BOBUIextCursor::KeepAnchor);
    cursor.removeSelectedText();

    QCOMPARE(cursor.block().userState(), 10);
}

void tst_BOBUIextCursor::crashOnDetachingDanglingCursor()
{
    BOBUIextDocument *document = new BOBUIextDocument;
    BOBUIextCursor cursor(document);
    BOBUIextCursor cursor2 = cursor;
    delete document;
    cursor2.setPosition(0); // Don't crash here
}

BOBUIEST_MAIN(tst_BOBUIextCursor)
#include "tst_bobuiextcursor.moc"
