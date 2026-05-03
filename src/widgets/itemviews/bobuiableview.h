// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIABLEVIEW_H
#define BOBUIABLEVIEW_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qabstractitemview.h>

BOBUI_REQUIRE_CONFIG(tableview);

BOBUI_BEGIN_NAMESPACE

class QHeaderView;
class BOBUIableViewPrivate;

class Q_WIDGETS_EXPORT BOBUIableView : public QAbstractItemView
{
    Q_OBJECT
    Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid)
    Q_PROPERTY(BobUI::PenStyle gridStyle READ gridStyle WRITE setGridStyle)
    Q_PROPERTY(bool sortingEnabled READ isSortingEnabled WRITE setSortingEnabled)
    Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)
#if BOBUI_CONFIG(abstractbutton)
    Q_PROPERTY(bool cornerButtonEnabled READ isCornerButtonEnabled WRITE setCornerButtonEnabled)
#endif

public:
    explicit BOBUIableView(QWidget *parent = nullptr);
    ~BOBUIableView();

    void setModel(QAbstractItemModel *model) override;
    void setRootIndex(const QModelIndex &index) override;
    void setSelectionModel(QItemSelectionModel *selectionModel) override;
    void doItemsLayout() override;

    QHeaderView *horizontalHeader() const;
    QHeaderView *verticalHeader() const;
    void setHorizontalHeader(QHeaderView *header);
    void setVerticalHeader(QHeaderView *header);

    int rowViewportPosition(int row) const;
    int rowAt(int y) const;

    void setRowHeight(int row, int height);
    int rowHeight(int row) const;

    int columnViewportPosition(int column) const;
    int columnAt(int x) const;

    void setColumnWidth(int column, int width);
    int columnWidth(int column) const;

    bool isRowHidden(int row) const;
    void setRowHidden(int row, bool hide);

    bool isColumnHidden(int column) const;
    void setColumnHidden(int column, bool hide);

    void setSortingEnabled(bool enable);
    bool isSortingEnabled() const;

    bool showGrid() const;

    BobUI::PenStyle gridStyle() const;
    void setGridStyle(BobUI::PenStyle style);

    void setWordWrap(bool on);
    bool wordWrap() const;

#if BOBUI_CONFIG(abstractbutton)
    void setCornerButtonEnabled(bool enable);
    bool isCornerButtonEnabled() const;
#endif

    QRect visualRect(const QModelIndex &index) const override;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
    QModelIndex indexAt(const QPoint &p) const override;

    void setSpan(int row, int column, int rowSpan, int columnSpan);
    int rowSpan(int row, int column) const;
    int columnSpan(int row, int column) const;
    void clearSpans();


public Q_SLOTS:
    void selectRow(int row);
    void selectColumn(int column);
    void hideRow(int row);
    void hideColumn(int column);
    void showRow(int row);
    void showColumn(int column);
    void resizeRowToContents(int row);
    void resizeRowsToContents();
    void resizeColumnToContents(int column);
    void resizeColumnsToContents();
    void sortByColumn(int column, BobUI::SortOrder order);
    void setShowGrid(bool show);

protected Q_SLOTS:
    void rowMoved(int row, int oldIndex, int newIndex);
    void columnMoved(int column, int oldIndex, int newIndex);
    void rowResized(int row, int oldHeight, int newHeight);
    void columnResized(int column, int oldWidth, int newWidth);
    void rowCountChanged(int oldCount, int newCount);
    void columnCountChanged(int oldCount, int newCount);

protected:
    BOBUIableView(BOBUIableViewPrivate &, QWidget *parent);
    void scrollContentsBy(int dx, int dy) override;

    void initViewItemOption(QStyleOptionViewItem *option) const override;
    void paintEvent(QPaintEvent *e) override;

    void timerEvent(BOBUIimerEvent *event) override;

#if BOBUI_CONFIG(draganddrop)
    void dropEvent(QDropEvent *event) override;
#endif

    int horizontalOffset() const override;
    int verticalOffset() const override;
    QModelIndex moveCursor(CursorAction cursorAction, BobUI::KeyboardModifiers modifiers) override;

    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
    QRegion visualRegionForSelection(const QItemSelection &selection) const override;
    QModelIndexList selectedIndexes() const override;

    void updateGeometries() override;

    QSize viewportSizeHint() const override;

    int sizeHintForRow(int row) const override;
    int sizeHintForColumn(int column) const override;

    void verticalScrollbarAction(int action) override;
    void horizontalScrollbarAction(int action) override;

    bool isIndexHidden(const QModelIndex &index) const override;

    void selectionChanged(const QItemSelection &selected,
                          const QItemSelection &deselected) override;
    void currentChanged(const QModelIndex &current,
                          const QModelIndex &previous) override;

private:
    friend class QAccessibleItemView;
    int visualIndex(const QModelIndex &index) const;

    Q_DECLARE_PRIVATE(BOBUIableView)
    Q_DISABLE_COPY(BOBUIableView)
};

BOBUI_END_NAMESPACE

#endif // BOBUIABLEVIEW_H
