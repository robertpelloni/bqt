// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

BOBUI_FORWARD_DECLARE_CLASS(QSlider)
BOBUI_FORWARD_DECLARE_CLASS(QPushButton)

class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void dockUndock();

private:
    void dock();
    void undock();
    QSlider *createSlider();

    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
    QPushButton *dockBtn;
};

#endif
