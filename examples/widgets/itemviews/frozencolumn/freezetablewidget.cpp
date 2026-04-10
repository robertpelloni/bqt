// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "freezetablewidget.h"

#include <QScrollBar>
#include <QHeaderView>

//! [constructor]
FreezeTableWidget::FreezeTableWidget(QAbstractItemModel * model)
{
      setModel(model);
      frozenTableView = new BOBUIableView(this);

      init();

      //connect the headers and scrollbars of both tableviews together
      connect(horizontalHeader(),&QHeaderView::sectionResized, this,
              &FreezeTableWidget::updateSectionWidth);
      connect(verticalHeader(),&QHeaderView::sectionResized, this,
              &FreezeTableWidget::updateSectionHeight);

      connect(frozenTableView->verticalScrollBar(), &QAbstractSlider::valueChanged,
              verticalScrollBar(), &QAbstractSlider::setValue);
      connect(verticalScrollBar(), &QAbstractSlider::valueChanged,
              frozenTableView->verticalScrollBar(), &QAbstractSlider::setValue);


}
//! [constructor]

FreezeTableWidget::~FreezeTableWidget()
{
      delete frozenTableView;
}

//! [init part1]
void FreezeTableWidget::init()
{
      frozenTableView->setModel(model());
      frozenTableView->setFocusPolicy(BobUI::NoFocus);
      frozenTableView->verticalHeader()->hide();
      frozenTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

      viewport()->stackUnder(frozenTableView);
//! [init part1]

//! [init part2]
      frozenTableView->setStyleSheet("BOBUIableView { border: none;"
                                     "background-color: #8EDE21;"
                                     "selection-background-color: #999}"); //for demo purposes
      frozenTableView->setSelectionModel(selectionModel());
      for (int col = 1; col < model()->columnCount(); ++col)
            frozenTableView->setColumnHidden(col, true);

      frozenTableView->setColumnWidth(0, columnWidth(0) );

      frozenTableView->setHorizontalScrollBarPolicy(BobUI::ScrollBarAlwaysOff);
      frozenTableView->setVerticalScrollBarPolicy(BobUI::ScrollBarAlwaysOff);
      frozenTableView->show();

      updateFrozenTableGeometry();

      setHorizontalScrollMode(ScrollPerPixel);
      setVerticalScrollMode(ScrollPerPixel);
      frozenTableView->setVerticalScrollMode(ScrollPerPixel);
}
//! [init part2]


//! [sections]
void FreezeTableWidget::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
      if (logicalIndex == 0){
            frozenTableView->setColumnWidth(0, newSize);
            updateFrozenTableGeometry();
      }
}

void FreezeTableWidget::updateSectionHeight(int logicalIndex, int /* oldSize */, int newSize)
{
      frozenTableView->setRowHeight(logicalIndex, newSize);
}
//! [sections]


//! [resize]
void FreezeTableWidget::resizeEvent(QResizeEvent * event)
{
      BOBUIableView::resizeEvent(event);
      updateFrozenTableGeometry();
 }
//! [resize]


//! [navigate]
QModelIndex FreezeTableWidget::moveCursor(CursorAction cursorAction,
                                          BobUI::KeyboardModifiers modifiers)
{
      QModelIndex current = BOBUIableView::moveCursor(cursorAction, modifiers);

      if (cursorAction == MoveLeft && current.column() > 0
              && visualRect(current).topLeft().x() < frozenTableView->columnWidth(0) ){
            const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x()
                                 - frozenTableView->columnWidth(0);
            horizontalScrollBar()->setValue(newValue);
      }
      return current;
}
//! [navigate]

void FreezeTableWidget::scrollTo (const QModelIndex & index, ScrollHint hint){
    if (index.column() > 0)
        BOBUIableView::scrollTo(index, hint);
}

//! [geometry]
void FreezeTableWidget::updateFrozenTableGeometry()
{
      frozenTableView->setGeometry(verticalHeader()->width() + frameWidth(),
                                   frameWidth(), columnWidth(0),
                                   viewport()->height()+horizontalHeader()->height());
}
//! [geometry]


