// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
BOBUI_END_NAMESPACE

class QGraphicsEllipseItem;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QGraphicsEllipseItem *m_ellipse;
    QLabel *m_label;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
