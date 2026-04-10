// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTCURSOR_H
#define BOBUIEXTCURSOR_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qshareddata.h>
#include <BobUIGui/bobuiextdocument.h>
#include <BobUIGui/bobuiextformat.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIextCursorPrivate;
class BOBUIextDocumentFragment;
class BOBUIextCharFormat;
class BOBUIextBlockFormat;
class BOBUIextListFormat;
class BOBUIextTableFormat;
class BOBUIextFrameFormat;
class BOBUIextImageFormat;
class BOBUIextDocumentPrivate;
class BOBUIextList;
class BOBUIextTable;
class BOBUIextFrame;
class BOBUIextBlock;

class Q_GUI_EXPORT BOBUIextCursor
{
public:
    BOBUIextCursor();
    explicit BOBUIextCursor(BOBUIextDocument *document);
    BOBUIextCursor(BOBUIextDocumentPrivate *p, int pos);
    explicit BOBUIextCursor(BOBUIextCursorPrivate *d);
    explicit BOBUIextCursor(BOBUIextFrame *frame);
    explicit BOBUIextCursor(const BOBUIextBlock &block);
    BOBUIextCursor(const BOBUIextCursor &cursor);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(BOBUIextCursor)
    BOBUIextCursor &operator=(const BOBUIextCursor &other);
    ~BOBUIextCursor();

    void swap(BOBUIextCursor &other) noexcept { d.swap(other.d); }

    bool isNull() const;

    enum MoveMode {
        MoveAnchor,
        KeepAnchor
    };

    void setPosition(int pos, MoveMode mode = MoveAnchor);
    int position() const;
    int positionInBlock() const;

    int anchor() const;

    void insertText(const QString &text);
    void insertText(const QString &text, const BOBUIextCharFormat &format);

    enum MoveOperation {
        NoMove,

        Start,
        Up,
        StartOfLine,
        StartOfBlock,
        StartOfWord,
        PreviousBlock,
        PreviousCharacter,
        PreviousWord,
        Left,
        WordLeft,

        End,
        Down,
        EndOfLine,
        EndOfWord,
        EndOfBlock,
        NextBlock,
        NextCharacter,
        NextWord,
        Right,
        WordRight,

        NextCell,
        PreviousCell,
        NextRow,
        PreviousRow
    };

    bool movePosition(MoveOperation op, MoveMode = MoveAnchor, int n = 1);

    bool visualNavigation() const;
    void setVisualNavigation(bool b);

    void setVerticalMovementX(int x);
    int verticalMovementX() const;

    void setKeepPositionOnInsert(bool b);
    bool keepPositionOnInsert() const;

    void deleteChar();
    void deletePreviousChar();

    enum SelectionType {
        WordUnderCursor,
        LineUnderCursor,
        BlockUnderCursor,
        Document
    };
    void select(SelectionType selection);

    bool hasSelection() const;
    bool hasComplexSelection() const;
    void removeSelectedText();
    void clearSelection();
    int selectionStart() const;
    int selectionEnd() const;

    QString selectedText() const;
    BOBUIextDocumentFragment selection() const;
    void selectedTableCells(int *firstRow, int *numRows, int *firstColumn, int *numColumns) const;

    BOBUIextBlock block() const;

    BOBUIextCharFormat charFormat() const;
    void setCharFormat(const BOBUIextCharFormat &format);
    void mergeCharFormat(const BOBUIextCharFormat &modifier);

    BOBUIextBlockFormat blockFormat() const;
    void setBlockFormat(const BOBUIextBlockFormat &format);
    void mergeBlockFormat(const BOBUIextBlockFormat &modifier);

    BOBUIextCharFormat blockCharFormat() const;
    void setBlockCharFormat(const BOBUIextCharFormat &format);
    void mergeBlockCharFormat(const BOBUIextCharFormat &modifier);

    bool atBlockStart() const;
    bool atBlockEnd() const;
    bool atStart() const;
    bool atEnd() const;

    void insertBlock();
    void insertBlock(const BOBUIextBlockFormat &format);
    void insertBlock(const BOBUIextBlockFormat &format, const BOBUIextCharFormat &charFormat);

    BOBUIextList *insertList(const BOBUIextListFormat &format);
    BOBUIextList *insertList(BOBUIextListFormat::Style style);

    BOBUIextList *createList(const BOBUIextListFormat &format);
    BOBUIextList *createList(BOBUIextListFormat::Style style);
    BOBUIextList *currentList() const;

    BOBUIextTable *insertTable(int rows, int cols, const BOBUIextTableFormat &format);
    BOBUIextTable *insertTable(int rows, int cols);
    BOBUIextTable *currentTable() const;

    BOBUIextFrame *insertFrame(const BOBUIextFrameFormat &format);
    BOBUIextFrame *currentFrame() const;

    void insertFragment(const BOBUIextDocumentFragment &fragment);

#ifndef BOBUI_NO_TEXTHTMLPARSER
    void insertHtml(const QString &html);
#endif // BOBUI_NO_TEXTHTMLPARSER
#if BOBUI_CONFIG(textmarkdownreader)
    void insertMarkdown(const QString &markdown,
                        BOBUIextDocument::MarkdownFeatures features = BOBUIextDocument::MarkdownDialectGitHub);
#endif // textmarkdownreader

    void insertImage(const BOBUIextImageFormat &format, BOBUIextFrameFormat::Position alignment);
    void insertImage(const BOBUIextImageFormat &format);
    void insertImage(const QString &name);
    void insertImage(const QImage &image, const QString &name = QString());

    void beginEditBlock();
    void joinPreviousEditBlock();
    void endEditBlock();

    bool operator!=(const BOBUIextCursor &rhs) const;
    bool operator<(const BOBUIextCursor &rhs) const;
    bool operator<=(const BOBUIextCursor &rhs) const;
    bool operator==(const BOBUIextCursor &rhs) const;
    bool operator>=(const BOBUIextCursor &rhs) const;
    bool operator>(const BOBUIextCursor &rhs) const;

    bool isCopyOf(const BOBUIextCursor &other) const;

    int blockNumber() const;
    int columnNumber() const;

    BOBUIextDocument *document() const;

private:
    QSharedDataPointer<BOBUIextCursorPrivate> d;
    friend class BOBUIextCursorPrivate;
    friend class BOBUIextDocumentPrivate;
    friend class BOBUIextDocumentFragmentPrivate;
    friend class BOBUIextCopyHelper;
    friend class QWidgetTextControlPrivate;
};

Q_DECLARE_SHARED(BOBUIextCursor)

BOBUI_END_NAMESPACE

#endif // BOBUIEXTCURSOR_H
