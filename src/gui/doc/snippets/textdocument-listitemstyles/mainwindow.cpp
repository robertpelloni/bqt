// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <BOBUIextEdit>

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    fileMenu->addAction(tr("E&xit"), QKeySequence(tr("Ctrl+Q", "File|Exit")),
                        this, SLOT(close()));

    QMenu *insertMenu = new QMenu(tr("&Insert"));

    insertMenu->addAction(tr("&List"), QKeySequence(tr("Ctrl+L", "Insert|List")),
                          this, SLOT(insertList()));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(insertMenu);

    editor = new BOBUIextEdit(this);
    document = new BOBUIextDocument(this);
    editor->setDocument(document);

    setCentralWidget(editor);
    setWindowTitle(tr("Text Document List Item Styles"));
}

void MainWindow::insertList()
{
    BOBUIextCursor cursor = editor->textCursor();
    cursor.beginEditBlock();

    //! [add a styled, ordered list]
    BOBUIextListFormat listFormat;

    listFormat.setStyle(BOBUIextListFormat::ListDecimal);
    listFormat.setNumberPrefix("(");
    listFormat.setNumberSuffix(")");

    cursor.insertList(listFormat);
    //! [add a styled, ordered list]

    cursor.endEditBlock();
}
