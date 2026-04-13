// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTTABLE_P_H
#define BOBUIEXTTABLE_P_H

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
#include "private/bobuiextobject_p.h"
#include "private/bobuiextdocument_p.h"

#include <vector>

BOBUI_BEGIN_NAMESPACE

class BOBUIextTablePrivate : public BOBUIextFramePrivate
{
    Q_DECLARE_PUBLIC(BOBUIextTable)
public:
    BOBUIextTablePrivate(BOBUIextDocument *document) : BOBUIextFramePrivate(document), nRows(0), nCols(0), dirty(true), blockFragmentUpdates(false) {}

    static BOBUIextTable *createTable(BOBUIextDocumentPrivate *, int pos, int rows, int cols, const BOBUIextTableFormat &tableFormat);
    void fragmentAdded(QChar type, uint fragment) override;
    void fragmentRemoved(QChar type, uint fragment) override;

    void update() const;

    int findCellIndex(int fragment) const;

    QList<int> cells;
    // symmetric to cells array and maps to indecs in grid,
    // used for fast-lookup for row/column by fragment
    mutable QList<int> cellIndices;
    mutable std::vector<int> grid;
    mutable int nRows;
    mutable int nCols;
    mutable bool dirty;
    bool blockFragmentUpdates;
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTTABLE_P_H
