// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

#include "../include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Dock Widgets");

    setupDockWindow();
    setupContents();
    setupMenus();

    textBrowser = new BOBUIextBrowser(this);

    connect(headingList, &QListWidget::itemClicked,
            this, &MainWindow::updateText);

    updateText(headingList->item(0));
    headingList->setCurrentRow(0);
    setCentralWidget(textBrowser);
}

void MainWindow::setupContents()
{
    QFile titlesFile(":/Resources/titles.txt");
    titlesFile.open(QFile::ReadOnly);
    int chapter = 0;

    do {
        QString line = titlesFile.readLine().trimmed();
        QStringList parts = line.split(u'\t', BobUI::SkipEmptyParts);
        if (parts.size() != 2)
            break;

        QString chapterTitle = parts[0];
        QString fileName = parts[1];

        QFile chapterFile(fileName);
        headingList = new QListWidget();

        chapterFile.open(QFile::ReadOnly);
        QListWidgetItem *item = new QListWidgetItem(chapterTitle, headingList);
        item->setData(BobUI::DisplayRole, chapterTitle);
        item->setData(BobUI::UserRole, chapterFile.readAll());
        item->setFlags(BobUI::ItemIsEnabled | BobUI::ItemIsSelectable);
        chapterFile.close();

        chapter++;
    } while (titlesFile.isOpen());

    titlesFile.close();
}

void MainWindow::setupDockWindow()
{
    //! [0]
    contentsWindow = new QDockWidget(tr("Table of Contents"), this);
    contentsWindow->setAllowedAreas(BobUI::LeftDockWidgetArea
                                  | BobUI::RightDockWidgetArea);
    addDockWidget(BobUI::LeftDockWidgetArea, contentsWindow);

    headingList = new QListWidget(contentsWindow);
    contentsWindow->setWidget(headingList);
    //! [0]
}

void MainWindow::setupMenus()
{
    QAction *exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, qApp, &QApplication::quit);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);
}

void MainWindow::updateText(QListWidgetItem *item)
{
    QString text = item->data(BobUI::UserRole).toString();
    textBrowser->setHtml(text);
}
