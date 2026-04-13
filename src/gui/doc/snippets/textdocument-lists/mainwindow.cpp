// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <BOBUIextEdit>
#include <BOBUIextList>

namespace textdocument_lists {
struct MainWindow
{
    void insertList();

private:
    BOBUIextEdit *editor = nullptr;
};

void MainWindow::insertList()
{
    BOBUIextCursor cursor = editor->textCursor();
    BOBUIextList *list = cursor.currentList();

//! [0]
    BOBUIextListFormat listFormat;
    if (list) {
        listFormat = list->format();
        listFormat.setIndent(listFormat.indent() + 1);
    }

    listFormat.setStyle(BOBUIextListFormat::ListDisc);
    cursor.insertList(listFormat);
//! [0]
}

} //textdocument_lists
