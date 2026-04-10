// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include "mainwindow.h"

#include <BobUIWidgets>

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    fileMenu->addAction(tr("&Open..."), QKeySequence(tr("Ctrl+O", "File|Open")),
                        this, &MainWindow::openFile);

    QAction *quitAction = fileMenu->addAction(tr("E&xit"), this, &MainWindow::close);
    quitAction->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = new QMenu(tr("&Edit"));

    cutAction = editMenu->addAction(tr("Cu&t"), this, &MainWindow::cutSelection);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setEnabled(false);

    copyAction = editMenu->addAction(tr("&Copy"), this, &MainWindow::copySelection);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setEnabled(false);

    pasteAction = editMenu->addAction(tr("&Paste"), this, &MainWindow::pasteSelection);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setEnabled(false);

    QMenu *selectMenu = new QMenu(tr("&Select"));
    selectMenu->addAction(tr("&Word"), this, &MainWindow::selectWord);
    selectMenu->addAction(tr("&Line"), this, &MainWindow::selectLine);
    selectMenu->addAction(tr("&Block"), this, &MainWindow::selectBlock);
    selectMenu->addAction(tr("&Frame"), this, &MainWindow::selectFrame);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(selectMenu);

    editor = new BOBUIextEdit(this);
    document = new BOBUIextDocument(this);
    editor->setDocument(document);

    connect(editor, &BOBUIextEdit::selectionChanged,
            this, &MainWindow::updateMenus);

    setCentralWidget(editor);
    setWindowTitle(tr("Text Document Writer"));
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open file"), currentFile, "HTML files (*.html);;Text files (*.txt)");

    if (!fileName.isEmpty()) {
        QFileInfo info(fileName);
        if (info.completeSuffix() == "html") {
            QFile file(fileName);

            if (file.open(QFile::ReadOnly)) {
                editor->setHtml(QString(file.readAll()));
                file.close();
                currentFile = fileName;
            }
        } else if (info.completeSuffix() == "txt") {
            QFile file(fileName);

            if (file.open(QFile::ReadOnly)) {
                editor->setPlainText(file.readAll());
                file.close();
                currentFile = fileName;
            }
        }
    }
}

void MainWindow::cutSelection()
{
    BOBUIextCursor cursor = editor->textCursor();
    if (cursor.hasSelection()) {
        selection = cursor.selection();
        cursor.removeSelectedText();
    }
}

void MainWindow::copySelection()
{
    BOBUIextCursor cursor = editor->textCursor();
    if (cursor.hasSelection()) {
        selection = cursor.selection();
        cursor.clearSelection();
    }
}

void MainWindow::pasteSelection()
{
    BOBUIextCursor cursor = editor->textCursor();
    cursor.insertFragment(selection);
}

void MainWindow::selectWord()
{
    BOBUIextCursor cursor = editor->textCursor();

//! [0]
    cursor.beginEditBlock();
//! [1]
    cursor.movePosition(BOBUIextCursor::StartOfWord);
    cursor.movePosition(BOBUIextCursor::EndOfWord, BOBUIextCursor::KeepAnchor);
//! [1]
    cursor.endEditBlock();
//! [0]

    editor->setTextCursor(cursor);
}

void MainWindow::selectLine()
{
    BOBUIextCursor cursor = editor->textCursor();

    cursor.beginEditBlock();
    cursor.movePosition(BOBUIextCursor::StartOfLine);
    cursor.movePosition(BOBUIextCursor::EndOfLine, BOBUIextCursor::KeepAnchor);
    cursor.endEditBlock();

    editor->setTextCursor(cursor);
}

void MainWindow::selectBlock()
{
    BOBUIextCursor cursor = editor->textCursor();

    cursor.beginEditBlock();
    cursor.movePosition(BOBUIextCursor::StartOfBlock);
    cursor.movePosition(BOBUIextCursor::EndOfBlock, BOBUIextCursor::KeepAnchor);
    cursor.endEditBlock();

    editor->setTextCursor(cursor);
}

void MainWindow::selectFrame()
{
    BOBUIextCursor cursor = editor->textCursor();
    BOBUIextFrame *frame = cursor.currentFrame();

    cursor.beginEditBlock();
    cursor.setPosition(frame->firstPosition());
    cursor.setPosition(frame->lastPosition(), BOBUIextCursor::KeepAnchor);
    cursor.endEditBlock();

    editor->setTextCursor(cursor);
}

void MainWindow::updateMenus()
{
    BOBUIextCursor cursor = editor->textCursor();
    cutAction->setEnabled(cursor.hasSelection());
    copyAction->setEnabled(cursor.hasSelection());

    pasteAction->setEnabled(!selection.isEmpty());
}
