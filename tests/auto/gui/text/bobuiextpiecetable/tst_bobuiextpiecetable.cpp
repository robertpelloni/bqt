// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QRandomGenerator>

#define protected public

#include <bobuiextdocument.h>
#undef protected
#include <private/bobuiextdocument_p.h>
#include <qabstracttextdocumentlayout.h>
#include <bobuiextobject.h>
#include <qdebug.h>
#include <stdlib.h>
#include <bobuiextcursor.h>
#include "../bobuiextdocument/common.h"

class tst_BOBUIextPieceTable : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIextPieceTable();


public slots:
    void init();
    void cleanup();
private slots:
    void insertion1();
    void insertion2();
    void insertion3();
    void insertion4();
    void insertion5();

    void removal1();
    void removal2();
    void removal3();
    void removal4();

    void undoRedo1();
    void undoRedo2();
    void undoRedo3();
    void undoRedo4();
    void undoRedo5();
    void undoRedo6();
    void undoRedo7();
    void undoRedo8();
    void undoRedo9();
    void undoRedo10();
    void undoRedo11();

    void checkDocumentChanged();
    void checkDocumentChanged2();
    void setBlockFormat();

    void blockInsertion();
    void blockInsertion2();

    void blockRemoval1();
    void blockRemoval2();
    void blockRemoval3();
    void blockRemoval4();
    void blockRemoval5();

    void checkBlockSeparation();

    void checkFrames1();
    void removeFrameDirect();
    void removeWithChildFrame();
    void clearWithFrames();

private:
    BOBUIextDocument *doc;
    BOBUIextDocumentPrivate *table;
    int blockFormatIndex;
    int charFormatIndex;
};

tst_BOBUIextPieceTable::tst_BOBUIextPieceTable()
{ doc = 0; table = 0; }


void tst_BOBUIextPieceTable::init()
{
    doc = new BOBUIextDocument(0);
    table = BOBUIextDocumentPrivate::get(doc);
    blockFormatIndex = table->formatCollection()->indexForFormat(BOBUIextBlockFormat());
    charFormatIndex = table->formatCollection()->indexForFormat(BOBUIextCharFormat());
}

void tst_BOBUIextPieceTable::cleanup()
{
    delete doc;
    doc = 0;
}

void tst_BOBUIextPieceTable::insertion1()
{
    table->insert(0, u"aacc", charFormatIndex);
    QCOMPARE(table->plainText(), QString("aacc"));
    table->insert(2, u"bb", charFormatIndex);
    QCOMPARE(table->plainText(), QString("aabbcc"));
    table->insert(1, u"1", charFormatIndex);
    QCOMPARE(table->plainText(), QString("a1abbcc"));
    table->insert(6, u"d", charFormatIndex);
    QCOMPARE(table->plainText(), QString("a1abbcdc"));
    table->insert(8, u"z", charFormatIndex);
    QCOMPARE(table->plainText(), QString("a1abbcdcz"));
}

void tst_BOBUIextPieceTable::insertion2()
{
    table->insert(0, u"bb", charFormatIndex);
    QCOMPARE(table->plainText(), QString("bb"));
}

void tst_BOBUIextPieceTable::insertion3()
{
    QString compare;
    for (int i = 0; i < 20000; ++i) {
        int pos = QRandomGenerator::global()->bounded(i+1);
        QChar c((unsigned short)(i & 0xff) + 1);
        QString str;
        str += c;
        table->insert(pos, str, charFormatIndex);
        compare.insert(pos, str);
    }
    QCOMPARE(table->plainText(), compare);
}

void tst_BOBUIextPieceTable::insertion4()
{
    QString compare;
    for (int i = 0; i < 20000; ++i) {
        int pos = QRandomGenerator::global()->generate() % (i+1);
        QChar c((unsigned short)((i % 26) + (i>25?'A':'a')));
        QString str;
        str += c;
        str += c;
        table->insert(pos, str, charFormatIndex);
        compare.insert(pos, str);
//        if (table->text() != compare) {
//            qDebug("compare failed: i=%d (current char=%c) insert at %d\nexpected '%s'\ngot      '%s'", i, (i % 26) + (i>25?'A':'a'), pos, compare.latin1(), table->text().latin1());
//            exit(12);
//        }
    }
    QCOMPARE(table->plainText(), compare);
}

void tst_BOBUIextPieceTable::insertion5()
{
    QString compare;
    for (int i = 0; i < 20000; ++i) {
        int pos = QRandomGenerator::global()->generate() % (i+1);
        QChar c((unsigned short)((i % 26) + (i>25?'A':'a')));
        QString str;
        str += c;
        str += c;
        if (c == 'a') {
            table->insertBlock(pos, blockFormatIndex, charFormatIndex);
            str = QChar(QChar::ParagraphSeparator);
        } else {
            table->insert(pos, str, charFormatIndex);
        }
        compare.insert(pos, str);
    }
    QCOMPARE(table->plainText(), compare);
    for (BOBUIextBlock it = table->blocksBegin(); it != table->blocksEnd(); it = it.next()) {
        BOBUIextDocumentPrivate::FragmentIterator fit = table->find(it.position());
        QCOMPARE(fit.position(), it.position());
    }
}

void tst_BOBUIextPieceTable::removal1()
{
    table->insert(0, u"abbccc", charFormatIndex);
    QCOMPARE(table->plainText(), QString("abbccc"));
    table->remove(1, 2);
    QCOMPARE(table->plainText(), QString("accc"));
    table->insert(1, u"1", charFormatIndex);
    QCOMPARE(table->plainText(), QString("a1ccc"));
    table->remove(4, 1);
    QCOMPARE(table->plainText(), QString("a1cc"));
    table->insert(4, u"z", charFormatIndex);
    QCOMPARE(table->plainText(), QString("a1ccz"));
}

void tst_BOBUIextPieceTable::removal2()
{
    table->insert(0, u"bb", charFormatIndex);
    QCOMPARE(table->plainText(), QString("bb"));
    table->remove(0, 2);
    QCOMPARE(table->plainText(), QString(""));
    table->insertBlock(0, blockFormatIndex, charFormatIndex);
    QCOMPARE(table->plainText(), QString(QChar(QChar::ParagraphSeparator)));
    table->remove(0, 1);
    QCOMPARE(table->plainText(), QString(""));

    table->insert(0, u"bb", charFormatIndex);
    QCOMPARE(table->plainText(), QString("bb"));
    table->insertBlock(1, blockFormatIndex, charFormatIndex);
    QCOMPARE(table->plainText(), QString("b") + QString(QChar(QChar::ParagraphSeparator)) + QString("b"));
    table->remove(1, 1);
    QCOMPARE(table->plainText(), QString("bb"));
}

void tst_BOBUIextPieceTable::removal3()
{
    QString compare;
    int l = 0;
    for (int i = 0; i < 20000; ++i) {
        bool remove = l && (QRandomGenerator::global()->bounded(2));
        int pos = QRandomGenerator::global()->bounded(remove ? l : (l+1));
        QChar c((unsigned short)((i % 26) + (i>25?'A':'a')));
        QString str;
        str += c;
        str += c;
        if (remove && pos < table->length()) {
            compare.remove(pos, 1);
            table->remove(pos, 1);
        } else {
            compare.insert(pos, str);
            table->insert(pos, str, charFormatIndex);
        }
        l += remove ? -1 : 2;
//        if (table->text() != compare) {
//            qDebug("compare failed: i=%d (current char=%c) insert at %d\nexpected '%s'\ngot      '%s'", i, (i % 26) + (i>25?'A':'a'), pos, compare.latin1(), table->text().latin1());
//            exit(12);
//        }
    }
    QCOMPARE(table->plainText(), compare);
}

void tst_BOBUIextPieceTable::removal4()
{
    QString compare;
    int l = 0;
    for (int i = 0; i < 20000; ++i) {
        bool remove = l && (QRandomGenerator::global()->bounded(2));
        int pos = (l > 1) ? QRandomGenerator::global()->bounded(remove ? l-1 : l) : 0;
        QChar c((unsigned short)((i % 26) + (i>25?'A':'a')));
        QString str;
        if (c != 'a') {
            str += c;
            str += c;
        } else {
            str = QChar(QChar::ParagraphSeparator);
        }
        if (remove && pos < table->length() - 1) {
            compare.remove(pos, 1);
            table->remove(pos, 1);
        } else {
            if (str[0] == QChar(QChar::ParagraphSeparator))
                table->insertBlock(pos, blockFormatIndex, charFormatIndex);
            else
                table->insert(pos, str, charFormatIndex);
            compare.insert(pos, str);
        }
        l += remove ? -1 : 2;
//        if (table->plainText() != compare) {
//            qDebug("compare failed: i=%d (current char=%c) insert at %d\nexpected '%s'\ngot      '%s'", i, (i % 26) + (i>25?'A':'a'), pos, compare.latin1(), table->plainText().latin1());
//            exit(12);
//        }
    }
    QCOMPARE(table->plainText(), compare);
}

void tst_BOBUIextPieceTable::undoRedo1()
{
    table->insert(0, u"01234567", charFormatIndex);
    table->insert(0, u"a", charFormatIndex);
    table->insert(1, u"b", charFormatIndex);
    QCOMPARE(table->plainText(), QString("ab01234567"));
    table->undo();
    QCOMPARE(table->plainText(), QString("01234567"));
    table->redo();
    QCOMPARE(table->plainText(), QString("ab01234567"));
    table->undo();
    table->insert(1, u"c", charFormatIndex);
    QCOMPARE(table->plainText(), QString("0c1234567"));
    table->undo();
    QCOMPARE(table->plainText(), QString("01234567"));
    table->undo();
    QVERIFY(table->plainText().isEmpty());
}

void tst_BOBUIextPieceTable::undoRedo2()
{
    table->insert(0, u"01", charFormatIndex);
    table->insert(1, u"a", charFormatIndex);
    QCOMPARE(table->plainText(), QString("0a1"));
    table->undo();
    QCOMPARE(table->plainText(), QString("01"));
    table->undo();
    QCOMPARE(table->plainText(), QString(""));
    table->redo();
    QCOMPARE(table->plainText(), QString("01"));
    table->redo();
    QCOMPARE(table->plainText(), QString("0a1"));
}

void tst_BOBUIextPieceTable::undoRedo3()
{
    table->insert(0, u"01", charFormatIndex);
    table->insert(2, u"ab", charFormatIndex);
    table->remove(2, 1);
    QCOMPARE(table->plainText(), QString("01b"));
    table->undo();
    QCOMPARE(table->plainText(), QString("01ab"));
    table->undo();
    QVERIFY(table->plainText().isEmpty());
    table->redo();
    QCOMPARE(table->plainText(), QString("01ab"));
    table->redo();
    QCOMPARE(table->plainText(), QString("01b"));
}

void tst_BOBUIextPieceTable::undoRedo4()
{
    table->insert(0, u"01", charFormatIndex);
    table->insert(0, u"ab", charFormatIndex);
    table->remove(0, 1);
    QCOMPARE(table->plainText(), QString("b01"));
    table->undo();
    QCOMPARE(table->plainText(), QString("ab01"));
    table->undo();
    QCOMPARE(table->plainText(), QString("01"));
    table->undo();
    QCOMPARE(table->plainText(), QString(""));
    table->redo();
    QCOMPARE(table->plainText(), QString("01"));
    table->redo();
    QCOMPARE(table->plainText(), QString("ab01"));
    table->redo();
    QCOMPARE(table->plainText(), QString("b01"));
}

void tst_BOBUIextPieceTable::undoRedo5()
{
    table->beginEditBlock();
    table->insert(0, u"01", charFormatIndex);
    table->remove(1, 1);
    table->endEditBlock();
    QCOMPARE(table->plainText(), QString("0"));
    table->undo();
    QCOMPARE(table->plainText(), QString(""));
}

void tst_BOBUIextPieceTable::undoRedo6()
{
    // this is essentially a test for the undoStack[undoPosition - 1].block = false in PieceTable::endUndoBlock()
    BOBUIextDocument doc;
    BOBUIextCursor cursor(&doc);
    cursor.insertText("Hello World");

    cursor.insertBlock();
    cursor.insertText("Hello World2");

    cursor.movePosition(BOBUIextCursor::Start);
    BOBUIextBlockFormat bfmt;
    bfmt.setAlignment(BobUI::AlignHCenter);
    cursor.setBlockFormat(bfmt);
    QCOMPARE(cursor.blockFormat().alignment(), BobUI::AlignHCenter);

    BOBUIextCursor range = cursor;
    range.clearSelection();
    range.movePosition(BOBUIextCursor::Start);
    range.movePosition(BOBUIextCursor::End, BOBUIextCursor::KeepAnchor);

    BOBUIextCharFormat modifier;
    modifier.setFontItalic(true);
    range.mergeCharFormat(modifier);

    cursor.movePosition(BOBUIextCursor::Start);
    QCOMPARE(cursor.blockFormat().alignment(), BobUI::AlignHCenter);

    doc.undo();

    QCOMPARE(cursor.blockFormat().alignment(), BobUI::AlignHCenter);
}

void tst_BOBUIextPieceTable::undoRedo7()
{
    table->insert(0, u"a", charFormatIndex);
    table->insert(1, u"b", charFormatIndex);
    QCOMPARE(table->plainText(), QString("ab"));

    table->undo();
    QVERIFY(table->plainText().isEmpty());
}

void tst_BOBUIextPieceTable::undoRedo8()
{
    table->insert(0, u"a", charFormatIndex);
    table->insert(1, u"b", charFormatIndex);
    QCOMPARE(table->plainText(), QString("ab"));

    table->remove(0, 1);
    table->remove(0, 1);

    QVERIFY(table->plainText().isEmpty());
    table->undo();
    QCOMPARE(table->plainText(), QString("ab"));
}

void tst_BOBUIextPieceTable::undoRedo9()
{
    table->insert(0, u"a", charFormatIndex);
    table->insert(1, u"b", charFormatIndex);
    QCOMPARE(table->plainText(), QString("ab"));

    table->remove(1, 1);
    table->remove(0, 1);

    QVERIFY(table->plainText().isEmpty());
    table->undo();
    QCOMPARE(table->plainText(), QString("ab"));
}

void tst_BOBUIextPieceTable::undoRedo10()
{
    // testcase for the beginUndoBlock/endUndoBlock calls being surrounded by an if (undoEnabled)
    BOBUIextCharFormat cf;
    cf.setForeground(BobUI::blue);
    int cfIdx = table->formatCollection()->indexForFormat(cf);

    BOBUIextBlockFormat f;
    int idx = table->formatCollection()->indexForFormat(f);

    table->insert(0, u"a", cfIdx);
    table->insertBlock(1, idx, cfIdx);
    table->insert(1, u"b", cfIdx);

    cf.setForeground(BobUI::red);
    int newCfIdx = table->formatCollection()->indexForFormat(cf);

    table->setCharFormat(0, 3, cf, BOBUIextDocumentPrivate::MergeFormat);

    QCOMPARE(table->find(0).value()->format, newCfIdx);

    table->undo();

    QCOMPARE(table->find(0).value()->format, cfIdx);
}

void tst_BOBUIextPieceTable::undoRedo11()
{
    const int loops = 20;
    QString compare;
    int l = 0;
    for (int i = 0; i < loops; ++i) {
        bool remove = l && (QRandomGenerator::global()->bounded(2));
        int pos = (l > 1) ? QRandomGenerator::global()->bounded(remove ? l-1 : l) : 0;
        QChar c((unsigned short)((i % 26) + (i>25?'A':'a')));
        QString str;
        str += c;
        str += c;
        if (remove) {
            compare.remove(pos, 1);
            table->remove(pos, 1);
        } else {
            compare.insert(pos, str);
            table->insert(pos, str, charFormatIndex);
        }
        l += remove ? -1 : 2;
    }
    QCOMPARE(table->plainText(), compare);
    for (int i = 0; i < loops; ++i)
        table->undo();
    QCOMPARE(table->plainText(), QString(""));
    for (int i = 0; i < loops; ++i)
        table->redo();
    QCOMPARE(table->plainText(), compare);
}


void tst_BOBUIextPieceTable::checkDocumentChanged()
{
    table->enableUndoRedo(false);
    BOBUIestDocumentLayout *layout = new BOBUIestDocumentLayout(doc);
    doc->setDocumentLayout(layout);

    // single insert
    layout->expect(0, 0, 15);
    table->insert(0, u"012345678901234", charFormatIndex);
    QVERIFY(!layout->error);

    // single remove
    layout->expect(0, 5, 0);
    table->remove(0, 5);
    QVERIFY(!layout->error);

    // symmetric insert/remove
    layout->expect(0, 0, 0);
    table->beginEditBlock();
    table->insert(0, u"01234", charFormatIndex);
    table->remove(0, 5);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(0, 5, 5);
    table->beginEditBlock();
    table->remove(0, 5);
    table->insert(0, u"01234", charFormatIndex);
    table->endEditBlock();
    QVERIFY(!layout->error);

    // replace
    layout->expect(0, 3, 5);
    table->beginEditBlock();
    table->remove(0, 3);
    table->insert(0, u"01234", charFormatIndex);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(0, 0, 2);
    table->beginEditBlock();
    table->insert(0, u"01234", charFormatIndex);
    table->remove(0, 3);
    table->endEditBlock();
    QVERIFY(!layout->error);

    // insert + remove inside insert block
    layout->expect(0, 0, 2);
    table->beginEditBlock();
    table->insert(0, u"01234", charFormatIndex);
    table->remove(1, 3);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(0, 0, 2);
    table->beginEditBlock();
    table->insert(0, u"01234", charFormatIndex);
    table->remove(2, 3);
    table->endEditBlock();
    QVERIFY(!layout->error);

    // insert + remove partly outside
    layout->expect(0, 1, 0);
    table->beginEditBlock();
    table->insert(1, u"0", charFormatIndex);
    table->remove(0, 2);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(0, 1, 1);
    table->beginEditBlock();
    table->insert(1, u"01", charFormatIndex);
    table->remove(0, 2);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(0, 1, 2);
    table->beginEditBlock();
    table->insert(1, u"012", charFormatIndex);
    table->remove(0, 2);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(1, 1, 0);
    table->beginEditBlock();
    table->insert(1, u"0", charFormatIndex);
    table->remove(1, 2);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(1, 1, 1);
    table->beginEditBlock();
    table->insert(1, u"01", charFormatIndex);
    table->remove(2, 2);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(1, 1, 2);
    table->beginEditBlock();
    table->insert(1, u"012", charFormatIndex);
    table->remove(3, 2);
    table->endEditBlock();
    QVERIFY(!layout->error);

    // insert + remove non overlapping
    layout->expect(0, 1, 1);
    table->beginEditBlock();
    table->insert(1, u"0", charFormatIndex);
    table->remove(0, 1);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(0, 2, 2);
    table->beginEditBlock();
    table->insert(2, u"1", charFormatIndex);
    table->remove(0, 1);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(0, 2, 2);
    table->beginEditBlock();
    table->remove(0, 1);
    table->insert(1, u"0", charFormatIndex);
    table->endEditBlock();
    QVERIFY(!layout->error);

    layout->expect(0, 3, 3);
    table->beginEditBlock();
    table->remove(0, 1);
    table->insert(2, u"1", charFormatIndex);
    table->endEditBlock();


    layout->expect(0, 3, 3);
    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::blue);
    table->beginEditBlock();
    table->setCharFormat(0, 1, fmt);
    table->setCharFormat(2, 1, fmt);
    table->endEditBlock();
    QVERIFY(!layout->error);
}

void tst_BOBUIextPieceTable::checkDocumentChanged2()
{
    BOBUIestDocumentLayout *layout = new BOBUIestDocumentLayout(doc);
    doc->setDocumentLayout(layout);

    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::blue);
    int anotherCharFormatIndex = table->formatCollection()->indexForFormat(fmt);

    layout->expect(0, 0, 12);
    table->beginEditBlock();
    table->insert(0, u"0123", charFormatIndex);
    table->insert(4, u"4567", anotherCharFormatIndex);
    table->insert(8, u"8901", charFormatIndex);
    table->endEditBlock();
    QVERIFY(!layout->error);

    fmt.setFontItalic(true);

    layout->expect(1, 10, 10);
    table->beginEditBlock();
    table->setCharFormat(8, 3, fmt);
    table->setCharFormat(4, 4, fmt);
    table->setCharFormat(1, 3, fmt);
    table->endEditBlock();
    QVERIFY(!layout->error);
}

void tst_BOBUIextPieceTable::setBlockFormat()
{
    BOBUIextBlockFormat bfmt;
    int index = table->formatCollection()->indexForFormat(bfmt);

    table->insertBlock(0, index, charFormatIndex);
    table->insertBlock(0, index, charFormatIndex);
    table->insertBlock(0, index, charFormatIndex);

    BOBUIextBlockFormat newbfmt = bfmt;
    newbfmt.setAlignment(BobUI::AlignRight);
    index = table->formatCollection()->indexForFormat(bfmt);
    BOBUIextBlock b = table->blocksFind(1);
    table->setBlockFormat(b, b, newbfmt);

    QCOMPARE(table->blocksFind(0).blockFormat(), bfmt);
    QCOMPARE(table->blocksFind(1).blockFormat(), newbfmt);
    QCOMPARE(table->blocksFind(2).blockFormat(), bfmt);
}


void tst_BOBUIextPieceTable::blockInsertion()
{
    BOBUIextBlockFormat fmt;
    fmt.setTopMargin(100);
    int idx = table->formatCollection()->indexForFormat(fmt);
    int charFormat = table->formatCollection()->indexForFormat(BOBUIextCharFormat());
    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());

    table->insertBlock(0, idx, charFormat);
    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(1).blockFormat(), fmt);

    table->undo();
    QCOMPARE(table->blockMap().length(), 1);
    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());

    table->redo();
    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(1).blockFormat(), fmt);
}

void tst_BOBUIextPieceTable::blockInsertion2()
{
    // caused evil failing assertion in fragmentmap
    int pos = 0;
    table->insertBlock(pos, blockFormatIndex, charFormatIndex);
    pos += 1;
    table->insert(pos, u"a", charFormatIndex);
    pos += 1;

    pos -= 1;
    table->insertBlock(pos, blockFormatIndex, charFormatIndex);
    QCOMPARE(table->blocksFind(0).position(), 0);
    QCOMPARE(table->blocksFind(1).position(), 1);
    QCOMPARE(table->blocksFind(2).position(), 2);
}

/*
  Tests correct removal behaviour when deleting over block boundaries or complete blocks.
*/

void tst_BOBUIextPieceTable::blockRemoval1()
{
    BOBUIextBlockFormat fmt1;
    fmt1.setTopMargin(100);
    BOBUIextBlockFormat fmt2;
    fmt2.setAlignment(BobUI::AlignRight);
    int idx1 = table->formatCollection()->indexForFormat(fmt1);
    int idx2 = table->formatCollection()->indexForFormat(fmt2);

    table->insert(0, u"0123", charFormatIndex);
    table->insertBlock(4, idx1, charFormatIndex);
    table->insert(5, u"5678", charFormatIndex);
    table->insertBlock(9, idx2, charFormatIndex);
    table->insert(10, u"0123", charFormatIndex);

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->beginEditBlock();
    table->remove(5, 5);
    table->endEditBlock();
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(4).position(), 0);
    QCOMPARE(table->blocksFind(5).position(), 5);

    table->undo();

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->redo();
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(4).position(), 0);
    QCOMPARE(table->blocksFind(5).position(), 5);
}

void tst_BOBUIextPieceTable::blockRemoval2()
{
    BOBUIextBlockFormat fmt1;
    fmt1.setTopMargin(100);
    BOBUIextBlockFormat fmt2;
    fmt2.setAlignment(BobUI::AlignRight);
    int idx1 = table->formatCollection()->indexForFormat(fmt1);
    int idx2 = table->formatCollection()->indexForFormat(fmt2);

    table->insert(0, u"0123", charFormatIndex);
    table->insertBlock(4, idx1, charFormatIndex);
    table->insert(5, u"5678", charFormatIndex);
    table->insertBlock(9, idx2, charFormatIndex);
    table->insert(10, u"0123", charFormatIndex);

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->remove(4, 1);
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(6).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 0);

    table->undo();

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->redo();
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(6).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 0);
}

void tst_BOBUIextPieceTable::blockRemoval3()
{
    BOBUIextBlockFormat fmt1;
    fmt1.setTopMargin(100);
    BOBUIextBlockFormat fmt2;
    fmt2.setAlignment(BobUI::AlignRight);
    int idx1 = table->formatCollection()->indexForFormat(fmt1);
    int idx2 = table->formatCollection()->indexForFormat(fmt2);

    table->insert(0, u"0123", charFormatIndex);
    table->insertBlock(4, idx1, charFormatIndex);
    table->insert(5, u"5678", charFormatIndex);
    table->insertBlock(9, idx2, charFormatIndex);
    table->insert(10, u"0123", charFormatIndex);

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->beginEditBlock();
    table->remove(3, 4);
    table->endEditBlock();

    QCOMPARE(table->blocksFind(1).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(5).position(), 0);

    table->undo();

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->redo();
    QCOMPARE(table->blocksFind(1).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(5).position(), 0);
}

void tst_BOBUIextPieceTable::blockRemoval4()
{
#if 0
    BOBUIextBlockFormat fmt1;
    fmt1.setTopMargin(100);
    BOBUIextBlockFormat fmt2;
    fmt2.setAlignment(BobUI::AlignRight);
    int idx1 = table->formatCollection()->indexForFormat(fmt1);
    int idx2 = table->formatCollection()->indexForFormat(fmt2);

    table->insert(0, "0123", charFormatIndex);
    table->insertBlock(4, idx1, charFormatIndex);
    table->insert(5, "5678", charFormatIndex);
    table->insertBlock(9, idx2, charFormatIndex);
    table->insert(10, "0123", charFormatIndex);

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->remove(3, 7);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(5).position(), 0);
    QCOMPARE(table->blocksFind(1).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), BOBUIextBlockFormat());

    table->undo();

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->redo();
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(5).position(), 0);
    QCOMPARE(table->blocksFind(1).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), BOBUIextBlockFormat());
#endif
}

void tst_BOBUIextPieceTable::blockRemoval5()
{
    BOBUIextBlockFormat fmt1;
    fmt1.setTopMargin(100);
    BOBUIextBlockFormat fmt2;
    fmt2.setAlignment(BobUI::AlignRight);
    int idx1 = table->formatCollection()->indexForFormat(fmt1);
    int idx2 = table->formatCollection()->indexForFormat(fmt2);

    table->insert(0, u"0123", charFormatIndex);
    table->insertBlock(4, idx1, charFormatIndex);
    table->insert(5, u"5678", charFormatIndex);
    table->insertBlock(9, idx2, charFormatIndex);
    table->insert(10, u"0123", charFormatIndex);

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->beginEditBlock();
    table->remove(3, 8);
    table->endEditBlock();

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(5).position(), 0);

    table->undo();

    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(4).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), fmt1);
    QCOMPARE(table->blocksFind(10).blockFormat(), fmt2);
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(6).position(), 5);
    QCOMPARE(table->blocksFind(11).position(), 10);

    table->redo();
    QCOMPARE(table->blocksFind(0).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(5).blockFormat(), BOBUIextBlockFormat());
    QCOMPARE(table->blocksFind(1).position(), 0);
    QCOMPARE(table->blocksFind(5).position(), 0);
}


void tst_BOBUIextPieceTable::checkBlockSeparation()
{
    table->insertBlock(0, blockFormatIndex, charFormatIndex);
    table->insertBlock(1, blockFormatIndex, charFormatIndex);

    QVERIFY(table->find(0) != table->find(1));
}

void tst_BOBUIextPieceTable::checkFrames1()
{
    BOBUIextFrameFormat ffmt;
    table->insert(0, u"Hello", charFormatIndex);
    QPointer<BOBUIextFrame> frame = table->insertFrame(1, 3, ffmt);
    BOBUIextFrame *root = table->rootFrame();

    QCOMPARE(root, frame->parentFrame());

    QVERIFY(root);
    QVERIFY(!root->parentFrame());

    QCOMPARE(root->childFrames().size(), 1);
    QVERIFY(frame->format() == ffmt);
    QCOMPARE(frame->firstPosition(), 2);
    QCOMPARE(frame->lastPosition(), 4);


    QPointer<BOBUIextFrame> frame2 = table->insertFrame(2, 3, ffmt);

    QCOMPARE(root->childFrames().size(), 1);
    QCOMPARE(root->childFrames().at(0), frame.data());
    QCOMPARE(frame->childFrames().size(), 1);
    QCOMPARE(frame2->childFrames().size(), 0);
    QCOMPARE(frame2->parentFrame(), frame.data());
    QCOMPARE(frame2->firstPosition(), 3);
    QCOMPARE(frame2->lastPosition(), 4);

    QVERIFY(frame->format() == ffmt);
    QCOMPARE(frame->firstPosition(), 2);
    QCOMPARE(frame->lastPosition(), 6);

    table->removeFrame(frame);

    QCOMPARE(root->childFrames().size(), 1);
    QCOMPARE(root->childFrames().at(0), frame2.data());
    QVERIFY(!frame);
    QCOMPARE(frame2->childFrames().size(), 0);
    QCOMPARE(frame2->parentFrame(), root);
    QCOMPARE(frame2->firstPosition(), 2);
    QCOMPARE(frame2->lastPosition(), 3);

    table->undo();

    frame = table->frameAt(2);

    QCOMPARE(root->childFrames().size(), 1);
    QCOMPARE(root->childFrames().at(0), frame.data());
    QCOMPARE(frame->childFrames().size(), 1);
    QCOMPARE(frame->childFrames().at(0), frame2.data());
    QCOMPARE(frame2->childFrames().size(), 0);
    QCOMPARE(frame2->parentFrame(), frame.data());
    QCOMPARE(frame2->firstPosition(), 3);
    QCOMPARE(frame2->lastPosition(), 4);

    QCOMPARE(frame->firstPosition(), 2);
    QCOMPARE(frame->lastPosition(), 6);

    table->undo();

    QCOMPARE(root->childFrames().size(), 1);
    QCOMPARE(root->childFrames().at(0), frame.data());
    QCOMPARE(frame->childFrames().size(), 0);
    QVERIFY(!frame2);

    QCOMPARE(frame->firstPosition(), 2);
    QCOMPARE(frame->lastPosition(), 4);
}

void tst_BOBUIextPieceTable::removeFrameDirect()
{
    BOBUIextFrameFormat ffmt;
    table->insert(0, u"Hello", charFormatIndex);

    BOBUIextFrame *frame = table->insertFrame(1, 5, ffmt);

    QCOMPARE(frame->parentFrame(), table->rootFrame());

    const int start = frame->firstPosition() - 1;
    const int end = frame->lastPosition();
    const int length = end - start + 1;

    table->remove(start, length);
}

void tst_BOBUIextPieceTable::removeWithChildFrame()
{
    /*
       The piecetable layout is:

       ...
       1 BeginningOfFrame(first frame)
       2 text
       3 BeginningOfFrame(second frame)
       4 text
       5 text
       6 EndOfFrame(second frame)
       7 text
       8 text
       9 EndOfFrame(first frame)
       ...

       The idea is to remove from [2] until [6], basically some trailing text and the second frame.
       In this case frameAt(2) != frameAt(6), so the assertion in remove() needed an adjustement.
     */
    BOBUIextFrameFormat ffmt;
    table->insert(0, u"Hello World", charFormatIndex);

    BOBUIextFrame *frame = table->insertFrame(1, 6, ffmt);
    BOBUIextFrame *childFrame = table->insertFrame(3, 5, ffmt);
    Q_UNUSED(frame);
    Q_UNUSED(childFrame);

    // used to give a failing assertion
    table->remove(2, 5);
    QVERIFY(true);
}

void tst_BOBUIextPieceTable::clearWithFrames()
{
    /*
       The piecetable layout is:

       ...
       1 BeginningOfFrame(first frame)
       2 text
       3 EndOfFrame(first frame)
       4 BeginningOfFrame(second frame)
       5 text
       6 text
       7 EndOfFrame(second frame)
       ...

       The idea is to remove from [1] until [7].
     */
    BOBUIextFrameFormat ffmt;
    table->insert(0, u"Hello World", charFormatIndex);

    BOBUIextFrame *firstFrame = table->insertFrame(1, 2, ffmt);
    BOBUIextFrame *secondFrame = table->insertFrame(4, 6, ffmt);

    const int start = firstFrame->firstPosition() - 1;
    const int end = secondFrame->lastPosition();
    const int length = end - start + 1;
    // used to give a failing assertion
    table->remove(start, length);
    QVERIFY(true);
}

BOBUIEST_MAIN(tst_BOBUIextPieceTable)


#include "tst_bobuiextpiecetable.moc"

