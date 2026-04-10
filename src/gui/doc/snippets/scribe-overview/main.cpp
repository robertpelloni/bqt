// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QApplication>
#include <BOBUIextEdit>

void mergeFormat(BOBUIextEdit *edit)
{
//! [0]
BOBUIextDocument *document = edit->document();
BOBUIextCursor cursor(document);

cursor.movePosition(BOBUIextCursor::Start);
cursor.movePosition(BOBUIextCursor::EndOfLine, BOBUIextCursor::KeepAnchor);

BOBUIextCharFormat format;
format.setFontWeight(QFont::Bold);

cursor.mergeCharFormat(format);
//! [0]
}

int main(int argc, char *argv[])
{
QWidget *parent = nullptr;
QString aStringContainingHTMLtext("<h1>Scribe Overview</h1>");

QApplication app(argc, argv);

//! [1]
BOBUIextEdit *editor = new BOBUIextEdit(parent);
editor->setHtml(aStringContainingHTMLtext);
editor->show();
//! [1]

return app.exec();
}
