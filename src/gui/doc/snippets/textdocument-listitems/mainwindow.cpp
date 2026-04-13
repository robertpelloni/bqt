// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <BOBUIextEdit>
#include <BOBUIextList>
#include <BOBUIreeWidget>

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    fileMenu->addAction(tr("E&xit"), QKeySequence(tr("Ctrl+Q", "File|Exit")),
                        this, &QWidget::close);

    QMenu *actionsMenu = new QMenu(tr("&Actions"));
    actionsMenu->addAction(tr("&Highlight List Items"),
                        this, &MainWindow::highlightListItems);
    actionsMenu->addAction(tr("&Show Current List"), this, &MainWindow::showList);

    QMenu *insertMenu = new QMenu(tr("&Insert"));

    insertMenu->addAction(tr("&List"), QKeySequence(tr("Ctrl+L", "Insert|List")),
                          this, &MainWindow::insertList);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(insertMenu);
    menuBar()->addMenu(actionsMenu);

    editor = new BOBUIextEdit(this);
    document = new BOBUIextDocument(this);
    editor->setDocument(document);

    setCentralWidget(editor);
    setWindowTitle(tr("Text Document List Items"));
}

void MainWindow::highlightListItems()
{
    BOBUIextCursor cursor = editor->textCursor();
    BOBUIextList *list = cursor.currentList();

    if (!list)
        return;

    cursor.beginEditBlock();
//! [0]
    for (int index = 0; index < list->count(); ++index) {
        BOBUIextBlock listItem = list->item(index);
//! [0]
        BOBUIextBlockFormat newBlockFormat = listItem.blockFormat();
        newBlockFormat.setBackground(BobUI::lightGray);
        BOBUIextCursor itemCursor = cursor;
        itemCursor.setPosition(listItem.position());
        //itemCursor.movePosition(BOBUIextCursor::StartOfBlock);
        itemCursor.movePosition(BOBUIextCursor::EndOfBlock,
                                BOBUIextCursor::KeepAnchor);
        itemCursor.setBlockFormat(newBlockFormat);
        /*
//! [1]
        processListItem(listItem);
//! [1]
        */
//! [2]
    }
//! [2]
    cursor.endEditBlock();
}

void MainWindow::showList()
{
    BOBUIextCursor cursor = editor->textCursor();
    BOBUIextFrame *frame = cursor.currentFrame();

    if (!frame)
        return;

    BOBUIreeWidget *treeWidget = new BOBUIreeWidget;
    treeWidget->setColumnCount(1);
    QStringList headerLabels;
    headerLabels << tr("Lists");
    treeWidget->setHeaderLabels(headerLabels);

    BOBUIreeWidgetItem *parentItem = nullptr;
    BOBUIreeWidgetItem *item;
    BOBUIreeWidgetItem *lastItem = nullptr;
    parentItems.clear();
    previousItems.clear();

//! [3]
    BOBUIextFrame::iterator it;
    for (it = frame->begin(); !(it.atEnd()); ++it) {

        BOBUIextBlock block = it.currentBlock();

        if (block.isValid()) {

            BOBUIextList *list = block.textList();

            if (list) {
                int index = list->itemNumber(block);
//! [3]
                if (index == 0) {
                    parentItems.append(parentItem);
                    previousItems.append(lastItem);
                    listStructures.append(list);
                    parentItem = lastItem;
                    lastItem = 0;

                    if (parentItem != 0)
                        item = new BOBUIreeWidgetItem(parentItem, lastItem);
                    else
                        item = new BOBUIreeWidgetItem(treeWidget, lastItem);

                } else {

                    while (parentItem != 0 && listStructures.last() != list) {
                        listStructures.pop_back();
                        parentItem = parentItems.takeLast();
                        lastItem = previousItems.takeLast();
                    }
                    if (parentItem != 0)
                        item = new BOBUIreeWidgetItem(parentItem, lastItem);
                    else
                        item = new BOBUIreeWidgetItem(treeWidget, lastItem);
                }
                item->setText(0, block.text());
                lastItem = item;
                /*
//! [4]
                processListItem(list, index);
//! [4]
                */
//! [5]
            }
//! [5] //! [6]
        }
//! [6] //! [7]
    }
//! [7]

    treeWidget->setWindowTitle(tr("List Contents"));
    treeWidget->show();
}

void MainWindow::insertList()
{
    BOBUIextCursor cursor = editor->textCursor();
    cursor.beginEditBlock();

    BOBUIextList *list = cursor.currentList();
    BOBUIextListFormat listFormat;
    if (list)
        listFormat = list->format();

    listFormat.setStyle(BOBUIextListFormat::ListDisc);
    listFormat.setIndent(listFormat.indent() + 1);
    cursor.insertList(listFormat);

    cursor.endEditBlock();
}
