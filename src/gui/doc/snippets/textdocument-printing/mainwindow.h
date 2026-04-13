// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
#include <QMainWindow>
#include <BOBUIextDocumentFragment>

BOBUI_BEGIN_NAMESPACE

class QAction;
class BOBUIextDocument;
class BOBUIextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void openFile();
    void printFile();
    void printPdf();
    void updateMenus();

private:
    QAction *printAction = nullptr;
    QAction *pdfPrintAction = nullptr;
    QString currentFile;
    BOBUIextEdit *editor = nullptr;
    BOBUIextDocument *document = nullptr;
};

BOBUI_END_NAMESPACE

#endif
