// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
BOBUI_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
#ifndef BOBUI_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // BOBUI_NO_CONTEXTMENU
//! [0]

//! [1]
private slots:
    void newFile();
    void open();
    void save();
    void print();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold();
    void italic();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutBobUI();
//! [1]

//! [2]
private:
    void createActions();
    void createMenus();
//! [2]

//! [3]
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct;
    QAction *aboutAct;
    QAction *aboutBobUIAct;
    QLabel *infoLabel;
};
//! [3]

#endif
