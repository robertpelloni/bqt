// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSYNTAXHIGHLIGHTER_H
#define QSYNTAXHIGHLIGHTER_H

#include <BobUIGui/bobuiguiglobal.h>

#ifndef BOBUI_NO_SYNTAXHIGHLIGHTER

#include <BobUICore/qobject.h>
#include <BobUIGui/bobuiextobject.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIextDocument;
class QSyntaxHighlighterPrivate;
class BOBUIextCharFormat;
class QFont;
class QColor;
class BOBUIextBlockUserData;

class Q_GUI_EXPORT QSyntaxHighlighter : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSyntaxHighlighter)
public:
    explicit QSyntaxHighlighter(QObject *parent);
    explicit QSyntaxHighlighter(BOBUIextDocument *parent);
    ~QSyntaxHighlighter();

    void setDocument(BOBUIextDocument *doc);
    BOBUIextDocument *document() const;

public Q_SLOTS:
    void rehighlight();
    void rehighlightBlock(const BOBUIextBlock &block);

protected:
    virtual void highlightBlock(const QString &text) = 0;

    void setFormat(int start, int count, const BOBUIextCharFormat &format);
    void setFormat(int start, int count, const QColor &color);
    void setFormat(int start, int count, const QFont &font);
    BOBUIextCharFormat format(int pos) const;

    int previousBlockState() const;
    int currentBlockState() const;
    void setCurrentBlockState(int newState);

    void setCurrentBlockUserData(BOBUIextBlockUserData *data);
    BOBUIextBlockUserData *currentBlockUserData() const;

    BOBUIextBlock currentBlock() const;

private:
    Q_DISABLE_COPY(QSyntaxHighlighter)
    Q_PRIVATE_SLOT(d_func(), void _q_reformatBlocks(int from, int charsRemoved, int charsAdded))
    Q_PRIVATE_SLOT(d_func(), void _q_delayedRehighlight())
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_SYNTAXHIGHLIGHTER

#endif // QSYNTAXHIGHLIGHTER_H
