// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTDOCUMENTFRAGMENT_P_H
#define BOBUIEXTDOCUMENTFRAGMENT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUIGui/bobuiextdocument.h"
#include "private/bobuiexthtmlparser_p.h"
#include "private/bobuiextdocument_p.h"
#include "BobUIGui/bobuiexttable.h"
#include "BobUICore/qatomic.h"
#include "BobUICore/qlist.h"
#include "BobUICore/qmap.h"
#include "BobUICore/qpointer.h"
#include "BobUICore/qvarlengtharray.h"
#include "BobUICore/qdatastream.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIextDocumentFragmentPrivate;

class BOBUIextCopyHelper
{
public:
    BOBUIextCopyHelper(const BOBUIextCursor &_source, const BOBUIextCursor &_destination, bool forceCharFormat = false, const BOBUIextCharFormat &fmt = BOBUIextCharFormat());

    void copy();

private:
    void appendFragments(int pos, int endPos);
    int appendFragment(int pos, int endPos, int objectIndex = -1);
    int convertFormatIndex(const BOBUIextFormat &oldFormat, int objectIndexToSet = -1);
    inline int convertFormatIndex(int oldFormatIndex, int objectIndexToSet = -1)
    { return convertFormatIndex(src->formatCollection()->format(oldFormatIndex), objectIndexToSet); }
    inline BOBUIextFormat convertFormat(const BOBUIextFormat &fmt)
    { return dst->formatCollection()->format(convertFormatIndex(fmt)); }

    int insertPos;

    bool forceCharFormat;
    int primaryCharFormatIndex;

    BOBUIextCursor cursor;
    BOBUIextDocumentPrivate *dst;
    BOBUIextDocumentPrivate *src;
    BOBUIextFormatCollection &formatCollection;
    const QString originalText;
    QMap<int, int> objectIndexMap;
};

class BOBUIextDocumentFragmentPrivate
{
public:
    BOBUIextDocumentFragmentPrivate(const BOBUIextCursor &cursor = BOBUIextCursor());
    inline ~BOBUIextDocumentFragmentPrivate() { delete doc; }

    void insert(BOBUIextCursor &cursor) const;

    QAtomicInt ref;
    BOBUIextDocument *doc;

    uint importedFromPlainText : 1;
private:
    Q_DISABLE_COPY_MOVE(BOBUIextDocumentFragmentPrivate)
};

#ifndef BOBUI_NO_TEXTHTMLPARSER

class BOBUIextHtmlImporter : public BOBUIextHtmlParser
{
    struct Table;
public:
    enum ImportMode {
        ImportToFragment,
        ImportToDocument
    };

    BOBUIextHtmlImporter(BOBUIextDocument *_doc, const QString &html,
                      ImportMode mode,
                      const BOBUIextDocument *resourceProvider = nullptr);

    void import();

private:
    bool closeTag();

    Table scanTable(int tableNodeIdx);

    enum ProcessNodeResult { ContinueWithNextNode, ContinueWithCurrentNode, ContinueWithNextSibling };

    void appendBlock(const BOBUIextBlockFormat &format, BOBUIextCharFormat charFmt = BOBUIextCharFormat());
    bool appendNodeText();

    ProcessNodeResult processBlockNode();
    ProcessNodeResult processSpecialNodes();

    struct List
    {
        inline List() : listNode(0) {}
        BOBUIextListFormat format;
        int listNode;
        QPointer<BOBUIextList> list;
    };
    friend class BOBUIypeInfo<List>;
    QList<List> lists;
    int indent;
    int headingLevel;

    // insert a named anchor the next time we emit a char format,
    // either in a block or in regular text
    QStringList namedAnchors;

#ifdef Q_CC_SUN
    friend struct BOBUIextHtmlImporter::Table;
#endif
    struct TableCellIterator
    {
        inline TableCellIterator(BOBUIextTable *t = nullptr) : table(t), row(0), column(0) {}

        inline TableCellIterator &operator++() {
            if (atEnd())
                return *this;
            do {
                const BOBUIextTableCell cell = table->cellAt(row, column);
                if (!cell.isValid())
                    break;
                column += cell.columnSpan();
                if (column >= table->columns()) {
                    column = 0;
                    ++row;
                }
            } while (row < table->rows() && table->cellAt(row, column).row() != row);

            return *this;
        }

        inline bool atEnd() const { return table == nullptr || row >= table->rows(); }

        BOBUIextTableCell cell() const { return table->cellAt(row, column); }

        BOBUIextTable *table;
        int row;
        int column;
    };
    friend class BOBUIypeInfo<TableCellIterator>;

    friend struct Table;
    struct Table
    {
        Table() : isTextFrame(false), rows(0), columns(0), currentRow(0), lastIndent(0) {}
        QPointer<BOBUIextFrame> frame;
        bool isTextFrame;
        int rows;
        int columns;
        int currentRow; // ... for buggy html (see html_skipCell testcase)
        TableCellIterator currentCell;
        int lastIndent;
    };
    friend class BOBUIypeInfo<Table>;
    QList<Table> tables;

    struct RowColSpanInfo
    {
        int row, col;
        int rowSpan, colSpan;
    };
    friend class BOBUIypeInfo<RowColSpanInfo>;

    enum WhiteSpace
    {
        RemoveWhiteSpace,
        CollapseWhiteSpace,
        PreserveWhiteSpace
    };

    WhiteSpace compressNextWhitespace;

    BOBUIextDocument *doc;
    BOBUIextCursor cursor;
    BOBUIextHtmlParserNode::WhiteSpaceMode wsm;
    ImportMode importMode;
    bool hasBlock;
    bool forceBlockMerging;
    bool blockTagClosed;
    int currentNodeIdx;
    const BOBUIextHtmlParserNode *currentNode;
};
Q_DECLARE_TYPEINFO(BOBUIextHtmlImporter::List, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(BOBUIextHtmlImporter::TableCellIterator, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(BOBUIextHtmlImporter::Table, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(BOBUIextHtmlImporter::RowColSpanInfo, Q_PRIMITIVE_TYPE);

BOBUI_END_NAMESPACE
#endif // BOBUI_NO_TEXTHTMLPARSER

#endif // BOBUIEXTDOCUMENTFRAGMENT_P_H
