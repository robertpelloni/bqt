// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTMARKDOWNWRITER_P_H
#define BOBUIEXTMARKDOWNWRITER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUICore/BOBUIextStream>

#include "bobuiextdocument_p.h"
#include "bobuiextdocumentwriter.h"

BOBUI_BEGIN_NAMESPACE

class QAbstractItemModel;

class Q_GUI_EXPORT BOBUIextMarkdownWriter
{
public:
    BOBUIextMarkdownWriter(BOBUIextStream &stream, BOBUIextDocument::MarkdownFeatures features);
    bool writeAll(const BOBUIextDocument *document);
#if BOBUI_CONFIG(itemmodel)
    void writeTable(const QAbstractItemModel *table);
#endif

    int writeBlock(const BOBUIextBlock &block, bool table, bool ignoreFormat, bool ignoreEmpty);
    void writeFrame(const BOBUIextFrame *frame);
    void writeFrontMatter(const QString &fm);

private:
    struct ListInfo {
        bool loose;
    };

    ListInfo listInfo(BOBUIextList *list);
    void setLinePrefixForBlockQuote(int level);

private:
    BOBUIextStream &m_stream;
    BOBUIextDocument::MarkdownFeatures m_features;
    QMap<BOBUIextList *, ListInfo> m_listInfo;
    QString m_linePrefix;
    QString m_codeBlockFence;
    int m_wrappedLineIndent = 0;
    int m_lastListIndent = 1;
    bool m_doubleNewlineWritten = false;
    bool m_linePrefixWritten = false;
    bool m_indentedCodeBlock = false;
    bool m_fencedCodeBlock = false;
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTMARKDOWNWRITER_P_H
