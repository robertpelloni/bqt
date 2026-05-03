// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE
class QAction;
class BOBUIoolBar;
class QMenu;
class QUndoStack;
class QUndoView;
BOBUI_END_NAMESPACE
class DiagramScene;
class DiagramItem;

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void itemMoved(DiagramItem *movedDiagram, const QPointF &moveStartPosition);

private slots:
    void deleteItem();
    void addBox();
    void addTriangle();
    void about();
    void updateActions();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createUndoView();

    QAction *deleteAction = nullptr;
    QAction *addBoxAction = nullptr;
    QAction *addTriangleAction = nullptr;
    QAction *undoAction = nullptr;
    QAction *redoAction = nullptr;
    QAction *exitAction = nullptr;
    QAction *aboutAction = nullptr;

    QMenu *fileMenu = nullptr;
    QMenu *editMenu = nullptr;
    QMenu *itemMenu = nullptr;
    QMenu *helpMenu = nullptr;

    DiagramScene *diagramScene = nullptr;
    QUndoStack *undoStack = nullptr;
    QUndoView *undoView = nullptr;
};
//! [0]

#endif
