// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE
class QColorDialog;
BOBUI_END_NAMESPACE
class TabletCanvas;

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(TabletCanvas *canvas);

private slots:
    void setBrushColor();
    void setAlphaValuator(QAction *action);
    void setLineWidthValuator(QAction *action);
    void setSaturationValuator(QAction *action);
    void setEventCompression(bool compress);
    bool save();
    void load();
    void clear();
    void about();

private:
    void createMenus();

    TabletCanvas *m_canvas;
    QColorDialog *m_colorDialog = nullptr;
};
//! [0]

#endif
