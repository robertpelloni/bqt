// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTOBJECT_P_H
#define BOBUIEXTOBJECT_P_H

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
#include "BobUIGui/bobuiextobject.h"
#include "private/qobject_p.h"
#include "BobUIGui/bobuiextdocument.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIextDocumentPrivate;

class BOBUIextObjectPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(BOBUIextObject)
public:
    BOBUIextObjectPrivate(BOBUIextDocument *doc)
        : pieceTable(doc->d_func()), objectIndex(-1)
    {
    }
    BOBUIextDocumentPrivate *pieceTable;
    int objectIndex;
};

class BOBUIextBlockGroupPrivate : public BOBUIextObjectPrivate
{
    Q_DECLARE_PUBLIC(BOBUIextBlockGroup)
public:
    BOBUIextBlockGroupPrivate(BOBUIextDocument *doc)
        : BOBUIextObjectPrivate(doc)
    {
    }
    typedef QList<BOBUIextBlock> BlockList;
    BlockList blocks;
    void markBlocksDirty();
};

class BOBUIextFrameLayoutData;

class BOBUIextFramePrivate : public BOBUIextObjectPrivate
{
    friend class BOBUIextDocumentPrivate;
    Q_DECLARE_PUBLIC(BOBUIextFrame)
public:
    BOBUIextFramePrivate(BOBUIextDocument *doc)
        : BOBUIextObjectPrivate(doc), fragment_start(0), fragment_end(0), parentFrame(nullptr), layoutData(nullptr)
    {
    }
    virtual void fragmentAdded(QChar type, uint fragment);
    virtual void fragmentRemoved(QChar type, uint fragment);
    void remove_me();

    uint fragment_start;
    uint fragment_end;

    BOBUIextFrame *parentFrame;
    QList<BOBUIextFrame *> childFrames;
    BOBUIextFrameLayoutData *layoutData;
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTOBJECT_P_H
