// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QApplication>
#include <BOBUIextEdit>
#include <BOBUIextTable>

int main(int argc, char * argv[])
{
    int rows = 6;
    int columns = 2;

    QApplication app(argc, argv);
    BOBUIextEdit *textEdit = new BOBUIextEdit;
    BOBUIextCursor cursor(textEdit->textCursor());
    cursor.movePosition(BOBUIextCursor::Start);

    BOBUIextTableFormat tableFormat;
    tableFormat.setAlignment(BobUI::AlignHCenter);
    tableFormat.setCellPadding(2);
    tableFormat.setCellSpacing(2);
    BOBUIextTable *table = cursor.insertTable(rows, columns);
    table->setFormat(tableFormat);

    BOBUIextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);

    BOBUIextBlockFormat centerFormat;
    centerFormat.setAlignment(BobUI::AlignHCenter);
    cursor.mergeBlockFormat(centerFormat);

    cursor = table->cellAt(0, 0).firstCursorPosition();
    cursor.insertText(("Details"), boldFormat);

    cursor = table->cellAt(1, 0).firstCursorPosition();
    cursor.insertText("Alan");

    cursor = table->cellAt(1, 1).firstCursorPosition();
    cursor.insertText("5, Pickety Street");

//! [0]
    table->mergeCells(0, 0, 1, 2);
//! [0] //! [1]
    table->splitCell(0, 0, 1, 1);
//! [1]

    textEdit->show();
    return app.exec();
}
