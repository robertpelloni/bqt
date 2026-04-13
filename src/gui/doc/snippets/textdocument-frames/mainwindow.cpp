// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    QAction *saveAction = fileMenu->addAction(tr("&Save..."));
    saveAction->setShortcut(tr("Ctrl+S"));

    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(tr("Ctrl+Q"));

    menuBar()->addMenu(fileMenu);
    editor = new BOBUIextEdit;

//! [rootframe]
    BOBUIextDocument *editorDocument = editor->document();
    BOBUIextFrame *root = editorDocument->rootFrame();
//! [rootframe]
    processFrame(root);

    BOBUIextCursor cursor(editor->textCursor());
    cursor.movePosition(BOBUIextCursor::Start);

    BOBUIextFrame *mainFrame = cursor.currentFrame();

    BOBUIextCharFormat plainCharFormat;
    BOBUIextCharFormat boldCharFormat;
    boldCharFormat.setFontWeight(QFont::Bold);
/*  main frame
//! [0]
    BOBUIextFrame *mainFrame = cursor.currentFrame();
    cursor.insertText(...);
//! [0]
*/
    cursor.insertText("Text documents are represented by the "
                      "BOBUIextDocument class, rather than by QString objects. "
                      "Each BOBUIextDocument object contains information about "
                      "the document's internal representation, its structure, "
                      "and keeps track of modifications to provide undo/redo "
                      "facilities. This approach allows features such as the "
                      "layout management to be delegated to specialized "
                      "classes, but also provides a focus for the framework.",
                      plainCharFormat);

//! [1]
    BOBUIextFrameFormat frameFormat;
    frameFormat.setMargin(32);
    frameFormat.setPadding(8);
    frameFormat.setBorder(4);
//! [1]
    cursor.insertFrame(frameFormat);

/*  insert frame
//! [2]
    cursor.insertFrame(frameFormat);
    cursor.insertText(...);
//! [2]
*/
    cursor.insertText("Documents are either converted from external sources "
                      "or created from scratch using BobUI. The creation process "
                      "can done by an editor widget, such as BOBUIextEdit, or by "
                      "explicit calls to the Scribe API.", boldCharFormat);

    cursor = mainFrame->lastCursorPosition();
/*  last cursor
//! [3]
    cursor = mainFrame->lastCursorPosition();
    cursor.insertText(...);
//! [3]
*/
    cursor.insertText("There are two complementary ways to visualize the "
                      "contents of a document: as a linear buffer that is "
                      "used by editors to modify the contents, and as an "
                      "object hierarchy containing structural information "
                      "that is useful to layout engines. In the hierarchical "
                      "model, the objects generally correspond to visual "
                      "elements such as frames, tables, and lists. At a lower "
                      "level, these elements describe properties such as the "
                      "style of text used and its alignment. The linear "
                      "representation of the document is used for editing and "
                      "manipulation of the document's contents.",
                      plainCharFormat);


    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);

    setCentralWidget(editor);
    setWindowTitle(tr("Text Document Frames"));
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save document as:"), "", tr("XML (*.xml)"));

    if (!fileName.isEmpty()) {
        if (writeXml(fileName))
            setWindowTitle(fileName);
        else
            QMessageBox::warning(this, tr("Warning"),
                tr("Failed to save the document."), QMessageBox::Cancel,
                QMessageBox::NoButton);
    }
}

void MainWindow::processBlock(BOBUIextBlock)
{
}

void MainWindow::processFrame(BOBUIextFrame *frame)
{
//! [4]
    BOBUIextFrame::iterator it;
    for (it = frame->begin(); !(it.atEnd()); ++it) {

        BOBUIextFrame *childFrame = it.currentFrame();
        BOBUIextBlock childBlock = it.currentBlock();

        if (childFrame)
            processFrame(childFrame);
        else if (childBlock.isValid())
            processBlock(childBlock);
    }
//! [4]
}
