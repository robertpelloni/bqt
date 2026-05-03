// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE
class BOBUIreeWidget;
BOBUI_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void open();
    void saveAs();
    void about();
#if BOBUI_CONFIG(clipboard) && BOBUI_CONFIG(contextmenu)
    void onCustomContextMenuRequested(const QPoint &pos);
#endif
private:
    void createMenus();

    BOBUIreeWidget *const treeWidget;
};
//! [0]

#endif
