// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "toolbar.h"

#include <QRandomGenerator>

#include <QActionGroup>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPainterPath>
#include <QSpinBox>
#include <QLabel>
#include <BOBUIoolTip>

#include <stdlib.h>

static QPixmap genIcon(const QSize &iconSize, const QString &, const QColor &color, qreal pixelRatio)
{
    int w = qRound(iconSize.width() * pixelRatio);
    int h = qRound(iconSize.height() * pixelRatio);

    QImage image(w, h, QImage::Format_ARGB32_Premultiplied);
    image.fill(0);

    QPainter p(&image);

    extern void render_bobui_text(QPainter *, int, int, const QColor &);
    render_bobui_text(&p, w, h, color);

    QPixmap pm = QPixmap::fromImage(image, BobUI::DiffuseDither | BobUI::DiffuseAlphaDither);
    pm.setDevicePixelRatio(pixelRatio);
    return pm;
}

static QPixmap genIcon(const QSize &iconSize, int number, const QColor &color, qreal pixelRatio)
{ return genIcon(iconSize, QString::number(number), color, pixelRatio); }

ToolBar::ToolBar(const QString &title, QMainWindow *mainWindow)
    : BOBUIoolBar(mainWindow)
    , mainWindow(mainWindow)
    , spinbox(nullptr)
    , spinboxAction(nullptr)
{
    setWindowTitle(title);
    setObjectName(title);

    setIconSize(QSize(32, 32));

    qreal dpr = devicePixelRatio();
    menu = new QMenu("One", this);
    menu->setIcon(genIcon(iconSize(), 1, BobUI::black, dpr));
    menu->addAction(genIcon(iconSize(), "A", BobUI::blue, dpr), "A");
    menu->addAction(genIcon(iconSize(), "B", BobUI::blue, dpr), "B");
    menu->addAction(genIcon(iconSize(), "C", BobUI::blue, dpr), "C");
    addAction(menu->menuAction());

    QAction *two = addAction(genIcon(iconSize(), 2, BobUI::white, dpr), "Two");
    QFont boldFont;
    boldFont.setBold(true);
    two->setFont(boldFont);

    addAction(genIcon(iconSize(), 3, BobUI::red, dpr), "Three");
    addAction(genIcon(iconSize(), 4, BobUI::green, dpr), "Four");
    addAction(genIcon(iconSize(), 5, BobUI::blue, dpr), "Five");
    addAction(genIcon(iconSize(), 6, BobUI::yellow, dpr), "Six");
    orderAction = new QAction(this);
    orderAction->setText(tr("Order Items in Tool Bar"));
    connect(orderAction, &QAction::triggered, this, &ToolBar::order);

    randomizeAction = new QAction(this);
    randomizeAction->setText(tr("Randomize Items in Tool Bar"));
    connect(randomizeAction, &QAction::triggered, this, &ToolBar::randomize);

    addSpinBoxAction = new QAction(this);
    addSpinBoxAction->setText(tr("Add Spin Box"));
    connect(addSpinBoxAction, &QAction::triggered, this, &ToolBar::addSpinBox);

    removeSpinBoxAction = new QAction(this);
    removeSpinBoxAction->setText(tr("Remove Spin Box"));
    removeSpinBoxAction->setEnabled(false);
    connect(removeSpinBoxAction, &QAction::triggered, this, &ToolBar::removeSpinBox);

    movableAction = new QAction(tr("Movable"), this);
    movableAction->setCheckable(true);
    connect(movableAction, &QAction::triggered, this, &ToolBar::changeMovable);

    allowedAreasActions = new QActionGroup(this);
    allowedAreasActions->setExclusive(false);

    allowLeftAction = new QAction(tr("Allow on Left"), this);
    allowLeftAction->setCheckable(true);
    connect(allowLeftAction, &QAction::triggered, this, &ToolBar::allowLeft);

    allowRightAction = new QAction(tr("Allow on Right"), this);
    allowRightAction->setCheckable(true);
    connect(allowRightAction, &QAction::triggered, this, &ToolBar::allowRight);

    allowTopAction = new QAction(tr("Allow on Top"), this);
    allowTopAction->setCheckable(true);
    connect(allowTopAction, &QAction::triggered, this, &ToolBar::allowTop);

    allowBottomAction = new QAction(tr("Allow on Bottom"), this);
    allowBottomAction->setCheckable(true);
    connect(allowBottomAction, &QAction::triggered, this, &ToolBar::allowBottom);

    allowedAreasActions->addAction(allowLeftAction);
    allowedAreasActions->addAction(allowRightAction);
    allowedAreasActions->addAction(allowTopAction);
    allowedAreasActions->addAction(allowBottomAction);

    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);

    leftAction = new QAction(tr("Place on Left") , this);
    leftAction->setCheckable(true);
    connect(leftAction, &QAction::triggered, this, &ToolBar::placeLeft);

    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, &QAction::triggered, this, &ToolBar::placeRight);

    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, &QAction::triggered, this, &ToolBar::placeTop);

    bottomAction = new QAction(tr("Place on Bottom") , this);
    bottomAction->setCheckable(true);
    connect(bottomAction, &QAction::triggered, this, &ToolBar::placeBottom);

    areaActions->addAction(leftAction);
    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);
    areaActions->addAction(bottomAction);

    connect(movableAction, &QAction::triggered, areaActions, &QActionGroup::setEnabled);

    connect(movableAction, &QAction::triggered, allowedAreasActions, &QActionGroup::setEnabled);

    menu = new QMenu(title, this);
    menu->addAction(toggleViewAction());
    menu->addSeparator();
    menu->addAction(orderAction);
    menu->addAction(randomizeAction);
    menu->addSeparator();
    menu->addAction(addSpinBoxAction);
    menu->addAction(removeSpinBoxAction);
    menu->addSeparator();
    menu->addAction(movableAction);
    menu->addSeparator();
    menu->addActions(allowedAreasActions->actions());
    menu->addSeparator();
    menu->addActions(areaActions->actions());
    menu->addSeparator();
    menu->addAction(tr("Insert break"), this, &ToolBar::insertToolBarBreak);

    connect(menu, &QMenu::aboutToShow, this, &ToolBar::updateMenu);

    randomize();
}

void ToolBar::updateMenu()
{
    const BobUI::ToolBarArea area = mainWindow->toolBarArea(this);
    const BobUI::ToolBarAreas areas = allowedAreas();

    movableAction->setChecked(isMovable());

    allowLeftAction->setChecked(isAreaAllowed(BobUI::LeftToolBarArea));
    allowRightAction->setChecked(isAreaAllowed(BobUI::RightToolBarArea));
    allowTopAction->setChecked(isAreaAllowed(BobUI::TopToolBarArea));
    allowBottomAction->setChecked(isAreaAllowed(BobUI::BottomToolBarArea));

    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != BobUI::LeftToolBarArea);
        allowRightAction->setEnabled(area != BobUI::RightToolBarArea);
        allowTopAction->setEnabled(area != BobUI::TopToolBarArea);
        allowBottomAction->setEnabled(area != BobUI::BottomToolBarArea);
    }

    leftAction->setChecked(area == BobUI::LeftToolBarArea);
    rightAction->setChecked(area == BobUI::RightToolBarArea);
    topAction->setChecked(area == BobUI::TopToolBarArea);
    bottomAction->setChecked(area == BobUI::BottomToolBarArea);

    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & BobUI::LeftToolBarArea);
        rightAction->setEnabled(areas & BobUI::RightToolBarArea);
        topAction->setEnabled(areas & BobUI::TopToolBarArea);
        bottomAction->setEnabled(areas & BobUI::BottomToolBarArea);
    }
}

void ToolBar::order()
{
    QList<QAction *> ordered;
    QList<QAction *> actions1 = actions();
    const QList<QAction *> childActions = findChildren<QAction *>();
    for (QAction *action : childActions) {
        if (!actions1.contains(action))
            continue;
        actions1.removeAll(action);
        ordered.append(action);
    }

    clear();
    addActions(ordered);

    orderAction->setEnabled(false);
}

void ToolBar::randomize()
{
    QList<QAction *> randomized;
    QList<QAction *> actions = this->actions();
    while (!actions.isEmpty()) {
        QAction *action = actions.takeAt(QRandomGenerator::global()->bounded(actions.size()));
        randomized.append(action);
    }
    clear();
    addActions(randomized);

    orderAction->setEnabled(true);
}

void ToolBar::addSpinBox()
{
    if (!spinbox)
        spinbox = new QSpinBox(this);
    if (!spinboxAction)
        spinboxAction = addWidget(spinbox);
    else
        addAction(spinboxAction);

    addSpinBoxAction->setEnabled(false);
    removeSpinBoxAction->setEnabled(true);
}

void ToolBar::removeSpinBox()
{
    if (spinboxAction)
        removeAction(spinboxAction);

    addSpinBoxAction->setEnabled(true);
    removeSpinBoxAction->setEnabled(false);
}

void ToolBar::allow(BobUI::ToolBarArea area, bool a)
{
    BobUI::ToolBarAreas areas = allowedAreas();
    areas = a ? areas | area : areas & ~area;
    setAllowedAreas(areas);

    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & BobUI::LeftToolBarArea);
        rightAction->setEnabled(areas & BobUI::RightToolBarArea);
        topAction->setEnabled(areas & BobUI::TopToolBarArea);
        bottomAction->setEnabled(areas & BobUI::BottomToolBarArea);
    }
}

void ToolBar::place(BobUI::ToolBarArea area, bool p)
{
    if (!p)
        return;

    mainWindow->addToolBar(area, this);

    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != BobUI::LeftToolBarArea);
        allowRightAction->setEnabled(area != BobUI::RightToolBarArea);
        allowTopAction->setEnabled(area != BobUI::TopToolBarArea);
        allowBottomAction->setEnabled(area != BobUI::BottomToolBarArea);
    }
}

void ToolBar::changeMovable(bool movable)
{ setMovable(movable); }

void ToolBar::allowLeft(bool a)
{ allow(BobUI::LeftToolBarArea, a); }

void ToolBar::allowRight(bool a)
{ allow(BobUI::RightToolBarArea, a); }

void ToolBar::allowTop(bool a)
{ allow(BobUI::TopToolBarArea, a); }

void ToolBar::allowBottom(bool a)
{ allow(BobUI::BottomToolBarArea, a); }

void ToolBar::placeLeft(bool p)
{ place(BobUI::LeftToolBarArea, p); }

void ToolBar::placeRight(bool p)
{ place(BobUI::RightToolBarArea, p); }

void ToolBar::placeTop(bool p)
{ place(BobUI::TopToolBarArea, p); }

void ToolBar::placeBottom(bool p)
{ place(BobUI::BottomToolBarArea, p); }

void ToolBar::insertToolBarBreak()
{
    mainWindow->insertToolBarBreak(this);
}
