// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTTABLE_H
#define BOBUIEXTTABLE_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qobject.h>
#include <BobUIGui/bobuiextobject.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIextCursor;
class BOBUIextTable;
class BOBUIextTablePrivate;

class Q_GUI_EXPORT BOBUIextTableCell
{
public:
    BOBUIextTableCell() : table(nullptr) {}
    ~BOBUIextTableCell() {}
    BOBUIextTableCell(const BOBUIextTableCell &o) : table(o.table), fragment(o.fragment) {}
    BOBUIextTableCell &operator=(const BOBUIextTableCell &o)
    { table = o.table; fragment = o.fragment; return *this; }

    void setFormat(const BOBUIextCharFormat &format);
    BOBUIextCharFormat format() const;

    int row() const;
    int column() const;

    int rowSpan() const;
    int columnSpan() const;

    inline bool isValid() const { return table != nullptr; }

    BOBUIextCursor firstCursorPosition() const;
    BOBUIextCursor lastCursorPosition() const;
    int firstPosition() const;
    int lastPosition() const;

    inline bool operator==(const BOBUIextTableCell &other) const
    { return table == other.table && fragment == other.fragment; }
    inline bool operator!=(const BOBUIextTableCell &other) const
    { return !operator==(other); }

    BOBUIextFrame::iterator begin() const;
    BOBUIextFrame::iterator end() const;

    int tableCellFormatIndex() const;

private:
    friend class BOBUIextTable;
    BOBUIextTableCell(const BOBUIextTable *t, int f)
        : table(t), fragment(f) {}

    const BOBUIextTable *table;
    int fragment;
};

class Q_GUI_EXPORT BOBUIextTable : public BOBUIextFrame
{
    Q_OBJECT
public:
    explicit BOBUIextTable(BOBUIextDocument *doc);
    ~BOBUIextTable();

    void resize(int rows, int cols);
    void insertRows(int pos, int num);
    void insertColumns(int pos, int num);
    void appendRows(int count);
    void appendColumns(int count);
    void removeRows(int pos, int num);
    void removeColumns(int pos, int num);

    void mergeCells(int row, int col, int numRows, int numCols);
    void mergeCells(const BOBUIextCursor &cursor);
    void splitCell(int row, int col, int numRows, int numCols);

    int rows() const;
    int columns() const;

    BOBUIextTableCell cellAt(int row, int col) const;
    BOBUIextTableCell cellAt(int position) const;
    BOBUIextTableCell cellAt(const BOBUIextCursor &c) const;

    BOBUIextCursor rowStart(const BOBUIextCursor &c) const;
    BOBUIextCursor rowEnd(const BOBUIextCursor &c) const;

    void setFormat(const BOBUIextTableFormat &format);
    BOBUIextTableFormat format() const { return BOBUIextObject::format().toTableFormat(); }

private:
    Q_DISABLE_COPY(BOBUIextTable)
    Q_DECLARE_PRIVATE(BOBUIextTable)
    friend class BOBUIextTableCell;
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTTABLE_H
