// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class BOBUIimer;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleOverrideCursor();

private:
    void keyPressEvent(QKeyEvent* event);

    Ui::MainWindow *ui;
    BOBUIimer *timer;
    int override;

    QCursor ccurs;
    QCursor bcurs;
};

#endif // MAINWINDOW_H
