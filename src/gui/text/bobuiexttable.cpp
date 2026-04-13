// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiexttable.h"
#include "bobuiextcursor.h"
#include "bobuiextformat.h"
#include <qdebug.h>
#include "bobuiextcursor_p.h"
#include "bobuiexttable_p.h"
#include "qvarlengtharray.h"

#include <algorithm>
#include <stdlib.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

/*!
    \class BOBUIextTableCell
    \reentrant

    \brief The BOBUIextTableCell class represents the properties of a
    cell in a BOBUIextTable.
    \inmodule BobUIGui

    \ingroup richtext-processing

    Table cells are pieces of document structure that belong to a table.
    The table orders cells into particular rows and columns; cells can
    also span multiple columns and rows.

    Cells are usually created when a table is inserted into a document with
    BOBUIextCursor::insertTable(), but they are also created and destroyed when
    a table is resized.

    Cells contain information about their location in a table; you can
    obtain the row() and column() numbers of a cell, and its rowSpan()
    and columnSpan().

    The format() of a cell describes the default character format of its
    contents. The firstCursorPosition() and lastCursorPosition() functions
    are used to obtain the extent of the cell in the document.

    \sa BOBUIextTable, BOBUIextTableFormat
*/

/*!
    \fn BOBUIextTableCell::BOBUIextTableCell()

    Constructs an invalid table cell.

    \sa isValid()
*/

/*!
    \fn BOBUIextTableCell::BOBUIextTableCell(const BOBUIextTableCell &other)

    Copy constructor. Creates a new BOBUIextTableCell object based on the
    \a other cell.
*/

/*!
    \fn BOBUIextTableCell& BOBUIextTableCell::operator=(const BOBUIextTableCell &other)

    Assigns the \a other table cell to this table cell.
*/

/*!
    \since 4.2

    Sets the cell's character format to \a format. This can for example be used to change
    the background color of the entire cell:

    \code
    BOBUIextTableCell cell = table->cellAt(2, 3);
    BOBUIextCharFormat format = cell.format();
    format.setBackground(BobUI::blue);
    cell.setFormat(format);
    \endcode

    Note that the cell's row or column span cannot be changed through this function. You have
    to use BOBUIextTable::mergeCells and BOBUIextTable::splitCell instead.

    \sa format()
*/
void BOBUIextTableCell::setFormat(const BOBUIextCharFormat &format)
{
    BOBUIextCharFormat fmt = format;
    fmt.clearProperty(BOBUIextFormat::ObjectIndex);
    fmt.setObjectType(BOBUIextFormat::TableCellObject);
    BOBUIextDocumentPrivate *p = const_cast<BOBUIextDocumentPrivate *>(BOBUIextDocumentPrivate::get(table));
    BOBUIextDocumentPrivate::FragmentIterator frag(&p->fragmentMap(), fragment);

    BOBUIextFormatCollection *c = p->formatCollection();
    BOBUIextCharFormat oldFormat = c->charFormat(frag->format);
    fmt.setTableCellRowSpan(oldFormat.tableCellRowSpan());
    fmt.setTableCellColumnSpan(oldFormat.tableCellColumnSpan());

    p->setCharFormat(frag.position(), 1, fmt, BOBUIextDocumentPrivate::SetFormatAndPreserveObjectIndices);
}

/*!
    Returns the cell's character format.
*/
BOBUIextCharFormat BOBUIextTableCell::format() const
{
    const BOBUIextDocumentPrivate *p = BOBUIextDocumentPrivate::get(table);
    const BOBUIextFormatCollection *c = p->formatCollection();

    BOBUIextCharFormat fmt = c->charFormat(tableCellFormatIndex());
    fmt.setObjectType(BOBUIextFormat::TableCellObject);
    return fmt;
}

/*!
    \since 4.5

    Returns the index of the tableCell's format in the document's internal list of formats.

    \sa BOBUIextDocument::allFormats()
*/
int BOBUIextTableCell::tableCellFormatIndex() const
{
    const BOBUIextDocumentPrivate *p = BOBUIextDocumentPrivate::get(table);
    return BOBUIextDocumentPrivate::FragmentIterator(&p->fragmentMap(), fragment)->format;
}

/*!
    Returns the number of the row in the table that contains this cell.

    \sa column()
*/
int BOBUIextTableCell::row() const
{
    const BOBUIextTablePrivate *tp = table->d_func();
    if (tp->dirty)
        tp->update();

    int idx = tp->findCellIndex(fragment);
    if (idx == -1)
        return idx;
    return tp->cellIndices.at(idx) / tp->nCols;
}

/*!
    Returns the number of the column in the table that contains this cell.

    \sa row()
*/
int BOBUIextTableCell::column() const
{
    const BOBUIextTablePrivate *tp = table->d_func();
    if (tp->dirty)
        tp->update();

    int idx = tp->findCellIndex(fragment);
    if (idx == -1)
        return idx;
    return tp->cellIndices.at(idx) % tp->nCols;
}

/*!
    Returns the number of rows this cell spans. The default is 1.

    \sa columnSpan()
*/
int BOBUIextTableCell::rowSpan() const
{
    return format().tableCellRowSpan();
}

/*!
    Returns the number of columns this cell spans. The default is 1.

    \sa rowSpan()
*/
int BOBUIextTableCell::columnSpan() const
{
    return format().tableCellColumnSpan();
}

/*!
    \fn bool BOBUIextTableCell::isValid() const

    Returns \c true if this is a valid table cell; otherwise returns
    false.
*/


/*!
    Returns the first valid cursor position in this cell.

    \sa lastCursorPosition()
*/
BOBUIextCursor BOBUIextTableCell::firstCursorPosition() const
{
    return BOBUIextCursorPrivate::fromPosition(table->d_func()->pieceTable, firstPosition());
}

/*!
    Returns the last valid cursor position in this cell.

    \sa firstCursorPosition()
*/
BOBUIextCursor BOBUIextTableCell::lastCursorPosition() const
{
    return BOBUIextCursorPrivate::fromPosition(table->d_func()->pieceTable, lastPosition());
}


/*!
    \internal

    Returns the first valid position in the document occupied by this cell.
*/
int BOBUIextTableCell::firstPosition() const
{
    const BOBUIextDocumentPrivate *p = BOBUIextDocumentPrivate::get(table);
    return p->fragmentMap().position(fragment) + 1;
}

/*!
    \internal

    Returns the last valid position in the document occupied by this cell.
*/
int BOBUIextTableCell::lastPosition() const
{
    const BOBUIextDocumentPrivate *p = BOBUIextDocumentPrivate::get(table);
    const BOBUIextTablePrivate *td = table->d_func();
    int index = table->d_func()->findCellIndex(fragment);
    int f;
    if (index != -1)
        f = td->cells.value(index + 1, td->fragment_end);
    else
        f = td->fragment_end;
    return p->fragmentMap().position(f);
}


/*!
    Returns a frame iterator pointing to the beginning of the table's cell.

    \sa end()
*/
BOBUIextFrame::iterator BOBUIextTableCell::begin() const
{
    const BOBUIextDocumentPrivate *p = BOBUIextDocumentPrivate::get(table);
    int b = p->blockMap().findNode(firstPosition());
    int e = p->blockMap().findNode(lastPosition()+1);
    return BOBUIextFrame::iterator(const_cast<BOBUIextTable *>(table), b, b, e);
}

/*!
    Returns a frame iterator pointing to the end of the table's cell.

    \sa begin()
*/
BOBUIextFrame::iterator BOBUIextTableCell::end() const
{
    const BOBUIextDocumentPrivate *p = BOBUIextDocumentPrivate::get(table);
    int b = p->blockMap().findNode(firstPosition());
    int e = p->blockMap().findNode(lastPosition()+1);
    return BOBUIextFrame::iterator(const_cast<BOBUIextTable *>(table), e, b, e);
}


/*!
    \fn BOBUIextCursor BOBUIextTableCell::operator==(const BOBUIextTableCell &other) const

    Returns \c true if this cell object and the \a other cell object
    describe the same cell; otherwise returns \c false.
*/

/*!
    \fn BOBUIextCursor BOBUIextTableCell::operator!=(const BOBUIextTableCell &other) const

    Returns \c true if this cell object and the \a other cell object
    describe different cells; otherwise returns \c false.
*/

/*!
    \fn BOBUIextTableCell::~BOBUIextTableCell()

    Destroys the table cell.
*/

BOBUIextTable *BOBUIextTablePrivate::createTable(BOBUIextDocumentPrivate *pieceTable, int pos, int rows, int cols, const BOBUIextTableFormat &tableFormat)
{
    BOBUIextTableFormat fmt = tableFormat;
    fmt.setColumns(cols);
    BOBUIextTable *table = qobject_cast<BOBUIextTable *>(pieceTable->createObject(fmt));
    Q_ASSERT(table);

    pieceTable->beginEditBlock();

//     qDebug("---> createTable: rows=%d, cols=%d at %d", rows, cols, pos);
    // add block after table
    BOBUIextCharFormat charFmt;
    charFmt.setObjectIndex(table->objectIndex());
    charFmt.setObjectType(BOBUIextFormat::TableCellObject);


    int charIdx = pieceTable->formatCollection()->indexForFormat(charFmt);
    int cellIdx = pieceTable->formatCollection()->indexForFormat(BOBUIextBlockFormat());

    BOBUIextTablePrivate *d = table->d_func();
    d->blockFragmentUpdates = true;

    d->fragment_start = pieceTable->insertBlock(BOBUIextBeginningOfFrame, pos, cellIdx, charIdx);
    d->cells.append(d->fragment_start);
    ++pos;

    for (int i = 1; i < rows*cols; ++i) {
        d->cells.append(pieceTable->insertBlock(BOBUIextBeginningOfFrame, pos, cellIdx, charIdx));
//      qDebug("      addCell at %d", pos);
        ++pos;
    }

    d->fragment_end = pieceTable->insertBlock(BOBUIextEndOfFrame, pos, cellIdx, charIdx);
//  qDebug("      addEOR at %d", pos);
    ++pos;

    d->blockFragmentUpdates = false;
    d->dirty = true;

    pieceTable->endEditBlock();

    return table;
}

struct QFragmentFindHelper
{
    inline QFragmentFindHelper(int _pos, const BOBUIextDocumentPrivate::FragmentMap &map)
        : pos(_pos), fragmentMap(map) {}
    uint pos;
    const BOBUIextDocumentPrivate::FragmentMap &fragmentMap;
};

static inline bool operator<(int fragment, const QFragmentFindHelper &helper)
{
    return helper.fragmentMap.position(fragment) < helper.pos;
}

static inline bool operator<(const QFragmentFindHelper &helper, int fragment)
{
    return helper.pos < helper.fragmentMap.position(fragment);
}

int BOBUIextTablePrivate::findCellIndex(int fragment) const
{
    QFragmentFindHelper helper(pieceTable->fragmentMap().position(fragment),
                              pieceTable->fragmentMap());
    const auto it = std::lower_bound(cells.constBegin(), cells.constEnd(), helper);
    if ((it == cells.constEnd()) || (helper < *it))
        return -1;
    return it - cells.constBegin();
}

void BOBUIextTablePrivate::fragmentAdded(QChar type, uint fragment)
{
    dirty = true;
    if (blockFragmentUpdates)
        return;
    if (type == BOBUIextBeginningOfFrame) {
        Q_ASSERT(cells.indexOf(int(fragment)) == -1);
        const uint pos = pieceTable->fragmentMap().position(fragment);
        QFragmentFindHelper helper(pos, pieceTable->fragmentMap());
        auto it = std::lower_bound(cells.begin(), cells.end(), helper);
        cells.insert(it, fragment);
        if (!fragment_start || pos < pieceTable->fragmentMap().position(fragment_start))
            fragment_start = fragment;
        return;
    }
    BOBUIextFramePrivate::fragmentAdded(type, fragment);
}

void BOBUIextTablePrivate::fragmentRemoved(QChar type, uint fragment)
{
    dirty = true;
    if (blockFragmentUpdates)
        return;
    if (type == BOBUIextBeginningOfFrame) {
        Q_ASSERT(cells.indexOf(int(fragment)) != -1);
        cells.removeAll(int(fragment));
        if (fragment_start == fragment && cells.size()) {
            fragment_start = cells.at(0);
        }
        if (fragment_start != fragment)
            return;
    }
    BOBUIextFramePrivate::fragmentRemoved(type, fragment);
}

/*!
    \internal
    This function is usually called when the table is "dirty".
    It seems to update all kind of table information.
*/
void BOBUIextTablePrivate::update() const
{
    Q_Q(const BOBUIextTable);
    nCols = q->format().columns();
    nRows = (cells.size() + nCols-1)/nCols;
//     qDebug(">>>> BOBUIextTablePrivate::update, nRows=%d, nCols=%d", nRows, nCols);

    grid.assign(nRows * nCols, 0);

    BOBUIextDocumentPrivate *p = pieceTable;
    BOBUIextFormatCollection *c = p->formatCollection();

    cellIndices.resize(cells.size());

    int cell = 0;
    for (int i = 0; i < cells.size(); ++i) {
        int fragment = cells.at(i);
        BOBUIextCharFormat fmt = c->charFormat(BOBUIextDocumentPrivate::FragmentIterator(&p->fragmentMap(), fragment)->format);
        int rowspan = fmt.tableCellRowSpan();
        int colspan = fmt.tableCellColumnSpan();

        // skip taken cells
        while (cell < nRows*nCols && grid[cell])
            ++cell;

        int r = cell/nCols;
        int c = cell%nCols;
        cellIndices[i] = cell;

        if (r + rowspan > nRows) {
            grid.resize((r + rowspan) * nCols, 0);
            nRows = r + rowspan;
        }

        Q_ASSERT(c + colspan <= nCols);
        for (int ii = 0; ii < rowspan; ++ii) {
            for (int jj = 0; jj < colspan; ++jj) {
                Q_ASSERT(grid[(r+ii)*nCols + c+jj] == 0);
                grid[(r+ii)*nCols + c+jj] = fragment;
//              qDebug("    setting cell %d span=%d/%d at %d/%d", fragment, rowspan, colspan, r+ii, c+jj);
            }
        }
    }
//     qDebug("<<<< end: nRows=%d, nCols=%d", nRows, nCols);

    dirty = false;
}





/*!
    \class BOBUIextTable
    \reentrant

    \brief The BOBUIextTable class represents a table in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing

    A table is a group of cells ordered into rows and columns. Each table
    contains at least one row and one column. Each cell contains a block, and
    is surrounded by a frame.

    Tables are usually created and inserted into a document with the
    BOBUIextCursor::insertTable() function.
    For example, we can insert a table with three rows and two columns at the
    current cursor position in an editor using the following lines of code:

    \snippet textdocument-tables/mainwindow.cpp 1
    \codeline
    \snippet textdocument-tables/mainwindow.cpp 3

    The table format is either defined when the table is created or changed
    later with setFormat().

    The table currently being edited by the cursor is found with
    BOBUIextCursor::currentTable(). This allows its format or dimensions to be
    changed after it has been inserted into a document.

    A table's size can be changed with resize(), or by using
    insertRows(), insertColumns(), removeRows(), or removeColumns().
    Use cellAt() to retrieve table cells.

    The starting and ending positions of table rows can be found by moving
    a cursor within a table, and using the rowStart() and rowEnd() functions
    to obtain cursors at the start and end of each row.

    Rows and columns within a BOBUIextTable can be merged and split using
    the mergeCells() and splitCell() functions. However, only cells that span multiple
    rows or columns can be split. (Merging or splitting does not increase or decrease
    the number of rows and columns.)

    Note that if you have merged multiple columns and rows into one cell, you will not
    be able to split the merged cell into new cells spanning over more than one row
    or column. To be able to split cells spanning over several rows and columns you
    need to do this over several iterations.

    \table 80%
    \row
        \li \inlineimage texttable-split.png Original Table
        \li Suppose we have a 2x3 table of names and addresses. To merge both
        columns in the first row we invoke mergeCells() with \a row = 0,
        \a column = 0, \a numRows = 1 and \a numColumns = 2.
        \snippet textdocument-texttable/main.cpp 0

    \row
        \li \inlineimage texttable-merge.png
        \li  This gives us the following table. To split the first row of the table
        back into two cells, we invoke the splitCell() function with \a numRows
        and \a numCols = 1.
        \snippet textdocument-texttable/main.cpp 1

    \row
        \li \inlineimage texttable-split.png Split Table
        \li This results in the original table.
    \endtable

    \sa BOBUIextTableFormat
*/

/*! \internal
 */
BOBUIextTable::BOBUIextTable(BOBUIextDocument *doc)
    : BOBUIextFrame(*new BOBUIextTablePrivate(doc), doc)
{
}

/*! \internal

Destroys the table.
 */
BOBUIextTable::~BOBUIextTable()
{
}


/*!
    \fn BOBUIextTableCell BOBUIextTable::cellAt(int row, int column) const

    Returns the table cell at the given \a row and \a column in the table.

    \sa columns(), rows()
*/
BOBUIextTableCell BOBUIextTable::cellAt(int row, int col) const
{
    Q_D(const BOBUIextTable);
    if (d->dirty)
        d->update();

    if (row < 0 || row >= d->nRows || col < 0 || col >= d->nCols)
        return BOBUIextTableCell();

    return BOBUIextTableCell(this, d->grid[row*d->nCols + col]);
}

/*!
    \overload

    Returns the table cell that contains the character at the given \a position
    in the document.
*/
BOBUIextTableCell BOBUIextTable::cellAt(int position) const
{
    Q_D(const BOBUIextTable);
    if (d->dirty)
        d->update();

    uint pos = (uint)position;
    const BOBUIextDocumentPrivate::FragmentMap &map = d->pieceTable->fragmentMap();
    if (position < 0 || map.position(d->fragment_start) >= pos || map.position(d->fragment_end) < pos)
        return BOBUIextTableCell();

    QFragmentFindHelper helper(position, map);
    auto it = std::lower_bound(d->cells.begin(), d->cells.end(), helper);
    if (it != d->cells.begin())
        --it;

    return BOBUIextTableCell(this, *it);
}

/*!
    \fn BOBUIextTableCell BOBUIextTable::cellAt(const BOBUIextCursor &cursor) const

    \overload

    Returns the table cell containing the given \a cursor.
*/
BOBUIextTableCell BOBUIextTable::cellAt(const BOBUIextCursor &c) const
{
    return cellAt(c.position());
}

/*!
    \fn void BOBUIextTable::resize(int rows, int columns)

    Resizes the table to contain the required number of \a rows and \a columns.

    \sa insertRows(), insertColumns(), removeRows(), removeColumns()
*/
void BOBUIextTable::resize(int rows, int cols)
{
    Q_D(BOBUIextTable);
    if (d->dirty)
        d->update();

    int nRows = this->rows();
    int nCols = this->columns();

    if (rows == nRows && cols == nCols)
        return;

    d->pieceTable->beginEditBlock();

    if (nCols < cols)
        insertColumns(nCols, cols - nCols);
    else if (nCols > cols)
        removeColumns(cols, nCols - cols);

    if (nRows < rows)
        insertRows(nRows, rows-nRows);
    else if (nRows > rows)
        removeRows(rows, nRows-rows);

    d->pieceTable->endEditBlock();
}

/*!
    \fn void BOBUIextTable::insertRows(int index, int rows)

    Inserts a number of \a rows before the row with the specified \a index.

    \sa resize(), insertColumns(), removeRows(), removeColumns(), appendRows(), appendColumns()
*/
void BOBUIextTable::insertRows(int pos, int num)
{
    Q_D(BOBUIextTable);
    if (num <= 0)
        return;

    if (d->dirty)
        d->update();

    if (pos > d->nRows || pos < 0)
        pos = d->nRows;

//     qDebug() << "-------- insertRows" << pos << num;
    BOBUIextDocumentPrivate *p = d->pieceTable;
    BOBUIextFormatCollection *c = p->formatCollection();
    p->beginEditBlock();

    int extended = 0;
    int insert_before = 0;
    if (pos > 0 && pos < d->nRows) {
        int lastCell = -1;
        for (int i = 0; i < d->nCols; ++i) {
            int cell = d->grid[pos*d->nCols + i];
            if (cell == d->grid[(pos-1)*d->nCols+i]) {
                // cell spans the insertion place, extend it
                if (cell != lastCell) {
                    BOBUIextDocumentPrivate::FragmentIterator it(&p->fragmentMap(), cell);
                    BOBUIextCharFormat fmt = c->charFormat(it->format);
                    fmt.setTableCellRowSpan(fmt.tableCellRowSpan() + num);
                    p->setCharFormat(it.position(), 1, fmt);
                }
                extended++;
            } else if (!insert_before) {
                insert_before = cell;
            }
            lastCell = cell;
        }
    } else {
        insert_before = (pos == 0 ? d->grid[0] : d->fragment_end);
    }
    if (extended < d->nCols) {
        Q_ASSERT(insert_before);
        BOBUIextDocumentPrivate::FragmentIterator it(&p->fragmentMap(), insert_before);
        BOBUIextCharFormat fmt = c->charFormat(it->format);
        fmt.setTableCellRowSpan(1);
        fmt.setTableCellColumnSpan(1);
        Q_ASSERT(fmt.objectIndex() == objectIndex());
        int pos = it.position();
        int cfmt = p->formatCollection()->indexForFormat(fmt);
        int bfmt = p->formatCollection()->indexForFormat(BOBUIextBlockFormat());
//         qDebug("inserting %d cells, nCols=%d extended=%d", num*(d->nCols-extended), d->nCols, extended);
        for (int i = 0; i < num*(d->nCols-extended); ++i)
            p->insertBlock(BOBUIextBeginningOfFrame, pos, bfmt, cfmt, BOBUIextUndoCommand::MoveCursor);
    }

//     qDebug() << "-------- end insertRows" << pos << num;
    p->endEditBlock();
}

/*!
    \fn void BOBUIextTable::insertColumns(int index, int columns)

    Inserts a number of \a columns before the column with the specified \a index.

    \sa insertRows(), resize(), removeRows(), removeColumns(), appendRows(), appendColumns()
*/
void BOBUIextTable::insertColumns(int pos, int num)
{
    Q_D(BOBUIextTable);
    if (num <= 0)
        return;

    if (d->dirty)
        d->update();

    if (pos > d->nCols || pos < 0)
        pos = d->nCols;

//     qDebug() << "-------- insertCols" << pos << num;
    BOBUIextDocumentPrivate *p = d->pieceTable;
    BOBUIextFormatCollection *c = p->formatCollection();
    p->beginEditBlock();

    QList<int> extendedSpans;
    for (int i = 0; i < d->nRows; ++i) {
        int cell;
        if (i == d->nRows - 1 && pos == d->nCols) {
            cell = d->fragment_end;
        } else {
            int logicalGridIndexBeforePosition = pos > 0 || i > 0
                                                 ? d->findCellIndex(d->grid[i*d->nCols + pos - 1])
                                                 : -1;

            // Search for the logical insertion point by skipping past cells which are not the first
            // cell in a rowspan. This means any cell for which the logical grid index is
            // less than the logical cell index of the cell before the insertion.
            int logicalGridIndex;
            int gridArrayOffset = i*d->nCols + pos;
            do {
                cell = d->grid[gridArrayOffset];
                logicalGridIndex = d->findCellIndex(cell);
                gridArrayOffset++;
            } while (logicalGridIndex < logicalGridIndexBeforePosition
                     && gridArrayOffset < d->nRows*d->nCols);

            if (logicalGridIndex < logicalGridIndexBeforePosition
                && gridArrayOffset == d->nRows*d->nCols)
                cell = d->fragment_end;
        }

        if (pos > 0 && pos < d->nCols && cell == d->grid[i*d->nCols + pos - 1]) {
            // cell spans the insertion place, extend it
            if (!extendedSpans.contains(cell)) {
                BOBUIextDocumentPrivate::FragmentIterator it(&p->fragmentMap(), cell);
                BOBUIextCharFormat fmt = c->charFormat(it->format);
                fmt.setTableCellColumnSpan(fmt.tableCellColumnSpan() + num);
                p->setCharFormat(it.position(), 1, fmt);
                d->dirty = true;
                extendedSpans << cell;
            }
        } else {
            /* If the next cell is spanned from the row above, we need to find the right position
            to insert to */
            if (i > 0 && pos < d->nCols && cell == d->grid[(i-1) * d->nCols + pos]) {
                int gridIndex = i*d->nCols + pos;
                const int gridEnd = d->nRows * d->nCols - 1;
                while (gridIndex < gridEnd && cell == d->grid[gridIndex]) {
                    ++gridIndex;
                }
                if (gridIndex == gridEnd)
                    cell = d->fragment_end;
                else
                    cell = d->grid[gridIndex];
            }
            BOBUIextDocumentPrivate::FragmentIterator it(&p->fragmentMap(), cell);
            BOBUIextCharFormat fmt = c->charFormat(it->format);
            fmt.setTableCellRowSpan(1);
            fmt.setTableCellColumnSpan(1);
            Q_ASSERT(fmt.objectIndex() == objectIndex());
            int position = it.position();
            int cfmt = p->formatCollection()->indexForFormat(fmt);
            int bfmt = p->formatCollection()->indexForFormat(BOBUIextBlockFormat());
            for (int i = 0; i < num; ++i)
                p->insertBlock(BOBUIextBeginningOfFrame, position, bfmt, cfmt, BOBUIextUndoCommand::MoveCursor);
        }
    }

    BOBUIextTableFormat tfmt = format();
    tfmt.setColumns(tfmt.columns()+num);
    QList<BOBUIextLength> columnWidths = tfmt.columnWidthConstraints();
    if (! columnWidths.isEmpty()) {
        for (int i = num; i > 0; --i)
            columnWidths.insert(pos, columnWidths.at(qMax(0, pos - 1)));
    }
    tfmt.setColumnWidthConstraints (columnWidths);
    BOBUIextObject::setFormat(tfmt);

//     qDebug() << "-------- end insertCols" << pos << num;
    p->endEditBlock();
}

/*!
    \since 4.5
    Appends \a count rows at the bottom of the table.

    \sa insertColumns(), insertRows(), resize(), removeRows(), removeColumns(), appendColumns()
*/
void BOBUIextTable::appendRows(int count)
{
    insertRows(rows(), count);
}

/*!
    \since 4.5
    Appends \a count columns at the right side of the table.

    \sa insertColumns(), insertRows(), resize(), removeRows(), removeColumns(), appendRows()
*/
void BOBUIextTable::appendColumns(int count)
{
    insertColumns(columns(), count);
}

/*!
    \fn void BOBUIextTable::removeRows(int index, int rows)

    Removes a number of \a rows starting with the row at the specified \a index.

    \sa insertRows(), insertColumns(), resize(), removeColumns(), appendRows(), appendColumns()
*/
void BOBUIextTable::removeRows(int pos, int num)
{
    Q_D(BOBUIextTable);
//     qDebug() << "-------- removeRows" << pos << num;

    if (num <= 0 || pos < 0)
        return;
    if (d->dirty)
        d->update();
    if (pos >= d->nRows)
        return;
    if (pos+num > d->nRows)
        num = d->nRows - pos;

    BOBUIextDocumentPrivate *p = d->pieceTable;
    BOBUIextFormatCollection *collection = p->formatCollection();
    p->beginEditBlock();

    // delete whole table?
    if (pos == 0 && num == d->nRows) {
        const int pos = p->fragmentMap().position(d->fragment_start);
        p->remove(pos, p->fragmentMap().position(d->fragment_end) - pos + 1);
        p->endEditBlock();
        return;
    }

    p->aboutToRemoveCell(cellAt(pos, 0).firstPosition(), cellAt(pos + num - 1, d->nCols - 1).lastPosition());

    QList<int> touchedCells;
    for (int r = pos; r < pos + num; ++r) {
        for (int c = 0; c < d->nCols; ++c) {
            int cell = d->grid[r*d->nCols + c];
            if (touchedCells.contains(cell))
                continue;
            touchedCells << cell;
            BOBUIextDocumentPrivate::FragmentIterator it(&p->fragmentMap(), cell);
            BOBUIextCharFormat fmt = collection->charFormat(it->format);
            int span = fmt.tableCellRowSpan();
            if (span > 1) {
                fmt.setTableCellRowSpan(span - 1);
                p->setCharFormat(it.position(), 1, fmt);
            } else {
                // remove cell
                int index = d->cells.indexOf(cell) + 1;
                int f_end = index < d->cells.size() ? d->cells.at(index) : d->fragment_end;
                p->remove(it.position(), p->fragmentMap().position(f_end) - it.position());
            }
        }
    }

    p->endEditBlock();
//     qDebug() << "-------- end removeRows" << pos << num;
}

/*!
    \fn void BOBUIextTable::removeColumns(int index, int columns)

    Removes a number of \a columns starting with the column at the specified
    \a index.

    \sa insertRows(), insertColumns(), removeRows(), resize(), appendRows(), appendColumns()
*/
void BOBUIextTable::removeColumns(int pos, int num)
{
    Q_D(BOBUIextTable);
//     qDebug() << "-------- removeCols" << pos << num;

    if (num <= 0 || pos < 0)
        return;
    if (d->dirty)
        d->update();
    if (pos >= d->nCols)
        return;
    if (pos + num > d->nCols)
        pos = d->nCols - num;

    BOBUIextDocumentPrivate *p = d->pieceTable;
    BOBUIextFormatCollection *collection = p->formatCollection();
    p->beginEditBlock();

    // delete whole table?
    if (pos == 0 && num == d->nCols) {
        const int pos = p->fragmentMap().position(d->fragment_start);
        p->remove(pos, p->fragmentMap().position(d->fragment_end) - pos + 1);
        p->endEditBlock();
        return;
    }

    p->aboutToRemoveCell(cellAt(0, pos).firstPosition(), cellAt(d->nRows - 1, pos + num - 1).lastPosition());

    QList<int> touchedCells;
    for (int r = 0; r < d->nRows; ++r) {
        for (int c = pos; c < pos + num; ++c) {
            int cell = d->grid[r*d->nCols + c];
            BOBUIextDocumentPrivate::FragmentIterator it(&p->fragmentMap(), cell);
            BOBUIextCharFormat fmt = collection->charFormat(it->format);
            int span = fmt.tableCellColumnSpan();
            if (touchedCells.contains(cell) && span <= 1)
                continue;
            touchedCells << cell;

            if (span > 1) {
                fmt.setTableCellColumnSpan(span - 1);
                p->setCharFormat(it.position(), 1, fmt);
            } else {
                // remove cell
                int index = d->cells.indexOf(cell) + 1;
                int f_end = index < d->cells.size() ? d->cells.at(index) : d->fragment_end;
                p->remove(it.position(), p->fragmentMap().position(f_end) - it.position());
            }
        }
    }

    BOBUIextTableFormat tfmt = format();
    tfmt.setColumns(tfmt.columns()-num);
    QList<BOBUIextLength> columnWidths = tfmt.columnWidthConstraints();
    if (columnWidths.size() > pos) {
        columnWidths.remove(pos, num);
        tfmt.setColumnWidthConstraints (columnWidths);
    }
    BOBUIextObject::setFormat(tfmt);

    p->endEditBlock();
//     qDebug() << "-------- end removeCols" << pos << num;
}

/*!
    \since 4.1

    Merges the cell at the specified \a row and \a column with the adjacent cells
    into one cell. The new cell will span \a numRows rows and \a numCols columns.
    This method does nothing if \a numRows or \a numCols is less than the current
    number of rows or columns spanned by the cell.

    \sa splitCell()
*/
void BOBUIextTable::mergeCells(int row, int column, int numRows, int numCols)
{
    Q_D(BOBUIextTable);

    if (d->dirty)
        d->update();

    BOBUIextDocumentPrivate *p = d->pieceTable;
    BOBUIextFormatCollection *fc = p->formatCollection();

    const BOBUIextTableCell cell = cellAt(row, column);
    if (!cell.isValid() || row != cell.row() || column != cell.column())
        return;

    BOBUIextCharFormat fmt = cell.format();
    const int rowSpan = fmt.tableCellRowSpan();
    const int colSpan = fmt.tableCellColumnSpan();

    numRows = qMin(numRows, rows() - cell.row());
    numCols = qMin(numCols, columns() - cell.column());

    // nothing to merge?
    if (numRows < rowSpan || numCols < colSpan)
        return;

    // check the edges of the merge rect to make sure no cell spans the edge
    for (int r = row; r < row + numRows; ++r) {
        if (cellAt(r, column) == cellAt(r, column - 1))
            return;
        if (cellAt(r, column + numCols) == cellAt(r, column + numCols - 1))
            return;
    }

    for (int c = column; c < column + numCols; ++c) {
        if (cellAt(row, c) == cellAt(row - 1, c))
            return;
        if (cellAt(row + numRows, c) == cellAt(row + numRows - 1, c))
            return;
    }

    p->beginEditBlock();

    const int origCellPosition = cell.firstPosition() - 1;

    const int cellFragment = d->grid[row * d->nCols + column];

    // find the position at which to insert the contents of the merged cells
    QFragmentFindHelper helper(origCellPosition, p->fragmentMap());
    const auto begin = d->cells.cbegin();
    const auto it = std::lower_bound(begin, d->cells.cend(), helper);
    Q_ASSERT(it != d->cells.cend());
    Q_ASSERT(!(helper < *it));
    Q_ASSERT(*it == cellFragment);
    const int insertCellIndex = it - begin;
    int insertFragment = d->cells.value(insertCellIndex + 1, d->fragment_end);
    uint insertPos = p->fragmentMap().position(insertFragment);

    d->blockFragmentUpdates = true;

    bool rowHasText = cell.firstCursorPosition().block().length();
    bool needsParagraph = rowHasText && colSpan == numCols;

    // find all cells that will be erased by the merge
    for (int r = row; r < row + numRows; ++r) {
        int firstColumn = r < row + rowSpan ? column + colSpan : column;

        // don't recompute the cell index for the first row
        int firstCellIndex = r == row ? insertCellIndex + 1 : -1;
        int cellIndex = firstCellIndex;

        for (int c = firstColumn; c < column + numCols; ++c) {
            const int fragment = d->grid[r * d->nCols + c];

            // already handled?
            if (fragment == cellFragment)
                continue;

            BOBUIextDocumentPrivate::FragmentIterator it(&p->fragmentMap(), fragment);
            uint pos = it.position();

            if (firstCellIndex == -1) {
                QFragmentFindHelper helper(pos, p->fragmentMap());
                const auto begin = d->cells.cbegin();
                const auto it = std::lower_bound(begin, d->cells.cend(), helper);
                Q_ASSERT(it != d->cells.cend());
                Q_ASSERT(!(helper < *it));
                Q_ASSERT(*it == fragment);
                firstCellIndex = cellIndex = it - begin;
            }

            ++cellIndex;

            BOBUIextCharFormat fmt = fc->charFormat(it->format);

            const int cellRowSpan = fmt.tableCellRowSpan();
            const int cellColSpan = fmt.tableCellColumnSpan();

            // update the grid for this cell
            for (int i = r; i < r + cellRowSpan; ++i)
                for (int j = c; j < c + cellColSpan; ++j)
                    d->grid[i * d->nCols + j] = cellFragment;

            // erase the cell marker
            p->remove(pos, 1);

            const int nextFragment = d->cells.value(cellIndex, d->fragment_end);
            const uint nextPos = p->fragmentMap().position(nextFragment);

            Q_ASSERT(nextPos >= pos);

            // merge the contents of the cell (if not empty)
            if (nextPos > pos) {
                if (needsParagraph) {
                    needsParagraph = false;
                    BOBUIextCursorPrivate::fromPosition(p, insertPos++).insertBlock();
                    p->move(pos + 1, insertPos, nextPos - pos);
                } else if (rowHasText) {
                    BOBUIextCursorPrivate::fromPosition(p, insertPos++).insertText(" "_L1);
                    p->move(pos + 1, insertPos, nextPos - pos);
                } else {
                    p->move(pos, insertPos, nextPos - pos);
                }

                insertPos += nextPos - pos;
                rowHasText = true;
            }
        }

        if (rowHasText) {
            needsParagraph = true;
            rowHasText = false;
        }

        // erase cells from last row
        if (firstCellIndex >= 0) {
            d->cellIndices.remove(firstCellIndex, cellIndex - firstCellIndex);
            d->cells.erase(d->cells.begin() + firstCellIndex, d->cells.begin() + cellIndex);
        }
    }

    d->fragment_start = d->cells.constFirst();

    fmt.setTableCellRowSpan(numRows);
    fmt.setTableCellColumnSpan(numCols);
    p->setCharFormat(origCellPosition, 1, fmt);

    d->blockFragmentUpdates = false;
    d->dirty = false;

    p->endEditBlock();
}

/*!
    \overload
    \since 4.1

    Merges the cells selected by the provided \a cursor.

    \sa splitCell()
*/
void BOBUIextTable::mergeCells(const BOBUIextCursor &cursor)
{
    if (!cursor.hasComplexSelection())
        return;

    int firstRow, numRows, firstColumn, numColumns;
    cursor.selectedTableCells(&firstRow, &numRows, &firstColumn, &numColumns);
    mergeCells(firstRow, firstColumn, numRows, numColumns);
}

/*!
    \since 4.1

    Splits the specified cell at \a row and \a column into an array of multiple
    cells with dimensions specified by \a numRows and \a numCols.

    \note It is only possible to split cells that span multiple rows or columns, such as rows
    that have been merged using mergeCells().

    \sa mergeCells()
*/
void BOBUIextTable::splitCell(int row, int column, int numRows, int numCols)
{
    Q_D(BOBUIextTable);

    if (d->dirty)
        d->update();

    BOBUIextDocumentPrivate *p = d->pieceTable;
    BOBUIextFormatCollection *c = p->formatCollection();

    const BOBUIextTableCell cell = cellAt(row, column);
    if (!cell.isValid())
        return;
    row = cell.row();
    column = cell.column();

    BOBUIextCharFormat fmt = cell.format();
    const int rowSpan = fmt.tableCellRowSpan();
    const int colSpan = fmt.tableCellColumnSpan();

    // nothing to split?
    if (numRows > rowSpan || numCols > colSpan)
        return;

    p->beginEditBlock();

    const int origCellPosition = cell.firstPosition() - 1;

    QVarLengthArray<int> rowPositions(rowSpan);

    rowPositions[0] = cell.lastPosition();

    for (int r = row + 1; r < row + rowSpan; ++r) {
        // find the cell before which to insert the new cell markers
        int gridIndex = r * d->nCols + column;
        const auto begin = d->cellIndices.cbegin();
        const auto it = std::upper_bound(begin, d->cellIndices.cend(), gridIndex);
        int fragment = d->cells.value(it - begin, d->fragment_end);
        rowPositions[r - row] = p->fragmentMap().position(fragment);
    }

    fmt.setTableCellColumnSpan(1);
    fmt.setTableCellRowSpan(1);
    const int fmtIndex = c->indexForFormat(fmt);
    const int blockIndex = p->blockMap().find(cell.lastPosition())->format;

    int insertAdjustement = 0;
    for (int i = 0; i < numRows; ++i) {
        for (int c = 0; c < colSpan - numCols; ++c)
            p->insertBlock(BOBUIextBeginningOfFrame, rowPositions[i] + insertAdjustement + c, blockIndex, fmtIndex);
        insertAdjustement += colSpan - numCols;
    }

    for (int i = numRows; i < rowSpan; ++i) {
        for (int c = 0; c < colSpan; ++c)
            p->insertBlock(BOBUIextBeginningOfFrame, rowPositions[i] + insertAdjustement + c, blockIndex, fmtIndex);
        insertAdjustement += colSpan;
    }

    fmt.setTableCellRowSpan(numRows);
    fmt.setTableCellColumnSpan(numCols);
    p->setCharFormat(origCellPosition, 1, fmt);

    p->endEditBlock();
}

/*!
    Returns the number of rows in the table.

    \sa columns()
*/
int BOBUIextTable::rows() const
{
    Q_D(const BOBUIextTable);
    if (d->dirty)
        d->update();

    return d->nRows;
}

/*!
    Returns the number of columns in the table.

    \sa rows()
*/
int BOBUIextTable::columns() const
{
    Q_D(const BOBUIextTable);
    if (d->dirty)
        d->update();

    return d->nCols;
}

/*!
    \fn BOBUIextCursor BOBUIextTable::rowStart(const BOBUIextCursor &cursor) const

    Returns a cursor pointing to the start of the row that contains the
    given \a cursor.

    \sa rowEnd()
*/
BOBUIextCursor BOBUIextTable::rowStart(const BOBUIextCursor &c) const
{
    Q_D(const BOBUIextTable);
    BOBUIextTableCell cell = cellAt(c);
    if (!cell.isValid())
        return BOBUIextCursor();

    int row = cell.row();
    BOBUIextDocumentPrivate *p = d->pieceTable;
    BOBUIextDocumentPrivate::FragmentIterator it(&p->fragmentMap(), d->grid[row*d->nCols]);
    return BOBUIextCursorPrivate::fromPosition(p, it.position());
}

/*!
    \fn BOBUIextCursor BOBUIextTable::rowEnd(const BOBUIextCursor &cursor) const

    Returns a cursor pointing to the end of the row that contains the given
    \a cursor.

    \sa rowStart()
*/
BOBUIextCursor BOBUIextTable::rowEnd(const BOBUIextCursor &c) const
{
    Q_D(const BOBUIextTable);
    BOBUIextTableCell cell = cellAt(c);
    if (!cell.isValid())
        return BOBUIextCursor();

    int row = cell.row() + 1;
    int fragment = row < d->nRows ? d->grid[row*d->nCols] : d->fragment_end;
    BOBUIextDocumentPrivate *p = d->pieceTable;
    BOBUIextDocumentPrivate::FragmentIterator it(&p->fragmentMap(), fragment);
    return BOBUIextCursorPrivate::fromPosition(p, it.position() - 1);
}

/*!
    \fn void BOBUIextTable::setFormat(const BOBUIextTableFormat &format)

    Sets the table's \a format.

    \sa format()
*/
void BOBUIextTable::setFormat(const BOBUIextTableFormat &format)
{
    BOBUIextTableFormat fmt = format;
    // don't try to change the number of table columns from here
    fmt.setColumns(columns());
    BOBUIextObject::setFormat(fmt);
}

/*!
    \fn BOBUIextTableFormat BOBUIextTable::format() const

    Returns the table's format.

    \sa setFormat()
*/

BOBUI_END_NAMESPACE

#include "moc_bobuiexttable.cpp"
