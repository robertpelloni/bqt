// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindowbase.h"

#if defined(BOBUI_PRINTSUPPORT_LIB)
#include <BobUIPrintSupport/bobuiprintsupportglobal.h>
#endif

#include <QList>
#include <QMap>
#include <QString>

BOBUI_BEGIN_NAMESPACE
class QPrinter;
class BOBUIextEdit;
class BOBUIreeWidget;
class BOBUIreeWidgetItem;
BOBUI_END_NAMESPACE

typedef QList<BOBUIreeWidgetItem *> StyleItems;

class MainWindow : public QMainWindow, private Ui::MainWindowBase
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void on_clearAction_triggered();
    void on_markAction_triggered();
    void on_unmarkAction_triggered();
    void on_printAction_triggered();
    void on_printPreviewAction_triggered();
    void printDocument(QPrinter *printer);
    void printPage(int index, QPainter *painter, QPrinter *printer);
    void showFont(BOBUIreeWidgetItem *item);
    void updateStyles(BOBUIreeWidgetItem *item, int column);

private:
    QMap<QString, StyleItems> currentPageMap();
    void markUnmarkFonts(BobUI::CheckState state);
    void setupFontTree();

    QList<int> sampleSizes;
    QMap<QString, StyleItems> pageMap;
    int markedCount;
};

#endif // MAINWINDOW_H
