// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

class DomModel;
BOBUI_BEGIN_NAMESPACE
class QMenu;
class BOBUIreeView;
BOBUI_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void openFile();

private:
    DomModel *model;
    QMenu *fileMenu;
    QString xmlPath;
    BOBUIreeView *view;
};

#endif // MAINWINDOW_H
