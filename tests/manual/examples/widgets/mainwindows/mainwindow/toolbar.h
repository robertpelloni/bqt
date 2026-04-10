// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <BOBUIoolBar>

BOBUI_FORWARD_DECLARE_CLASS(QAction)
BOBUI_FORWARD_DECLARE_CLASS(QActionGroup)
BOBUI_FORWARD_DECLARE_CLASS(QMenu)
BOBUI_FORWARD_DECLARE_CLASS(QSpinBox)

class ToolBar : public BOBUIoolBar
{
    Q_OBJECT

public:
    explicit ToolBar(const QString &title, QMainWindow *mainWindow);

    QMenu *toolbarMenu() const { return menu; }

private slots:
    void order();
    void randomize();
    void addSpinBox();
    void removeSpinBox();

    void changeMovable(bool movable);

    void allowLeft(bool a);
    void allowRight(bool a);
    void allowTop(bool a);
    void allowBottom(bool a);

    void placeLeft(bool p);
    void placeRight(bool p);
    void placeTop(bool p);
    void placeBottom(bool p);

    void updateMenu();
    void insertToolBarBreak();

private:
    void allow(BobUI::ToolBarArea area, bool allow);
    void place(BobUI::ToolBarArea area, bool place);

    QMainWindow *mainWindow;

    QSpinBox *spinbox;
    QAction *spinboxAction;

    QMenu *menu;
    QAction *orderAction;
    QAction *randomizeAction;
    QAction *addSpinBoxAction;
    QAction *removeSpinBoxAction;

    QAction *movableAction;

    QActionGroup *allowedAreasActions;
    QAction *allowLeftAction;
    QAction *allowRightAction;
    QAction *allowTopAction;
    QAction *allowBottomAction;

    QActionGroup *areaActions;
    QAction *leftAction;
    QAction *rightAction;
    QAction *topAction;
    QAction *bottomAction;
};

#endif // TOOLBAR_H
