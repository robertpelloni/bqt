// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE
class BOBUIextEdit;
BOBUI_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void about();
    void newFile();
    void openFile(const QString &path = QString());

private:
    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();

    BOBUIextEdit *editor;
    Highlighter *highlighter;
};
//! [0]

#endif // MAINWINDOW_H
