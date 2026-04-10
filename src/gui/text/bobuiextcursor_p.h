// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTCURSOR_P_H
#define BOBUIEXTCURSOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "bobuiextcursor.h"
#include "bobuiextdocument.h"
#include "bobuiextdocument_p.h"
#include <private/bobuiextformat_p.h>
#include "bobuiextobject.h"

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT BOBUIextCursorPrivate : public QSharedData
{
public:
    BOBUIextCursorPrivate(BOBUIextDocumentPrivate *p);
    BOBUIextCursorPrivate(const BOBUIextCursorPrivate &rhs);
    ~BOBUIextCursorPrivate();

    static inline BOBUIextCursorPrivate *getPrivate(BOBUIextCursor *c) { return c->d; }

    enum AdjustResult { CursorMoved, CursorUnchanged };
    AdjustResult adjustPosition(int positionOfChange, int charsAddedOrRemoved, BOBUIextUndoCommand::Operation op);

    void adjustCursor(BOBUIextCursor::MoveOperation m);

    void remove();
    void clearCells(BOBUIextTable *table, int startRow, int startCol, int numRows, int numCols, BOBUIextUndoCommand::Operation op);
    inline bool setPosition(int newPosition) {
        Q_ASSERT(newPosition >= 0 && newPosition < priv->length());
        bool moved = position != newPosition;
        if (moved) {
            position = newPosition;
            currentCharFormat = -1;
        }
        return moved;
    }
    void setX();
    bool canDelete(int pos) const;

    void insertBlock(const BOBUIextBlockFormat &format, const BOBUIextCharFormat &charFormat);
    bool movePosition(BOBUIextCursor::MoveOperation op, BOBUIextCursor::MoveMode mode = BOBUIextCursor::MoveAnchor);

    inline BOBUIextBlock block() const
        { return BOBUIextBlock(priv, priv->blockMap().findNode(position)); }
    inline BOBUIextBlockFormat blockFormat() const
        { return block().blockFormat(); }

    BOBUIextLayout *blockLayout(BOBUIextBlock &block) const;

    BOBUIextTable *complexSelectionTable() const;
    void selectedTableCells(int *firstRow, int *numRows, int *firstColumn, int *numColumns) const;

    void setBlockCharFormat(const BOBUIextCharFormat &format, BOBUIextDocumentPrivate::FormatChangeMode changeMode);
    void setBlockFormat(const BOBUIextBlockFormat &format, BOBUIextDocumentPrivate::FormatChangeMode changeMode);
    void setCharFormat(const BOBUIextCharFormat &format, BOBUIextDocumentPrivate::FormatChangeMode changeMode);

    void aboutToRemoveCell(int from, int to);

    static BOBUIextCursor fromPosition(BOBUIextDocumentPrivate *d, int pos)
    { return BOBUIextCursor(d, pos); }

    BOBUIextDocumentPrivate *priv;
    qreal x;
    int position;
    int anchor;
    int adjusted_anchor;
    int currentCharFormat;
    uint visualNavigation : 1;
    uint keepPositionOnInsert : 1;
    uint changed : 1;
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTCURSOR_P_H
