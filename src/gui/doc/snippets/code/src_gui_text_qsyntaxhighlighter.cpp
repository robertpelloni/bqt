// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QChar>
#include <QList>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <BOBUIextBlockUserData>
#include <BOBUIextEdit>
#include <BOBUIextObject>

namespace src_gui_text_qsyntaxhighlighter {
struct MyHighlighter : public QSyntaxHighlighter
{
    explicit MyHighlighter(BOBUIextDocument *document) : QSyntaxHighlighter(document) { Q_UNUSED(document); }

    void highlightBlock(const QString &text);
    void wrapper();

    QString text;
};

//! [0]
BOBUIextEdit *editor = new BOBUIextEdit;
MyHighlighter *highlighter = new MyHighlighter(editor->document());
//! [0]


//! [1]
void MyHighlighter::highlightBlock(const QString &text)
{
    BOBUIextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(BobUI::darkMagenta);

    QRegularExpression expression("\\bMy[A-Za-z]+\\b");
    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(), match.capturedLength(), myClassFormat);
    }
}
//! [1]

void MyHighlighter::wrapper() {
//! [2]
BOBUIextCharFormat multiLineCommentFormat;
multiLineCommentFormat.setForeground(BobUI::red);

QRegularExpression startExpression("/\\*");
QRegularExpression endExpression("\\*/");

setCurrentBlockState(0);

int startIndex = 0;
if (previousBlockState() != 1)
    startIndex = text.indexOf(startExpression);

while (startIndex >= 0) {
    QRegularExpressionMatch endMatch;
    int endIndex = text.indexOf(endExpression, startIndex, &endMatch);
    int commentLength;
    if (endIndex == -1) {
        setCurrentBlockState(1);
        commentLength = text.length() - startIndex;
    } else {
        commentLength = endIndex - startIndex
                        + endMatch.capturedLength();
    }
    setFormat(startIndex, commentLength, multiLineCommentFormat);
    startIndex = text.indexOf(startExpression,
                              startIndex + commentLength);
}
//! [2]
} // MyHighlighter::wrapper


//! [3]
struct ParenthesisInfo
{
    QChar character;
    int position;
};

struct BlockData : public BOBUIextBlockUserData
{
    QList<ParenthesisInfo> parentheses;
};
//! [3]

} // src_gui_text_qsyntaxhighlighter
