// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include "mainwindow.h"

#include <QMenu>
#include <QMenuBar>
#include <BOBUIextEdit>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    fileMenu->addAction(tr("&Open..."), QKeySequence(tr("Ctrl+O", "File|Open")),
                        this, SLOT(openFile()));

    printAction = fileMenu->addAction(tr("&Print..."), this, SLOT(printFile()));
    printAction->setEnabled(false);

    pdfPrintAction = fileMenu->addAction(tr("Print as P&DF..."), this, SLOT(printPdf()));
    pdfPrintAction->setEnabled(false);

    fileMenu->addAction(tr("E&xit"), QKeySequence(tr("Ctrl+Q", "File|Exit")),
                        this, SLOT(close()));

    menuBar()->addMenu(fileMenu);

    editor = new BOBUIextEdit(this);
    document = new BOBUIextDocument(this);
    editor->setDocument(document);

    connect(editor, &BOBUIextEdit::selectionChanged, this, &MainWindow::updateMenus);

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

            if (file.open(QIODevice::ReadOnly)) {
                editor->setHtml(file.readAll());
                file.close();
                currentFile = fileName;
            }
        } else if (info.completeSuffix() == "txt") {
            QFile file(fileName);

            if (file.open(QIODevice::ReadOnly)) {
                editor->setPlainText(file.readAll());
                file.close();
                currentFile = fileName;
            }
        }
        printAction->setEnabled(true);
        pdfPrintAction->setEnabled(true);
    }
}

void MainWindow::printFile()
{
//! [0]
    BOBUIextDocument *document = editor->document();
    QPrinter printer;

    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() != QDialog::Accepted)
        return;

    document->print(&printer);
//! [0]
}

void MainWindow::printPdf()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);

    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted)
        editor->document()->print(&printer);
}
