// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <BobUIGui/qwidget.h>

BOBUI_FORWARD_DECLARE_CLASS(QGraphicsScene)
BOBUI_FORWARD_DECLARE_CLASS(QGraphicsView)
BOBUI_FORWARD_DECLARE_CLASS(QLabel)
BOBUI_FORWARD_DECLARE_CLASS(QSlider)
BOBUI_FORWARD_DECLARE_CLASS(QSplitter)

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    void setupMatrix();
    void populateScene();

    QGraphicsScene *scene;
};

#endif
