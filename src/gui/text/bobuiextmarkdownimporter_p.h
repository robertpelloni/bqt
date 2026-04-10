// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTMARKDOWNIMPORTER_H
#define BOBUIEXTMARKDOWNIMPORTER_H

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

#include <BobUIGui/qfont.h>
#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qpalette.h>
#include <BobUIGui/bobuiextdocument.h>
#include <BobUIGui/bobuiextlist.h>
#include <BobUICore/qpointer.h>
#include <BobUICore/qstack.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIextCursor;
class BOBUIextDocument;
class BOBUIextTable;

class Q_GUI_EXPORT BOBUIextMarkdownImporter
{
public:
    enum Feature {
        FeatureCollapseWhitespace =       0x0001,
        FeaturePermissiveATXHeaders =     0x0002,
        FeaturePermissiveURLAutoLinks =   0x0004,
        FeaturePermissiveMailAutoLinks =  0x0008,
        FeatureNoIndentedCodeBlocks =     0x0010,
        FeatureNoHTMLBlocks =             0x0020,
        FeatureNoHTMLSpans =              0x0040,
        FeatureTables =                   0x0100,
        FeatureStrikeThrough =            0x0200,
        FeaturePermissiveWWWAutoLinks =   0x0400,
        FeatureTasklists =                0x0800,
        FeatureUnderline =                0x4000,
        FeatureFrontMatter =            0x100000, // BobUI feature, not yet in MD4C
        // composite flags
        FeaturePermissiveAutoLinks = FeaturePermissiveMailAutoLinks
            | FeaturePermissiveURLAutoLinks | FeaturePermissiveWWWAutoLinks,
        FeatureNoHTML = BOBUIextDocument::MarkdownNoHTML,
        DialectCommonMark = BOBUIextDocument::MarkdownDialectCommonMark,
        DialectGitHub = BOBUIextDocument::MarkdownDialectGitHub
    };
    Q_DECLARE_FLAGS(Features, Feature)

    BOBUIextMarkdownImporter(BOBUIextDocument *doc, Features features);
    BOBUIextMarkdownImporter(BOBUIextDocument *doc, BOBUIextDocument::MarkdownFeatures features);

    void import(const QString &markdown);

public:
    // MD4C callbacks
    int cbEnterBlock(int blockType, void* detail);
    int cbLeaveBlock(int blockType, void* detail);
    int cbEnterSpan(int spanType, void* detail);
    int cbLeaveSpan(int spanType, void* detail);
    int cbText(int textType, const char* text, unsigned size);

private:
    void insertBlock();

private:
    BOBUIextCursor m_cursor;
    BOBUIextTable *m_currentTable = nullptr; // because m_cursor->currentTable() doesn't work
#if BOBUI_CONFIG(regularexpression)
    QString m_htmlAccumulator;
#endif
    QString m_blockCodeLanguage;
    QList<int> m_nonEmptyTableCells; // in the current row
    QStack<QPointer<BOBUIextList>> m_listStack;
    QStack<BOBUIextCharFormat> m_spanFormatStack;
    QFont m_monoFont;
    QPalette m_palette;
#if BOBUI_CONFIG(regularexpression)
    int m_htmlTagDepth = 0;
#endif
    int m_blockQuoteDepth = 0;
    int m_tableColumnCount = 0;
    int m_tableRowCount = 0;
    int m_tableCol = -1; // because relative cell movements (e.g. m_cursor->movePosition(BOBUIextCursor::NextCell)) don't work
    int m_paragraphMargin = 0;
    int m_blockType = 0;
    char m_blockCodeFence = 0;
    Features m_features;
    BOBUIextImageFormat m_imageFormat;
    BOBUIextListFormat m_listFormat;
    BOBUIextBlockFormat::MarkerType m_markerType = BOBUIextBlockFormat::MarkerType::NoMarker;
    bool m_needsInsertBlock = false;
    bool m_needsInsertList = false;
    bool m_listItem = false; // true from the beginning of LI to the end of the first P
    bool m_codeBlock = false;
    bool m_imageSpan = false;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BOBUIextMarkdownImporter::Features)

BOBUI_END_NAMESPACE

#endif // BOBUIEXTMARKDOWNIMPORTER_H
