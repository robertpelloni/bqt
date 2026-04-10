// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include "glwidget.h"

MainWindow::MainWindow()
{
    GLWidget *glwidget = new GLWidget(qRgb(20, 20, 50));
    setCentralWidget(glwidget);

    QMenu *screenShotMenu = menuBar()->addMenu("&Screenshot");
    screenShotMenu->addAction("Left buffer", this, [glwidget](){
        glwidget->saveImage(QOpenGLWidget::LeftBuffer);
    });

    screenShotMenu->addAction("Right buffer", this, [glwidget](){
        glwidget->saveImage(QOpenGLWidget::RightBuffer);
    });

    QMenu *helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("About BobUI", qApp, &QApplication::aboutBobUI);
}
