// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/BOBUIest>

#include <bobuiextdocument.h>
#include <qdebug.h>
#ifndef Q_OS_WIN
#  include <private/bobuiextdocument_p.h>
#endif

#include <bobuiextobject.h>
#include <bobuiextcursor.h>

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextDocument)

class tst_BOBUIextBlock : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void fragmentOverBlockBoundaries();
    void excludeParagraphSeparatorFragment();
    void backwardsBlockIterator();
    void previousBlock_bobuibug18026();
    void removedBlock_bobuibug18500();

private:
    BOBUIextDocument *doc;
    BOBUIextCursor cursor;
};

void tst_BOBUIextBlock::init()
{
    doc = new BOBUIextDocument;
    cursor = BOBUIextCursor(doc);
}

void tst_BOBUIextBlock::cleanup()
{
    cursor = BOBUIextCursor();
    delete doc;
    doc = 0;
}

void tst_BOBUIextBlock::fragmentOverBlockBoundaries()
{
    /* this creates two fragments in the piecetable:
     * 1) 'hello<parag separator here>world'
     * 2) '<parag separator>'
     * (they are not united because the former was interested after the latter,
     * hence their position in the pt buffer is the other way around)
     */
    cursor.insertText("Hello");
    cursor.insertBlock();
    cursor.insertText("World");

    cursor.movePosition(BOBUIextCursor::Start);

    const BOBUIextDocument *doc = cursor.block().document();
    QVERIFY(doc);
    // Block separators are always a fragment of their self. Thus:
    // |Hello|\b|World|\b|
#if !defined(Q_OS_WIN)
    QCOMPARE(BOBUIextDocumentPrivate::get(doc)->fragmentMap().numNodes(), 4);
#endif
    QCOMPARE(cursor.block().text(), QString("Hello"));
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.block().text(), QString("World"));
}

void tst_BOBUIextBlock::excludeParagraphSeparatorFragment()
{
    BOBUIextCharFormat fmt;
    fmt.setForeground(BobUI::blue);
    cursor.insertText("Hello", fmt);

    BOBUIextBlock block = doc->begin();
    QVERIFY(block.isValid());

    BOBUIextBlock::Iterator it = block.begin();

    BOBUIextFragment fragment = it.fragment();
    QVERIFY(fragment.isValid());
    QCOMPARE(fragment.text(), QString("Hello"));

    ++it;
    QVERIFY(it.atEnd());
    QCOMPARE(it, block.end());
}

void tst_BOBUIextBlock::backwardsBlockIterator()
{
    BOBUIextCharFormat fmt;

    fmt.setForeground(BobUI::magenta);
    cursor.insertText("A", fmt);

    fmt.setForeground(BobUI::red);
    cursor.insertText("A", fmt);

    fmt.setForeground(BobUI::magenta);
    cursor.insertText("A", fmt);

    BOBUIextBlock block = doc->begin();
    QVERIFY(block.isValid());

    BOBUIextBlock::Iterator it = block.begin();
    QCOMPARE(it.fragment().position(), 0);
    ++it;
    QCOMPARE(it.fragment().position(), 1);
    ++it;

    QCOMPARE(it.fragment().position(), 2);

    --it;
    QCOMPARE(it.fragment().position(), 1);
    --it;
    QCOMPARE(it.fragment().position(), 0);
}

void tst_BOBUIextBlock::previousBlock_bobuibug18026()
{
    BOBUIextBlock last = doc->end().previous();
    QVERIFY(last.isValid());
}

void tst_BOBUIextBlock::removedBlock_bobuibug18500()
{
    cursor.insertText("line 1\nline 2\nline 3 \nline 4\n");
    cursor.setPosition(7);
    BOBUIextBlock block = cursor.block();
    cursor.setPosition(21, BOBUIextCursor::KeepAnchor);

    cursor.removeSelectedText();
    QVERIFY(!block.isValid());
}

BOBUIEST_MAIN(tst_BOBUIextBlock)
#include "tst_bobuiextblock.moc"
