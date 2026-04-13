// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef FREEZETABLEWIDGET_H
#define FREEZETABLEWIDGET_H

#include <BOBUIableView>

//! [Widget definition]
class FreezeTableWidget : public BOBUIableView {
     Q_OBJECT

public:
      FreezeTableWidget(QAbstractItemModel * model);
      ~FreezeTableWidget();


protected:
      void resizeEvent(QResizeEvent *event) override;
      QModelIndex moveCursor(CursorAction cursorAction, BobUI::KeyboardModifiers modifiers) override;
      void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible) override;

private:
      BOBUIableView *frozenTableView;
      void init();
      void updateFrozenTableGeometry();


private slots:
      void updateSectionWidth(int logicalIndex, int oldSize, int newSize);
      void updateSectionHeight(int logicalIndex, int oldSize, int newSize);

};
//! [Widget definition]
#endif // FREEZETABLEWIDGET_H
