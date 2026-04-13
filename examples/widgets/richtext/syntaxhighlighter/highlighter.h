// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <BOBUIextCharFormat>
#include <QRegularExpression>

BOBUI_BEGIN_NAMESPACE
class BOBUIextDocument;
BOBUI_END_NAMESPACE

//! [0]
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(BOBUIextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        BOBUIextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    BOBUIextCharFormat keywordFormat;
    BOBUIextCharFormat classFormat;
    BOBUIextCharFormat singleLineCommentFormat;
    BOBUIextCharFormat multiLineCommentFormat;
    BOBUIextCharFormat quotationFormat;
    BOBUIextCharFormat functionFormat;
};
//! [0]

#endif // HIGHLIGHTER_H
