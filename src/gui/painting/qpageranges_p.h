// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPAGERANGES_P_H
#define QPAGERANGES_P_H

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

BOBUI_BEGIN_NAMESPACE

class QPageRangesPrivate : public QSharedData
{
public:
    void mergeIntervals();

    QList<QPageRanges::Range> intervals;
};

BOBUI_END_NAMESPACE

#endif // QPAGERANGES_P_H
