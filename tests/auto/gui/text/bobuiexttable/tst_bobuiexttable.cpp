// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include <qbuffer.h>
#include <bobuiextdocument.h>
#include <bobuiextdocumentfragment.h>
#include <bobuiexttable.h>
#include <qdebug.h>
#include <bobuiextcursor.h>
#include <bobuiextdocument.h>
#ifndef BOBUI_NO_WIDGETS
#include <bobuiextedit.h>
#endif
#ifndef BOBUI_NO_PRINTER
#include <QPagedPaintDevice>
#include <QPainter>
#include <QPaintEngine>
#endif
#include <private/bobuiextdocumentlayout_p.h>
#include <private/qpagedpaintdevice_p.h>

typedef QList<int> IntList;

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextDocument)

Q_DECLARE_METATYPE(BOBUIextFrameFormat::BorderStyle);

class tst_BOBUIextTable : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void cursorPositioning();
    void variousTableModifications();
    void tableShrinking();
    void spans();
    void variousModifications2();
    void tableManager_undo();
    void tableManager_removeCell();
    void rowAt();
    void rowAtWithSpans();
    void multiBlockCells();
    void insertRows();
    void deleteInTable();
    void mergeCells();
    void mergeAndInsert();
    void splitCells();
    void blocksForTableShouldHaveEmptyFormat();
    void removeTableByRemoveRows();
    void removeTableByRemoveColumns();
    void setCellFormat();
    void removeRows1();
    void removeRows2();
    void removeRows3();
    void removeRows4();
    void removeRows5();
    void removeColumns1();
    void removeColumns2();
    void removeColumns3();
    void removeColumns4();
    void removeColumns5();
    void removeColumnsInTableWithMergedRows();
#ifndef BOBUI_NO_WIDGETS
    void BOBUIBUG11282_insertBeforeMergedEnding_data();
    void BOBUIBUG11282_insertBeforeMergedEnding();
#endif
    void BOBUIBUG22011_insertBeforeRowSpan();
#if !defined(BOBUI_NO_PRINTER) && defined(BOBUI_BUILD_INTERNAL)
    void BOBUIBUG31330_renderBackground();
#endif
    void checkBorderAttributes_data();
    void checkBorderAttributes();
    void checkTableBorderAttributes_data();
    void checkTableBorderAttributes();

#ifndef BOBUI_NO_WIDGETS
    void columnWidthWithSpans();

    void columnWidthWithImage_data();
    void columnWidthWithImage();

    void BOBUIBUG138678_insertColumnAtStartWithRowspan();
#endif

private:
    BOBUIextTable *create2x2Table();
    BOBUIextTable *create4x4Table();

    BOBUIextTable *createTable(int rows, int cols);

    BOBUIextDocument *doc;
    BOBUIextCursor cursor;
};

void tst_BOBUIextTable::init()
{
    doc = new BOBUIextDocument;
    cursor = BOBUIextCursor(doc);
}

void tst_BOBUIextTable::cleanup()
{
    cursor = BOBUIextCursor();
    delete doc;
    doc = 0;
}

void tst_BOBUIextTable::cursorPositioning()
{
    // ensure the cursor is placed at the beginning of the first cell upon
    // table creation
    BOBUIextTable *table = cursor.insertTable(2, 2);

    QVERIFY(cursor == table->cellAt(0, 0).firstCursorPosition());
    QVERIFY(table->cellAt(0, 0).firstPosition() == table->firstPosition());
}

void tst_BOBUIextTable::variousTableModifications()
{
    BOBUIextTableFormat tableFmt;

    BOBUIextTable *tab = cursor.insertTable(2, 2, tableFmt);
    QCOMPARE(doc->toPlainText().size(), 5);
    QCOMPARE(tab, cursor.currentTable());
    QCOMPARE(tab->columns(), 2);
    QCOMPARE(tab->rows(), 2);

    QCOMPARE(cursor.position(), 1);
    BOBUIextCharFormat fmt = cursor.charFormat();
    QCOMPARE(fmt.objectIndex(), -1);
    BOBUIextTableCell cell = tab->cellAt(cursor);
    QVERIFY(cell.isValid());
    QCOMPARE(cell.row(), 0);
    QCOMPARE(cell.column(), 0);

    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.position(), 2);
    fmt = cursor.charFormat();
    QCOMPARE(fmt.objectIndex(), -1);
    cell = tab->cellAt(cursor);
    QVERIFY(cell.isValid());
    QCOMPARE(cell.row(), 0);
    QCOMPARE(cell.column(), 1);

    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.position(), 3);
    fmt = cursor.charFormat();
    QCOMPARE(fmt.objectIndex(), -1);
    cell = tab->cellAt(cursor);
    QVERIFY(cell.isValid());
    QCOMPARE(cell.row(), 1);
    QCOMPARE(cell.column(), 0);

    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.position(), 4);
    fmt = cursor.charFormat();
    QCOMPARE(fmt.objectIndex(), -1);
    cell = tab->cellAt(cursor);
    QVERIFY(cell.isValid());
    QCOMPARE(cell.row(), 1);
    QCOMPARE(cell.column(), 1);

    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.position(), 5);
    fmt = cursor.charFormat();
    QCOMPARE(fmt.objectIndex(), -1);
    cell = tab->cellAt(cursor);
    QVERIFY(!cell.isValid());

    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.position(), 5);

    // check we can't delete the cells with the cursor
    cursor.movePosition(BOBUIextCursor::Start);
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.position(), 1);
    cursor.deleteChar();
    QCOMPARE(doc->toPlainText().size(), 5);
    cursor.movePosition(BOBUIextCursor::NextBlock);
    QCOMPARE(cursor.position(), 2);
    cursor.deleteChar();
    QCOMPARE(doc->toPlainText().size(), 5);
    cursor.deletePreviousChar();
    QCOMPARE(cursor.position(), 2);
    QCOMPARE(doc->toPlainText().size(), 5);

    BOBUIextTable *table = cursor.currentTable();
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);

    table->insertRows(2, 1);
    QCOMPARE(table->rows(), 3);
    QCOMPARE(table->columns(), 2);
    QCOMPARE(doc->toPlainText().size(), 7);
    table->insertColumns(2, 2);
    QCOMPARE(table->rows(), 3);
    QCOMPARE(table->columns(), 4);
    QCOMPARE(doc->toPlainText().size(), 13);

    table->resize(4, 5);
    QCOMPARE(table->rows(), 4);
    QCOMPARE(table->columns(), 5);
    QCOMPARE(doc->toPlainText().size(), 21);
}

void tst_BOBUIextTable::tableShrinking()
{
    BOBUIextTableFormat tableFmt;

    cursor.insertTable(3, 4, tableFmt);
    QCOMPARE(doc->toPlainText().size(), 13);

    BOBUIextTable *table = cursor.currentTable();
    QCOMPARE(table->rows(), 3);
    QCOMPARE(table->columns(), 4);

    table->removeRows(1, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 4);
    QCOMPARE(doc->toPlainText().size(), 9);
    table->removeColumns(1, 2);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);
    QCOMPARE(doc->toPlainText().size(), 5);

    table->resize(1, 1);
    QCOMPARE(table->rows(), 1);
    QCOMPARE(table->columns(), 1);
    QCOMPARE(doc->toPlainText().size(), 2);
}

void tst_BOBUIextTable::spans()
{
    BOBUIextTableFormat tableFmt;

    cursor.insertTable(2, 2, tableFmt);

    BOBUIextTable *table = cursor.currentTable();
    QVERIFY(table->cellAt(0, 0) != table->cellAt(0, 1));
    table->mergeCells(0, 0, 1, 2);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);
    QVERIFY(table->cellAt(0, 0) == table->cellAt(0, 1));
    table->mergeCells(0, 0, 2, 2);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);
}

void tst_BOBUIextTable::variousModifications2()
{
    BOBUIextTableFormat tableFmt;

    cursor.insertTable(2, 5, tableFmt);
    QCOMPARE(doc->toPlainText().size(), 11);
    BOBUIextTable *table = cursor.currentTable();
    QCOMPARE(cursor.position(), 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 5);

    table->insertColumns(0, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 6);
    table->insertColumns(6, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 7);

    table->insertRows(0, 1);
    QCOMPARE(table->rows(), 3);
    QCOMPARE(table->columns(), 7);
    table->insertRows(3, 1);
    QCOMPARE(table->rows(), 4);
    QCOMPARE(table->columns(), 7);

    table->removeRows(0, 1);
    QCOMPARE(table->rows(), 3);
    QCOMPARE(table->columns(), 7);
    table->removeRows(2, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 7);

    table->removeColumns(0, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 6);
    table->removeColumns(5, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 5);

    tableFmt = table->format();
    table->insertColumns(2, 1);
    table->setFormat(tableFmt);
    table->insertColumns(2, 1);
    QCOMPARE(table->columns(), 7);
}

void tst_BOBUIextTable::tableManager_undo()
{
    BOBUIextTableFormat fmt;
    fmt.setBorder(10);
    BOBUIextTable *table = cursor.insertTable(2, 2, fmt);
    QVERIFY(table);

    QCOMPARE(table->format().border(), qreal(10));

    fmt.setBorder(20);
    table->setFormat(fmt);

    QCOMPARE(table->format().border(), qreal(20));

    doc->undo();

    QCOMPARE(table->format().border(), qreal(10));
}

void tst_BOBUIextTable::tableManager_removeCell()
{
    // essentially a test for TableManager::removeCell, in particular to remove empty items from the rowlist.
    // If it fails it'll triger assertions inside TableManager. Yeah, not pretty, should VERIFY here ;(
    cursor.insertTable(2, 2, BOBUIextTableFormat());
    doc->undo();
    // ###
    QVERIFY(true);
}

void tst_BOBUIextTable::rowAt()
{
    // test TablePrivate::rowAt
    BOBUIextTable *table = cursor.insertTable(4, 2);

    QCOMPARE(table->rows(), 4);
    QCOMPARE(table->columns(), 2);

    BOBUIextCursor cell00Cursor = table->cellAt(0, 0).firstCursorPosition();
    BOBUIextCursor cell10Cursor = table->cellAt(1, 0).firstCursorPosition();
    BOBUIextCursor cell20Cursor = table->cellAt(2, 0).firstCursorPosition();
    BOBUIextCursor cell21Cursor = table->cellAt(2, 1).firstCursorPosition();
    BOBUIextCursor cell30Cursor = table->cellAt(3, 0).firstCursorPosition();
    QCOMPARE(table->cellAt(cell00Cursor).firstCursorPosition(), cell00Cursor);
    QCOMPARE(table->cellAt(cell10Cursor).firstCursorPosition(), cell10Cursor);
    QCOMPARE(table->cellAt(cell20Cursor).firstCursorPosition(), cell20Cursor);
    QCOMPARE(table->cellAt(cell30Cursor).firstCursorPosition(), cell30Cursor);

    table->mergeCells(1, 0, 2, 1);

    QCOMPARE(table->rows(), 4);
    QCOMPARE(table->columns(), 2);

    QVERIFY(cell00Cursor == table->cellAt(0, 0).firstCursorPosition());
    QVERIFY(cell10Cursor == table->cellAt(1, 0).firstCursorPosition());
    QVERIFY(cell10Cursor == table->cellAt(2, 0).firstCursorPosition());
    QVERIFY(cell21Cursor == table->cellAt(2, 1).firstCursorPosition());
    QVERIFY(cell30Cursor == table->cellAt(3, 0).firstCursorPosition());

    table->mergeCells(1, 0, 2, 2);

    QCOMPARE(table->rows(), 4);
    QCOMPARE(table->columns(), 2);

    QVERIFY(cell00Cursor == table->cellAt(0, 0).firstCursorPosition());
    QVERIFY(cell00Cursor == table->cellAt(0, 0).firstCursorPosition());
    QVERIFY(cell10Cursor == table->cellAt(1, 0).firstCursorPosition());
    QVERIFY(cell10Cursor == table->cellAt(1, 1).firstCursorPosition());
    QVERIFY(cell10Cursor == table->cellAt(2, 0).firstCursorPosition());
    QVERIFY(cell10Cursor == table->cellAt(2, 1).firstCursorPosition());
    QVERIFY(cell30Cursor == table->cellAt(3, 0).firstCursorPosition());
}

void tst_BOBUIextTable::rowAtWithSpans()
{
    BOBUIextTable *table = cursor.insertTable(2, 2);

    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);

    table->mergeCells(0, 0, 2, 1);
    QVERIFY(table->cellAt(0, 0).rowSpan() == 2);

    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);

    table->mergeCells(0, 0, 2, 2);
    QVERIFY(table->cellAt(0, 0).columnSpan() == 2);

    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);
}

void tst_BOBUIextTable::multiBlockCells()
{
    // little testcase for multi-block cells
    BOBUIextTable *table = cursor.insertTable(2, 2);

    QVERIFY(cursor == table->cellAt(0, 0).firstCursorPosition());

    cursor.insertText("Hello");
    cursor.insertBlock(BOBUIextBlockFormat());
    cursor.insertText("World");

    cursor.movePosition(BOBUIextCursor::Left);
    QVERIFY(table->cellAt(0, 0) == table->cellAt(cursor));
}

void tst_BOBUIextTable::insertRows()
{
    // little testcase for multi-block cells
    BOBUIextTable *table = cursor.insertTable(2, 2);

    QVERIFY(cursor == table->cellAt(0, 0).firstCursorPosition());

    table->insertRows(0, 1);
    QCOMPARE(table->rows(), 3);

    table->insertRows(1, 1);
    QCOMPARE(table->rows(), 4);

    table->insertRows(-1, 1);
    QCOMPARE(table->rows(), 5);

    table->insertRows(5, 2);
    QCOMPARE(table->rows(), 7);

    table = cursor.insertTable(5,5);
    table->mergeCells(0,0,3,3);
    table->insertRows(2,1);

    QCOMPARE(table->rows(), 6);

}

void tst_BOBUIextTable::deleteInTable()
{
    BOBUIextTable *table = cursor.insertTable(2, 2);
    table->cellAt(0, 0).firstCursorPosition().insertText("Blah");
    table->cellAt(0, 1).firstCursorPosition().insertText("Foo");
    table->cellAt(1, 0).firstCursorPosition().insertText("Bar");
    table->cellAt(1, 1).firstCursorPosition().insertText("Hah");

    cursor = table->cellAt(1, 1).firstCursorPosition();
    cursor.movePosition(BOBUIextCursor::PreviousBlock, BOBUIextCursor::KeepAnchor);

    QCOMPARE(table->cellAt(cursor.position()).row(), 1);
    QCOMPARE(table->cellAt(cursor.position()).column(), 0);

    cursor.removeSelectedText();

    QCOMPARE(table->columns(), 2);
    QCOMPARE(table->rows(), 2);

    // verify table is still all in shape. Only the text inside should get deleted
    for (int row = 0; row < table->rows(); ++row)
        for (int col = 0; col < table->columns(); ++col) {
            const BOBUIextTableCell cell = table->cellAt(row, col);
            QVERIFY(cell.isValid());
            QCOMPARE(cell.rowSpan(), 1);
            QCOMPARE(cell.columnSpan(), 1);
        }
}

BOBUIextTable *tst_BOBUIextTable::create2x2Table()
{
    cleanup();
    init();
    BOBUIextTable *table = cursor.insertTable(2, 2);
    table->cellAt(0, 0).firstCursorPosition().insertText("Blah");
    table->cellAt(0, 1).firstCursorPosition().insertText("Foo");
    table->cellAt(1, 0).firstCursorPosition().insertText("Bar");
    table->cellAt(1, 1).firstCursorPosition().insertText("Hah");
    return table;
}

BOBUIextTable *tst_BOBUIextTable::create4x4Table()
{
    cleanup();
    init();
    BOBUIextTable *table = cursor.insertTable(4, 4);
    table->cellAt(0, 0).firstCursorPosition().insertText("Blah");
    table->cellAt(0, 1).firstCursorPosition().insertText("Foo");
    table->cellAt(1, 0).firstCursorPosition().insertText("Bar");
    table->cellAt(1, 1).firstCursorPosition().insertText("Hah");
    return table;
}

BOBUIextTable *tst_BOBUIextTable::createTable(int rows, int cols)
{
    cleanup();
    init();
    BOBUIextTable *table = cursor.insertTable(rows, cols);
    return table;
}

void tst_BOBUIextTable::mergeCells()
{
    BOBUIextTable *table = create4x4Table();

    table->mergeCells(1, 1, 1, 2);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(1, 2));

    table->mergeCells(1, 1, 2, 2);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 2));

    table = create4x4Table();

    table->mergeCells(1, 1, 2, 1);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 1));

    table->mergeCells(1, 1, 2, 2);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 2));

    table = create4x4Table();

    table->mergeCells(1, 1, 2, 2);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 2));

    // should do nothing
    table->mergeCells(1, 1, 1, 1);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 2));

    table = create2x2Table();

    table->mergeCells(0, 1, 2, 1);
    table->mergeCells(0, 0, 2, 2);
    QVERIFY(table->cellAt(0, 0) == table->cellAt(0, 1));
    QVERIFY(table->cellAt(0, 0) == table->cellAt(1, 0));
    QVERIFY(table->cellAt(0, 0) == table->cellAt(1, 1));

    BOBUIextBlock block = table->cellAt(0, 0).firstCursorPosition().block();

    QCOMPARE(block.text(), QLatin1String("Blah Foo"));
    QCOMPARE(block.next().text(), QLatin1String("Hah"));
    QCOMPARE(block.next().next().text(), QLatin1String("Bar"));

    table = create4x4Table();

    BOBUIextCursor cursor = table->cellAt(3, 3).firstCursorPosition();
    BOBUIextTable *t2 = cursor.insertTable(2, 2);
    t2->cellAt(0, 0).firstCursorPosition().insertText("Test");

    table->mergeCells(2, 2, 2, 2);
    cursor = table->cellAt(2, 2).firstCursorPosition();

    BOBUIextFrame *frame = cursor.currentFrame();

    BOBUIextFrame::iterator it = frame->begin();

    // find the embedded table
    while (it != frame->end() && !it.currentFrame())
        ++it;

    table = qobject_cast<BOBUIextTable *>(it.currentFrame());

    QVERIFY(table);

    if (table) {
        cursor = table->cellAt(0, 0).firstCursorPosition();

        QCOMPARE(cursor.block().text(), QLatin1String("Test"));
    }

    table = create2x2Table();

    table->mergeCells(0, 1, 2, 1);

    QVERIFY(table->cellAt(0, 0) != table->cellAt(0, 1));
    QVERIFY(table->cellAt(0, 1) == table->cellAt(1, 1));

    // should do nothing
    table->mergeCells(0, 0, 1, 2);

    QVERIFY(table->cellAt(0, 0) != table->cellAt(0, 1));
    QVERIFY(table->cellAt(0, 1) == table->cellAt(1, 1));
}

void tst_BOBUIextTable::mergeAndInsert()
{
    BOBUIextTable *table = cursor.insertTable(4,3);
    table->mergeCells(0,1,3,2);
    table->mergeCells(3,0,1,3);
    //Don't crash !
    table->insertColumns(1,2);
    QCOMPARE(table->columns(), 5);
}

void tst_BOBUIextTable::splitCells()
{
    BOBUIextTable *table = create4x4Table();
    table->mergeCells(1, 1, 2, 2);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 2));

    table->splitCell(1, 1, 1, 2);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(2, 2));

    table->splitCell(1, 1, 1, 1);
    QVERIFY(table->cellAt(1, 1) != table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(2, 2));


    table = create4x4Table();
    table->mergeCells(1, 1, 2, 2);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 2));

    table->splitCell(1, 1, 2, 1);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(2, 2));

    table->splitCell(1, 1, 1, 1);
    QVERIFY(table->cellAt(1, 1) != table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(2, 2));


    table = create4x4Table();
    table->mergeCells(1, 1, 2, 2);
    QVERIFY(table->cellAt(1, 1) == table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) == table->cellAt(2, 2));

    table->splitCell(1, 1, 1, 1);
    QVERIFY(table->cellAt(1, 1) != table->cellAt(1, 2));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(2, 1));
    QVERIFY(table->cellAt(1, 1) != table->cellAt(2, 2));

    table = createTable(2, 5);
    table->mergeCells(0, 0, 2, 1);
    table->mergeCells(0, 1, 2, 1);
    QVERIFY(table->cellAt(0, 0) == table->cellAt(1, 0));
    QVERIFY(table->cellAt(0, 1) == table->cellAt(1, 1));
    table->splitCell(0, 0, 1, 1);
    QVERIFY(table->cellAt(0, 0) != table->cellAt(1, 0));
    QVERIFY(table->cellAt(0, 1) == table->cellAt(1, 1));

    table = createTable(2, 5);
    table->mergeCells(0, 4, 2, 1);
    QVERIFY(table->cellAt(0, 4) == table->cellAt(1, 4));

    table->splitCell(0, 4, 1, 1);
    QVERIFY(table->cellAt(0, 4) != table->cellAt(1, 4));
}

void tst_BOBUIextTable::blocksForTableShouldHaveEmptyFormat()
{
    BOBUIextBlockFormat fmt;
    fmt.setProperty(BOBUIextFormat::UserProperty, true);
    cursor.insertBlock(fmt);
    QVERIFY(cursor.blockFormat().hasProperty(BOBUIextFormat::UserProperty));

    BOBUIextTable *table = cursor.insertTable(1, 1);
    QVERIFY(!table->cellAt(0, 0).firstCursorPosition().blockFormat().hasProperty(BOBUIextFormat::UserProperty));

    int userPropCount = 0;
    for (BOBUIextBlock block = doc->begin();
         block.isValid(); block = block.next()) {
        if (block.blockFormat().hasProperty(BOBUIextFormat::UserProperty))
            userPropCount++;
    }
    QCOMPARE(userPropCount, 1);
}

void tst_BOBUIextTable::removeTableByRemoveRows()
{
    QPointer<BOBUIextTable> table1 = BOBUIextCursor(cursor).insertTable(4, 4);
    QPointer<BOBUIextTable> table2 = BOBUIextCursor(cursor).insertTable(4, 4);
    QPointer<BOBUIextTable> table3 = BOBUIextCursor(cursor).insertTable(4, 4);

    QVERIFY(table1);
    QVERIFY(table2);
    QVERIFY(table3);

    table2->removeRows(1, 1);

    QVERIFY(table1);
    QVERIFY(table2);
    QVERIFY(table3);

    table2->removeRows(0, table2->rows());

    QVERIFY(table1);
    QVERIFY(!table2);
    QVERIFY(table3);
}

void tst_BOBUIextTable::removeTableByRemoveColumns()
{
    QPointer<BOBUIextTable> table1 = BOBUIextCursor(cursor).insertTable(4, 4);
    QPointer<BOBUIextTable> table2 = BOBUIextCursor(cursor).insertTable(4, 4);
    QPointer<BOBUIextTable> table3 = BOBUIextCursor(cursor).insertTable(4, 4);

    QVERIFY(table1);
    QVERIFY(table2);
    QVERIFY(table3);

    table2->removeColumns(1, 1);

    QVERIFY(table1);
    QVERIFY(table2);
    QVERIFY(table3);

    table2->removeColumns(0, table2->columns());

    QVERIFY(table1);
    QVERIFY(!table2);
    QVERIFY(table3);
}

void tst_BOBUIextTable::setCellFormat()
{
    BOBUIextTable *table = cursor.insertTable(2, 2);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(1, 0).firstCursorPosition().insertText("Third");
    table->cellAt(1, 1).firstCursorPosition().insertText("Fourth");
    BOBUIextTableCell cell = table->cellAt(0, 0);
    BOBUIextCharFormat fmt;
    fmt.setObjectIndex(23);
    fmt.setBackground(BobUI::blue);
    fmt.setTableCellColumnSpan(25);
    fmt.setTableCellRowSpan(42);
    cell.setFormat(fmt);
    QCOMPARE(cell.format().background().color(), QColor(BobUI::blue));
    QCOMPARE(cell.format().tableCellColumnSpan(), 1);
    QCOMPARE(cell.format().tableCellRowSpan(), 1);
}

void tst_BOBUIextTable::removeRows1()
{
    BOBUIextTable *table = cursor.insertTable(2, 2);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(1, 0).firstCursorPosition().insertText("Third");
    table->cellAt(1, 1).firstCursorPosition().insertText("Fourth");
    table->removeRows(0, 1);
    QCOMPARE(table->rows(), 1);
    QCOMPARE(table->columns(), 2);
    QCOMPARE(table->cellAt(0, 0).firstCursorPosition().block().text(), QString("Third"));
    QCOMPARE(table->cellAt(0, 1).firstCursorPosition().block().text(), QString("Fourth"));
}

void tst_BOBUIextTable::removeRows2()
{
    BOBUIextTable *table = cursor.insertTable(2, 2);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(1, 0).firstCursorPosition().insertText("Third");
    table->cellAt(1, 1).firstCursorPosition().insertText("Fourth");
    table->removeRows(1, 1);
    QCOMPARE(table->rows(), 1);
    QCOMPARE(table->columns(), 2);
    QCOMPARE(table->cellAt(0, 0).firstCursorPosition().block().text(), QString("First"));
    QCOMPARE(table->cellAt(0, 1).firstCursorPosition().block().text(), QString("Second"));
}

void tst_BOBUIextTable::removeRows3()
{
    BOBUIextTable *table = cursor.insertTable(3, 2);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(1, 0).firstCursorPosition().insertText("Third");
    table->cellAt(1, 1).firstCursorPosition().insertText("Fourth");
    table->cellAt(2, 0).firstCursorPosition().insertText("Fifth");
    table->cellAt(2, 1).firstCursorPosition().insertText("Sixth");
    table->removeRows(1, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);
    QCOMPARE(table->cellAt(0, 0).firstCursorPosition().block().text(), QString("First"));
    QCOMPARE(table->cellAt(0, 1).firstCursorPosition().block().text(), QString("Second"));
    QCOMPARE(table->cellAt(1, 0).firstCursorPosition().block().text(), QString("Fifth"));
    QCOMPARE(table->cellAt(1, 1).firstCursorPosition().block().text(), QString("Sixth"));
}

void tst_BOBUIextTable::removeRows4()
{
    BOBUIextTable *table = cursor.insertTable(4, 2);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(1, 0).firstCursorPosition().insertText("Third");
    table->cellAt(1, 1).firstCursorPosition().insertText("Fourth");
    table->cellAt(2, 0).firstCursorPosition().insertText("Fifth");
    table->cellAt(2, 1).firstCursorPosition().insertText("Sixth");
    table->cellAt(3, 0).firstCursorPosition().insertText("Seventh");
    table->cellAt(3, 1).firstCursorPosition().insertText("Eighth");
    table->removeRows(1, 2);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);
    QCOMPARE(table->cellAt(0, 0).firstCursorPosition().block().text(), QString("First"));
    QCOMPARE(table->cellAt(0, 1).firstCursorPosition().block().text(), QString("Second"));
    QCOMPARE(table->cellAt(1, 0).firstCursorPosition().block().text(), QString("Seventh"));
    QCOMPARE(table->cellAt(1, 1).firstCursorPosition().block().text(), QString("Eighth"));
}

void tst_BOBUIextTable::removeRows5()
{
    BOBUIextTable *table = cursor.insertTable(2,2);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(1, 0).firstCursorPosition().insertText("Third");
    table->cellAt(1, 1).firstCursorPosition().insertText("Fourth");
    table->insertRows(1,1);
    table->mergeCells(1,0,1,2);
    table->removeRows(1,1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);
    QCOMPARE(table->cellAt(0, 0).firstCursorPosition().block().text(), QString("First"));
    QCOMPARE(table->cellAt(0, 1).firstCursorPosition().block().text(), QString("Second"));
    QCOMPARE(table->cellAt(1, 0).firstCursorPosition().block().text(), QString("Third"));
    QCOMPARE(table->cellAt(1, 1).firstCursorPosition().block().text(), QString("Fourth"));
}

void tst_BOBUIextTable::removeColumns1()
{
    BOBUIextTable *table = cursor.insertTable(2, 2);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(1, 0).firstCursorPosition().insertText("Third");
    table->cellAt(1, 1).firstCursorPosition().insertText("Fourth");
    table->removeColumns(0, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 1);
    QCOMPARE(table->cellAt(0, 0).firstCursorPosition().block().text(), QString("Second"));
    QCOMPARE(table->cellAt(1, 0).firstCursorPosition().block().text(), QString("Fourth"));
}

void tst_BOBUIextTable::removeColumns2()
{
    BOBUIextTable *table = cursor.insertTable(2, 2);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(1, 0).firstCursorPosition().insertText("Third");
    table->cellAt(1, 1).firstCursorPosition().insertText("Fourth");
    table->removeColumns(1, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 1);
    QCOMPARE(table->cellAt(0, 0).firstCursorPosition().block().text(), QString("First"));
    QCOMPARE(table->cellAt(1, 0).firstCursorPosition().block().text(), QString("Third"));
}

void tst_BOBUIextTable::removeColumns3()
{
    BOBUIextTable *table = cursor.insertTable(2, 3);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(0, 2).firstCursorPosition().insertText("Third");
    table->cellAt(1, 0).firstCursorPosition().insertText("Fourth");
    table->cellAt(1, 1).firstCursorPosition().insertText("Fifth");
    table->cellAt(1, 2).firstCursorPosition().insertText("Sixth");
    table->removeColumns(1, 1);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);
    QCOMPARE(table->cellAt(0, 0).firstCursorPosition().block().text(), QString("First"));
    QCOMPARE(table->cellAt(0, 1).firstCursorPosition().block().text(), QString("Third"));
    QCOMPARE(table->cellAt(1, 0).firstCursorPosition().block().text(), QString("Fourth"));
    QCOMPARE(table->cellAt(1, 1).firstCursorPosition().block().text(), QString("Sixth"));
}

void tst_BOBUIextTable::removeColumns4()
{
    BOBUIextTable *table = cursor.insertTable(2, 4);
    table->cellAt(0, 0).firstCursorPosition().insertText("First");
    table->cellAt(0, 1).firstCursorPosition().insertText("Second");
    table->cellAt(0, 2).firstCursorPosition().insertText("Third");
    table->cellAt(0, 3).firstCursorPosition().insertText("Fourth");
    table->cellAt(1, 0).firstCursorPosition().insertText("Fifth");
    table->cellAt(1, 1).firstCursorPosition().insertText("Sixth");
    table->cellAt(1, 2).firstCursorPosition().insertText("Seventh");
    table->cellAt(1, 3).firstCursorPosition().insertText("Eighth");
    table->removeColumns(1, 2);
    QCOMPARE(table->rows(), 2);
    QCOMPARE(table->columns(), 2);
    QCOMPARE(table->cellAt(0, 0).firstCursorPosition().block().text(), QString("First"));
    QCOMPARE(table->cellAt(0, 1).firstCursorPosition().block().text(), QString("Fourth"));
    QCOMPARE(table->cellAt(1, 0).firstCursorPosition().block().text(), QString("Fifth"));
    QCOMPARE(table->cellAt(1, 1).firstCursorPosition().block().text(), QString("Eighth"));
}

void tst_BOBUIextTable::removeColumns5()
{
    BOBUIextTable *table = cursor.insertTable(4, 4);
    BOBUIextCursor tc (doc);
    tc.setPosition(table->cellAt(2,0).firstPosition());
    tc.setPosition(table->cellAt(3,1).firstPosition(), BOBUIextCursor::KeepAnchor);
    table->mergeCells(tc);
    QCOMPARE(table->rows(), 4);
    QCOMPARE(table->columns(), 4);
    QCOMPARE(table->cellAt(0, 0).firstPosition(), 1);
    QCOMPARE(table->cellAt(0, 1).firstPosition(), 2);
    QCOMPARE(table->cellAt(0, 2).firstPosition(), 3);
    QCOMPARE(table->cellAt(0, 3).firstPosition(), 4);
    QCOMPARE(table->cellAt(1, 0).firstPosition(), 5);
    QCOMPARE(table->cellAt(1, 1).firstPosition(), 6);
    QCOMPARE(table->cellAt(1, 2).firstPosition(), 7);
    QCOMPARE(table->cellAt(1, 3).firstPosition(), 8);
    QCOMPARE(table->cellAt(2, 0).firstPosition(), 9);
    QCOMPARE(table->cellAt(2, 0).rowSpan(), 2);
    QCOMPARE(table->cellAt(2, 0).columnSpan(), 2);
    QCOMPARE(table->cellAt(2, 1).firstPosition(), 9);
    QCOMPARE(table->cellAt(2, 2).firstPosition(), 10);
    QCOMPARE(table->cellAt(2, 3).firstPosition(), 11);
    QCOMPARE(table->cellAt(3, 0).firstPosition(), 9);
    QCOMPARE(table->cellAt(3, 1).firstPosition(), 9);
    QCOMPARE(table->cellAt(3, 2).firstPosition(), 12);
    QCOMPARE(table->cellAt(3, 3).firstPosition(), 13);

    table->removeColumns(1, 1);
    QCOMPARE(table->rows(), 4);
    QCOMPARE(table->columns(), 3);
    QCOMPARE(table->cellAt(0, 0).firstPosition(), 1);
    QCOMPARE(table->cellAt(0, 1).firstPosition(), 2);
    QCOMPARE(table->cellAt(0, 2).firstPosition(), 3);
    QCOMPARE(table->cellAt(1, 0).firstPosition(), 4);
    QCOMPARE(table->cellAt(1, 1).firstPosition(), 5);
    QCOMPARE(table->cellAt(1, 2).firstPosition(), 6);
    QCOMPARE(table->cellAt(2, 0).firstPosition(), 7);
    QCOMPARE(table->cellAt(2, 0).rowSpan(), 2);
    QCOMPARE(table->cellAt(2, 0).columnSpan(), 1);
    QCOMPARE(table->cellAt(2, 1).firstPosition(), 8);
    QCOMPARE(table->cellAt(2, 2).firstPosition(), 9);
    QCOMPARE(table->cellAt(3, 0).firstPosition(), 7);
    QCOMPARE(table->cellAt(3, 1).firstPosition(), 10);
    QCOMPARE(table->cellAt(3, 2).firstPosition(), 11);
}

void tst_BOBUIextTable::removeColumnsInTableWithMergedRows()
{
    BOBUIextTable *table = cursor.insertTable(3, 4);
    table->mergeCells(0, 0, 1, 4);
    QCOMPARE(table->rows(), 3);
    QCOMPARE(table->columns(), 4);

    table->removeColumns(0, table->columns() - 1);

    QCOMPARE(table->rows(), 3);
    QCOMPARE(table->columns(), 1);
}

#ifndef BOBUI_NO_WIDGETS
void tst_BOBUIextTable::BOBUIBUG11282_insertBeforeMergedEnding_data()
{
    BOBUIest::addColumn<int>("rows");
    BOBUIest::addColumn<int>("columns");
    BOBUIest::addColumn<QList<int> >("merge");
    BOBUIest::addColumn<QList<int> >("insert");

    BOBUIest::newRow("2x3, merge two, insert one") << 2 << 3 << (QList<int>() << 1 << 2 << 2)
            << (QList<int>() << 1 << 1) ;
    BOBUIest::newRow("3x4, merge three, insert one") << 3 << 4 << (QList<int>() << 1 << 3 << 3)
            << (QList<int>() << 1 << 1) ;
    BOBUIest::newRow("4x3, merge two, insert two") << 4 << 3 << (QList<int>() << 1 << 4 << 2)
            << (QList<int>() << 1 << 2) ;
    BOBUIest::newRow("4x4, merge middle two, insert one") << 4 << 4 << (QList<int>() << 1 << 4 << 2)
            << (QList<int>() << 1 << 1) ;
}

void tst_BOBUIextTable::BOBUIBUG11282_insertBeforeMergedEnding()
{
    QFETCH(int, rows);
    QFETCH(int, columns);
    QFETCH(QList<int>, merge);
    QFETCH(QList<int>, insert);
    BOBUIextTable *table = cursor.insertTable(rows, columns);
    BOBUIextEdit *textEdit = new BOBUIextEdit;
    textEdit->setDocument(doc);
    textEdit->show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(textEdit));
    table->mergeCells(0,merge.at(0), merge.at(1), merge.at(2));
    //Don't crash !
    table->insertColumns(insert.at(0), insert.at(1));
    //Check that the final size is what we expected
    QCOMPARE(table->rows(), rows);
    QCOMPARE(table->columns(), columns + insert.at(1));
    delete textEdit;
}
#endif

void tst_BOBUIextTable::BOBUIBUG22011_insertBeforeRowSpan()
{
    BOBUIextDocument doc;
    BOBUIextCursor cursor(&doc);
    BOBUIextTable *table = cursor.insertTable(1,1); // 1x1

    table->appendColumns(1); // 1x2
    table->appendRows(1); // 2x2
    table->mergeCells(0, 0, 2, 1); // 2x2
    table->insertColumns(1, 1); // 2x3
    table->mergeCells(0, 1, 1, 2); // 2x3
    table->appendRows(1); // 3x3
    table->mergeCells(0, 0, 3, 1); // 3x3
    table->appendRows(1); // 4x3
    table->insertColumns(1, 1); // 4x4
    table->mergeCells(0, 1, 1, 3);
    table->mergeCells(1, 1, 1, 2);
    table->mergeCells(2, 1, 1, 2);
    table->mergeCells(3, 0, 1, 2);
    table->insertColumns(3, 1); // 4x5
    table->mergeCells(0, 1, 1, 4);

    table->appendColumns(1); // 4x6

    QCOMPARE(table->rows(), 4);
    QCOMPARE(table->columns(), 6);
}

#if !defined(BOBUI_NO_PRINTER) && defined(BOBUI_BUILD_INTERNAL)
namespace {
class BOBUIBUG31330_PaintDevice : public QPagedPaintDevice
{
public:
    class PaintEngine : public QPaintEngine
    {
    public:
        QList<QRectF> rects;

        PaintEngine()
            : QPaintEngine(QPaintEngine::PaintEngineFeatures{ })
        {}
        virtual Type type() const override
        {
            return User;
        }
        virtual bool begin(QPaintDevice *) override
        {
            return true;
        }
        virtual bool end() override
        {
            return true;
        }
        virtual void updateState(const QPaintEngineState &) override
        {}
        virtual void drawRects(const QRect *, int) override
        {}
        virtual void drawRects(const QRectF *r, int) override
        {
            if (painter()->brush() == QBrush(BobUI::green))
            {
                rects.append(*r);
            }
        }
        virtual void drawPixmap(const QRectF &, const QPixmap &, const QRectF &) override
        {}
    };

    class QDummyPagedPaintDevicePrivate : public QPagedPaintDevicePrivate
    {
        bool setPageLayout(const QPageLayout &newPageLayout) override
        {
            m_pageLayout = newPageLayout;
            return m_pageLayout.isEquivalentTo(newPageLayout);
        }

        bool setPageSize(const QPageSize &pageSize) override
        {
            m_pageLayout.setPageSize(pageSize);
            return m_pageLayout.pageSize().isEquivalentTo(pageSize);
        }

        bool setPageOrientation(QPageLayout::Orientation orientation) override
        {
            m_pageLayout.setOrientation(orientation);
            return m_pageLayout.orientation() == orientation;
        }

        bool setPageMargins(const QMarginsF &margins, QPageLayout::Unit units) override
        {
            m_pageLayout.setUnits(units);
            m_pageLayout.setMargins(margins);
            return m_pageLayout.margins() == margins && m_pageLayout.units() == units;
        }

        QPageLayout pageLayout() const override
        {
            return m_pageLayout;
        }

        QPageLayout m_pageLayout;
    };

    int pages;
    QPaintEngine* engine;

    BOBUIBUG31330_PaintDevice(QPaintEngine* engine)
        : QPagedPaintDevice(new QDummyPagedPaintDevicePrivate), pages(1), engine(engine)
    {
        QPageLayout layout = pageLayout();
        layout.setUnits(QPageLayout::Point);
        setPageLayout(layout);
    }
    virtual int metric(PaintDeviceMetric metric) const override
    {
        if (PdmDevicePixelRatio == metric)
            return 1;
        if (PdmDevicePixelRatioScaled == metric)
            return 1 * QPaintDevice::devicePixelRatioFScale();
        if (PdmDpiY == metric)
            return 96;
        if (PdmDpiX == metric)
            return 96;
        if (PdmHeight == metric)
            return 1000;
        if (PdmWidth == metric)
            return 700;
        return 900;
    }
    virtual QPaintEngine *paintEngine() const override
    {
        return engine;
    }
    bool newPage() override
    {
        ++pages;
        return true;
    }
};
}

void tst_BOBUIextTable::BOBUIBUG31330_renderBackground()
{
    BOBUIextDocument doc;
    BOBUIextCursor cursor(&doc);
    BOBUIextTable* table = cursor.insertTable(4, 2);

    BOBUIextTableCell cell = table->cellAt(3, 0);

    BOBUIextCharFormat cellFormat = cell.format();
    cellFormat.setBackground(QBrush(BobUI::green));
    cell.setFormat(cellFormat);

    BOBUIextCursor tc = cell.firstCursorPosition();
    for (int i = 0; i < 100; ++i) {
        tc.insertBlock();
    }
    BOBUIBUG31330_PaintDevice::PaintEngine engine;
    BOBUIBUG31330_PaintDevice paintDevice(&engine);
    paintDevice.setPageSize(QPageSize(QPageSize::A4));
    doc.print(&paintDevice);

    QVERIFY(paintDevice.pages >= 2);
    QCOMPARE(engine.rects.size(), paintDevice.pages);
    for (int i = 0; i < engine.rects.size(); ++i) {
        QRectF rect = engine.rects[i];
        QVERIFY(rect.top() > 0);
        QVERIFY(rect.bottom() < 1000);
    }
}
#endif

void tst_BOBUIextTable::checkBorderAttributes_data()
{
    BOBUIest::addColumn<QString>("html");
    BOBUIest::addColumn<qreal>("topBorderWidth");
    BOBUIest::addColumn<qreal>("bottomBorderWidth");
    BOBUIest::addColumn<qreal>("leftBorderWidth");
    BOBUIest::addColumn<qreal>("rightBorderWidth");
    BOBUIest::addColumn<BOBUIextFrameFormat::BorderStyle>("topBorderStyle");
    BOBUIest::addColumn<BOBUIextFrameFormat::BorderStyle>("bottomBorderStyle");
    BOBUIest::addColumn<BOBUIextFrameFormat::BorderStyle>("leftBorderStyle");
    BOBUIest::addColumn<BOBUIextFrameFormat::BorderStyle>("rightBorderStyle");
    BOBUIest::addColumn<QBrush>("topBorderBrush");
    BOBUIest::addColumn<QBrush>("bottomBorderBrush");
    BOBUIest::addColumn<QBrush>("leftBorderBrush");
    BOBUIest::addColumn<QBrush>("rightBorderBrush");

    const QString tableHtmlStart = QStringLiteral("<html><head><style>");
    const QString tableHtmlEnd = QStringLiteral("</style></head><body>"
                                                "<table border=\"1\"><tr><td>One</td><td>Two</td></tr>"
                                                "<tr><td>Three</td><td>Four</td></tr></table></body></html>");
    BOBUIest::newRow("1px-solid-colors")
        << QString("%1"
                   "td {"
                   "border-top: 1px solid red;"
                   "border-bottom: 1px solid blue;"
                   "border-left: 1px solid green;"
                   "border-right: 1px solid yellow;"
                   "}"
                   "%2").arg(tableHtmlStart).arg(tableHtmlEnd)
        << 1.0 << 1.0 << 1.0 << 1.0
        << BOBUIextFrameFormat::BorderStyle_Solid << BOBUIextFrameFormat::BorderStyle_Solid
        << BOBUIextFrameFormat::BorderStyle_Solid << BOBUIextFrameFormat::BorderStyle_Solid
        << QBrush(BobUI::red) << QBrush(BobUI::blue) << QBrush(BobUI::darkGreen) << QBrush(BobUI::yellow);
    BOBUIest::newRow("MixedWidth-solid-colors")
        << QString("%1"
                   "td {"
                   "border-top: 1px solid red;"
                   "border-bottom: 2px solid blue;"
                   "border-left: 3px solid green;"
                   "border-right: 4px solid yellow;"
                   "}"
                   "%2").arg(tableHtmlStart).arg(tableHtmlEnd)
        << 1.0 << 2.0 << 3.0 << 4.0
        << BOBUIextFrameFormat::BorderStyle_Solid << BOBUIextFrameFormat::BorderStyle_Solid
        << BOBUIextFrameFormat::BorderStyle_Solid << BOBUIextFrameFormat::BorderStyle_Solid
        << QBrush(BobUI::red) << QBrush(BobUI::blue) << QBrush(BobUI::darkGreen) << QBrush(BobUI::yellow);
    BOBUIest::newRow("MixedWidth-MixedStyle-colors")
        << QString("%1"
                   "td {"
                   "border-top: 1px solid red;"
                   "border-bottom: 2px dotted blue;"
                   "border-left: 3px dashed green;"
                   "border-right: 4px inset yellow;"
                   "}"
                   "%2").arg(tableHtmlStart).arg(tableHtmlEnd)
        << 1.0 << 2.0 << 3.0 << 4.0
        << BOBUIextFrameFormat::BorderStyle_Solid << BOBUIextFrameFormat::BorderStyle_Dotted
        << BOBUIextFrameFormat::BorderStyle_Dashed << BOBUIextFrameFormat::BorderStyle_Inset
        << QBrush(BobUI::red) << QBrush(BobUI::blue) << QBrush(BobUI::darkGreen) << QBrush(BobUI::yellow);
}

void tst_BOBUIextTable::checkBorderAttributes()
{
    QFETCH(QString, html);
    QFETCH(qreal, topBorderWidth);
    QFETCH(qreal, bottomBorderWidth);
    QFETCH(qreal, leftBorderWidth);
    QFETCH(qreal, rightBorderWidth);
    QFETCH(BOBUIextFrameFormat::BorderStyle, topBorderStyle);
    QFETCH(BOBUIextFrameFormat::BorderStyle, bottomBorderStyle);
    QFETCH(BOBUIextFrameFormat::BorderStyle, leftBorderStyle);
    QFETCH(BOBUIextFrameFormat::BorderStyle, rightBorderStyle);
    QFETCH(QBrush, topBorderBrush);
    QFETCH(QBrush, bottomBorderBrush);
    QFETCH(QBrush, leftBorderBrush);
    QFETCH(QBrush, rightBorderBrush);

#ifndef BOBUI_NO_TEXTHTMLPARSER
    BOBUIextDocument doc;
    doc.setHtml(html);
    BOBUIextCursor cursor(doc.firstBlock());
    cursor.movePosition(BOBUIextCursor::Right);

    BOBUIextTable *currentTable = cursor.currentTable();
    QVERIFY(currentTable);
    for (int row = 0; row < 2; row++) {
        for (int column = 0; column < 2; column++) {
            BOBUIextTableCell cell = currentTable->cellAt(row, column);
            BOBUIextCharFormat cellFormat = cell.format();
            QCOMPARE(cellFormat.doubleProperty(BOBUIextFormat::TableCellTopBorder), topBorderWidth);
            QCOMPARE(cellFormat.doubleProperty(BOBUIextFormat::TableCellBottomBorder), bottomBorderWidth);
            QCOMPARE(cellFormat.doubleProperty(BOBUIextFormat::TableCellLeftBorder), leftBorderWidth);
            QCOMPARE(cellFormat.doubleProperty(BOBUIextFormat::TableCellRightBorder), rightBorderWidth);
            QCOMPARE(cellFormat.property(BOBUIextFormat::TableCellTopBorderStyle), topBorderStyle);
            QCOMPARE(cellFormat.property(BOBUIextFormat::TableCellBottomBorderStyle), bottomBorderStyle);
            QCOMPARE(cellFormat.property(BOBUIextFormat::TableCellLeftBorderStyle), leftBorderStyle);
            QCOMPARE(cellFormat.property(BOBUIextFormat::TableCellRightBorderStyle), rightBorderStyle);
            QCOMPARE(cellFormat.brushProperty(BOBUIextFormat::TableCellTopBorderBrush), topBorderBrush);
            QCOMPARE(cellFormat.brushProperty(BOBUIextFormat::TableCellBottomBorderBrush), bottomBorderBrush);
            QCOMPARE(cellFormat.brushProperty(BOBUIextFormat::TableCellLeftBorderBrush), leftBorderBrush);
            QCOMPARE(cellFormat.brushProperty(BOBUIextFormat::TableCellRightBorderBrush), rightBorderBrush);
        }
    }
#endif
}

void tst_BOBUIextTable::checkTableBorderAttributes_data()
{
    BOBUIest::addColumn<QString>("html");
    BOBUIest::addColumn<qreal>("tableBorderWidth");
    BOBUIest::addColumn<BOBUIextFrameFormat::BorderStyle>("tableBorderStyle");
    BOBUIest::addColumn<QBrush>("tableBorderBrush");

    const QString tableHtmlStart = QStringLiteral("<html><head><style>");
    const QString tableHtmlEnd1 = QStringLiteral("</style></head><body>"
                                                "<table><tr><td>One</td><td>Two</td></tr></table>"
                                                "</body></html>");
    const QString tableHtmlEnd2 = QStringLiteral("</style></head><body>"
                                                "<table border=10><tr><td>One</td><td>Two</td></tr></table>"
                                                "</body></html>");

    BOBUIest::newRow("table-border-attributes-shorthand")
            << QString("%1"
                       "table {"
                            "border: 2px solid red;"
                       "}"
                       "%2").arg(tableHtmlStart).arg(tableHtmlEnd1)
            << 2.0 << BOBUIextFrameFormat::BorderStyle_Solid << QBrush(BobUI::red);

    BOBUIest::newRow("table-border-attributes-explicit")
            << QString("%1"
                       "table {"
                            "border-width: 2px;"
                            "border-color: red;"
                            "border-style: dashed;"
                       "}"
                       "%2").arg(tableHtmlStart).arg(tableHtmlEnd1)
            << 2.0 << BOBUIextFrameFormat::BorderStyle_Dashed << QBrush(BobUI::red);

    BOBUIest::newRow("table-border-override")
            << QString("%1"
                       "table {"
                          "border: 2px solid red;"
                       "}"
                       "%2").arg(tableHtmlStart).arg(tableHtmlEnd2)
            << 2.0 << BOBUIextFrameFormat::BorderStyle_Solid << QBrush(BobUI::red);

    BOBUIest::newRow("table-border-default")
            << QString("%1"
                       "%2").arg(tableHtmlStart).arg(tableHtmlEnd2)
            << 10.0 << BOBUIextFrameFormat::BorderStyle_Outset << QBrush(BobUI::darkGray);
}

void tst_BOBUIextTable::checkTableBorderAttributes()
{
    QFETCH(QString, html);
    QFETCH(qreal, tableBorderWidth);
    QFETCH(BOBUIextFrameFormat::BorderStyle, tableBorderStyle);
    QFETCH(QBrush, tableBorderBrush);

#ifndef BOBUI_NO_TEXTHTMLPARSER
    BOBUIextDocument doc;
    doc.setHtml(html);
    BOBUIextCursor cursor(doc.firstBlock());
    cursor.movePosition(BOBUIextCursor::Right);

    BOBUIextTable *currentTable = cursor.currentTable();
    QVERIFY(currentTable);
    QCOMPARE(currentTable->format().border(), tableBorderWidth);
    QCOMPARE(currentTable->format().borderStyle(), tableBorderStyle);
    QCOMPARE(currentTable->format().borderBrush(), tableBorderBrush);
#endif
}

#ifndef BOBUI_NO_WIDGETS
void tst_BOBUIextTable::columnWidthWithSpans()
{
    cleanup();
    init();
    BOBUIextTable *table = cursor.insertTable(4, 4);
    BOBUIextEdit textEdit;
    textEdit.setDocument(doc);
    textEdit.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&textEdit));

    for (int i = 0; i < table->columns(); ++i)
        table->cellAt(0, i).firstCursorPosition().insertText(QString("Header %1").arg(i));

    BOBUIextBlock block = table->cellAt(0, 0).firstCursorPosition().block();
    const QRectF beforeRect = table->document()->documentLayout()->blockBoundingRect(block);
    table->mergeCells(1, 0, 1, table->columns());
    block = table->cellAt(0, 0).firstCursorPosition().block();
    const QRectF afterRect = table->document()->documentLayout()->blockBoundingRect(block);
    QCOMPARE(afterRect, beforeRect);
}

void tst_BOBUIextTable::columnWidthWithImage_data()
{
    const auto imageHtml = [](int width, int height) {
        QImage image(width, height, QImage::Format_RGB32);
        image.fill(BobUI::red);
        QByteArray imageBytes;
        QBuffer buffer(&imageBytes);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "png");
        return QString("<td><img src='data:image/png;base64,%1'/></td>").arg(imageBytes.toBase64());
    };

    BOBUIest::addColumn<QString>("leftHtml");
    BOBUIest::addColumn<QString>("rightHtml");
    BOBUIest::addColumn<QSize>("imageSize");
    BOBUIest::addRow("image")
        << imageHtml(500, 32) << "<td></td>" << QSize(500, 32);
    BOBUIest::addRow("image, text")
        << imageHtml(32, 32) << "<td>abc</td>" << QSize(32, 32);
    BOBUIest::addRow("image, 100%% text")
        << imageHtml(32, 32) << "<td style='background-color: grey' width='100%'>abc</td>"
        << QSize(32, 32);
    BOBUIest::addRow("image, image")
        << imageHtml(256, 32) << imageHtml(256, 32) << QSize(256, 32);
}

void tst_BOBUIextTable::columnWidthWithImage()
{
    const QString tableTemplate = "<table><tr>%1 %2</tr></table>";

    QFETCH(QString, leftHtml);
    QFETCH(QString, rightHtml);
    QFETCH(QSize, imageSize);

#ifndef BOBUI_NO_TEXTHTMLPARSER
    BOBUIextDocument doc;
    doc.setHtml(tableTemplate.arg(leftHtml).arg(rightHtml));
    BOBUIextEdit textEdit;
    textEdit.setDocument(&doc);
    textEdit.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&textEdit));

    BOBUIextCursor cursor(doc.firstBlock());
    cursor.movePosition(BOBUIextCursor::Right);

    BOBUIextTable *currentTable = cursor.currentTable();
    QVERIFY(currentTable);

    BOBUIextBlock block = currentTable->cellAt(0, 0).firstCursorPosition().block();
    const QRectF leftRect = currentTable->document()->documentLayout()->blockBoundingRect(block);
    block = currentTable->cellAt(0, 1).firstCursorPosition().block();
    const QRectF rightRect = currentTable->document()->documentLayout()->blockBoundingRect(block);
    QCOMPARE(leftRect.size().toSize(), imageSize);
    QVERIFY(rightRect.left() > leftRect.right());
#endif
}

void tst_BOBUIextTable::BOBUIBUG138678_insertColumnAtStartWithRowspan()
{
    QString s =
"<html><body>"
"<table border=1>"
"<tr>"
"    <td />"
"    <td />"
"    <td />"
"    <td />"
"    <td />"
"</tr>"
"<tr>"
"    <td rowspan=2 />"
"    <td rowspan=2 />"
"    <td />"
"    <td />"
"    <td rowspan=2 />"
"</tr>"
"<tr>"
"    <td />"
"    <td />"
"</tr>"
"</table></body></html>)";

    BOBUIextDocument doc;
    doc.setHtml(s);

    BOBUIextEdit textEdit;
    textEdit.setDocument(&doc);
    textEdit.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&textEdit));

    BOBUIextCursor cursor(doc.firstBlock());
    cursor.movePosition(BOBUIextCursor::Right);

    BOBUIextTable *currentTable = cursor.currentTable();
    QVERIFY(currentTable);

    // Don't assert
    currentTable->insertColumns(0, 1);
}
#endif


BOBUIEST_MAIN(tst_BOBUIextTable)
#include "tst_bobuiexttable.moc"
