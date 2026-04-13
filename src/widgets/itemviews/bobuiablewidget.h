// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIABLEWIDGET_H
#define BOBUIABLEWIDGET_H

#include <BobUIWidgets/bobuiableview.h>
#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qvariant.h>

BOBUI_REQUIRE_CONFIG(tablewidget);

BOBUI_BEGIN_NAMESPACE

class BOBUIableWidgetSelectionRange
{
public:
    BOBUIableWidgetSelectionRange() = default;
    BOBUIableWidgetSelectionRange(int top, int left, int bottom, int right)
      : m_top(top), m_left(left), m_bottom(bottom), m_right(right)
    {}

    friend bool operator==(const BOBUIableWidgetSelectionRange &lhs,
                           const BOBUIableWidgetSelectionRange &rhs) noexcept
    {
        return lhs.m_top == rhs.m_top && lhs.m_left == rhs.m_left && lhs.m_bottom == rhs.m_bottom
                && lhs.m_right == rhs.m_right;
    }
    friend bool operator!=(const BOBUIableWidgetSelectionRange &lhs,
                           const BOBUIableWidgetSelectionRange &rhs) noexcept
    { return !(lhs == rhs); }

    inline int topRow() const { return m_top; }
    inline int bottomRow() const { return m_bottom; }
    inline int leftColumn() const { return m_left; }
    inline int rightColumn() const { return m_right; }
    inline int rowCount() const { return m_bottom - m_top + 1; }
    inline int columnCount() const { return m_right - m_left + 1; }
private:
    int m_top = -1, m_left = -1, m_bottom = -2, m_right = -2;
};

class BOBUIableWidget;
class BOBUIableModel;
class QWidgetItemData;
class BOBUIableWidgetItemPrivate;

class Q_WIDGETS_EXPORT BOBUIableWidgetItem
{
    friend class BOBUIableWidget;
    friend class BOBUIableModel;
public:
    enum ItemType { Type = 0, UserType = 1000 };
    explicit BOBUIableWidgetItem(int type = Type);
    explicit BOBUIableWidgetItem(const QString &text, int type = Type);
    explicit BOBUIableWidgetItem(const QIcon &icon, const QString &text, int type = Type);
    BOBUIableWidgetItem(const BOBUIableWidgetItem &other);
    virtual ~BOBUIableWidgetItem();

    virtual BOBUIableWidgetItem *clone() const;

    inline BOBUIableWidget *tableWidget() const { return view; }

    inline int row() const;
    inline int column() const;

    void setSelected(bool select);
    bool isSelected() const;

    inline BobUI::ItemFlags flags() const { return itemFlags; }
    void setFlags(BobUI::ItemFlags flags);

    inline QString text() const
        { return data(BobUI::DisplayRole).toString(); }
    inline void setText(const QString &text);

    inline QIcon icon() const
        { return qvariant_cast<QIcon>(data(BobUI::DecorationRole)); }
    inline void setIcon(const QIcon &icon);

    inline QString statusTip() const
        { return data(BobUI::StatusTipRole).toString(); }
    inline void setStatusTip(const QString &statusTip);

#if BOBUI_CONFIG(tooltip)
    inline QString toolTip() const
        { return data(BobUI::ToolTipRole).toString(); }
    inline void setToolTip(const QString &toolTip);
#endif

#if BOBUI_CONFIG(whatsthis)
    inline QString whatsThis() const
        { return data(BobUI::WhatsThisRole).toString(); }
    inline void setWhatsThis(const QString &whatsThis);
#endif

    inline QFont font() const
        { return qvariant_cast<QFont>(data(BobUI::FontRole)); }
    inline void setFont(const QFont &font);

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    inline int textAlignment() const
        { return data(BobUI::TextAlignmentRole).toInt(); }
#else
    inline BobUI::Alignment textAlignment() const
    { return qvariant_cast<BobUI::Alignment>(data(BobUI::TextAlignmentRole)); }
#endif
#if BOBUI_DEPRECATED_SINCE(6, 4)
    BOBUI_DEPRECATED_VERSION_X_6_4("Use the overload taking BobUI::Alignment")
    inline void setTextAlignment(int alignment)
        { setData(BobUI::TextAlignmentRole, alignment); }
    inline void setTextAlignment(BobUI::AlignmentFlag alignment)
        { setData(BobUI::TextAlignmentRole, QVariant::fromValue(BobUI::Alignment(alignment))); }
#endif
    inline void setTextAlignment(BobUI::Alignment alignment)
        { setData(BobUI::TextAlignmentRole, QVariant::fromValue(alignment)); }

    inline QBrush background() const
        { return qvariant_cast<QBrush>(data(BobUI::BackgroundRole)); }
    inline void setBackground(const QBrush &brush)
        { setData(BobUI::BackgroundRole, brush.style() != BobUI::NoBrush ? QVariant(brush) : QVariant()); }

    inline QBrush foreground() const
        { return qvariant_cast<QBrush>(data(BobUI::ForegroundRole)); }
    inline void setForeground(const QBrush &brush)
        { setData(BobUI::ForegroundRole, brush.style() != BobUI::NoBrush ? QVariant(brush) : QVariant()); }

    inline BobUI::CheckState checkState() const
        { return qvariant_cast<BobUI::CheckState>(data(BobUI::CheckStateRole)); }
    inline void setCheckState(BobUI::CheckState state)
        { setData(BobUI::CheckStateRole, state); }

    inline QSize sizeHint() const
        { return qvariant_cast<QSize>(data(BobUI::SizeHintRole)); }
    inline void setSizeHint(const QSize &size)
        { setData(BobUI::SizeHintRole, size.isValid() ? QVariant(size) : QVariant()); }

    virtual QVariant data(int role) const;
    virtual void setData(int role, const QVariant &value);

    virtual bool operator<(const BOBUIableWidgetItem &other) const;

#ifndef BOBUI_NO_DATASTREAM
    virtual void read(QDataStream &in);
    virtual void write(QDataStream &out) const;
#endif
    BOBUIableWidgetItem &operator=(const BOBUIableWidgetItem &other);

    inline int type() const { return rtti; }

private:
    BOBUIableModel *tableModel() const;

private:
    int rtti;
    QList<QWidgetItemData> values;
    BOBUIableWidget *view;
    BOBUIableWidgetItemPrivate *d;
    BobUI::ItemFlags itemFlags;
};

inline void BOBUIableWidgetItem::setText(const QString &atext)
{ setData(BobUI::DisplayRole, atext); }

inline void BOBUIableWidgetItem::setIcon(const QIcon &aicon)
{ setData(BobUI::DecorationRole, aicon); }

inline void BOBUIableWidgetItem::setStatusTip(const QString &astatusTip)
{ setData(BobUI::StatusTipRole, astatusTip); }

#if BOBUI_CONFIG(tooltip)
inline void BOBUIableWidgetItem::setToolTip(const QString &atoolTip)
{ setData(BobUI::ToolTipRole, atoolTip); }
#endif

#if BOBUI_CONFIG(whatsthis)
inline void BOBUIableWidgetItem::setWhatsThis(const QString &awhatsThis)
{ setData(BobUI::WhatsThisRole, awhatsThis); }
#endif

inline void BOBUIableWidgetItem::setFont(const QFont &afont)
{ setData(BobUI::FontRole, afont); }

#ifndef BOBUI_NO_DATASTREAM
Q_WIDGETS_EXPORT QDataStream &operator>>(QDataStream &in, BOBUIableWidgetItem &item);
Q_WIDGETS_EXPORT QDataStream &operator<<(QDataStream &out, const BOBUIableWidgetItem &item);
#endif

class BOBUIableWidgetPrivate;

class Q_WIDGETS_EXPORT BOBUIableWidget : public BOBUIableView
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount WRITE setRowCount)
    Q_PROPERTY(int columnCount READ columnCount WRITE setColumnCount)
#if BOBUI_CONFIG(draganddrop)
    Q_PROPERTY(BobUI::DropActions supportedDragActions READ supportedDragActions WRITE setSupportedDragActions)
#endif

    friend class BOBUIableModel;
public:
    explicit BOBUIableWidget(QWidget *parent = nullptr);
    BOBUIableWidget(int rows, int columns, QWidget *parent = nullptr);
    ~BOBUIableWidget();

    void setRowCount(int rows);
    int rowCount() const;

    void setColumnCount(int columns);
    int columnCount() const;

    int row(const BOBUIableWidgetItem *item) const;
    int column(const BOBUIableWidgetItem *item) const;

    BOBUIableWidgetItem *item(int row, int column) const;
    void setItem(int row, int column, BOBUIableWidgetItem *item);
    BOBUIableWidgetItem *takeItem(int row, int column);
    QList<BOBUIableWidgetItem*> items(const QMimeData *data) const;
    QModelIndex indexFromItem(const BOBUIableWidgetItem *item) const;
    BOBUIableWidgetItem *itemFromIndex(const QModelIndex &index) const;

    BOBUIableWidgetItem *verticalHeaderItem(int row) const;
    void setVerticalHeaderItem(int row, BOBUIableWidgetItem *item);
    BOBUIableWidgetItem *takeVerticalHeaderItem(int row);

    BOBUIableWidgetItem *horizontalHeaderItem(int column) const;
    void setHorizontalHeaderItem(int column, BOBUIableWidgetItem *item);
    BOBUIableWidgetItem *takeHorizontalHeaderItem(int column);
    void setVerticalHeaderLabels(const QStringList &labels);
    void setHorizontalHeaderLabels(const QStringList &labels);

    int currentRow() const;
    int currentColumn() const;
    BOBUIableWidgetItem *currentItem() const;
    void setCurrentItem(BOBUIableWidgetItem *item);
    void setCurrentItem(BOBUIableWidgetItem *item, QItemSelectionModel::SelectionFlags command);
    void setCurrentCell(int row, int column);
    void setCurrentCell(int row, int column, QItemSelectionModel::SelectionFlags command);

    void sortItems(int column, BobUI::SortOrder order = BobUI::AscendingOrder);
    void setSortingEnabled(bool enable);
    bool isSortingEnabled() const;

    void editItem(BOBUIableWidgetItem *item);
    void openPersistentEditor(BOBUIableWidgetItem *item);
    void closePersistentEditor(BOBUIableWidgetItem *item);
    using QAbstractItemView::isPersistentEditorOpen;
    bool isPersistentEditorOpen(BOBUIableWidgetItem *item) const;

    QWidget *cellWidget(int row, int column) const;
    void setCellWidget(int row, int column, QWidget *widget);
    inline void removeCellWidget(int row, int column);

    void setRangeSelected(const BOBUIableWidgetSelectionRange &range, bool select);

    QList<BOBUIableWidgetSelectionRange> selectedRanges() const;
    QList<BOBUIableWidgetItem*> selectedItems() const;
    QList<BOBUIableWidgetItem*> findItems(const QString &text, BobUI::MatchFlags flags) const;

    int visualRow(int logicalRow) const;
    int visualColumn(int logicalColumn) const;

    BOBUIableWidgetItem *itemAt(const QPoint &p) const;
    inline BOBUIableWidgetItem *itemAt(int x, int y) const;
    QRect visualItemRect(const BOBUIableWidgetItem *item) const;

    const BOBUIableWidgetItem *itemPrototype() const;
    void setItemPrototype(const BOBUIableWidgetItem *item);

#if BOBUI_CONFIG(draganddrop)
    BobUI::DropActions supportedDragActions() const;
    void setSupportedDragActions(BobUI::DropActions actions);
#endif

public Q_SLOTS:
    void scrollToItem(const BOBUIableWidgetItem *item, QAbstractItemView::ScrollHint hint = EnsureVisible);
    void insertRow(int row);
    void insertColumn(int column);
    void removeRow(int row);
    void removeColumn(int column);
    void clear();
    void clearContents();

Q_SIGNALS:
    void itemPressed(BOBUIableWidgetItem *item);
    void itemClicked(BOBUIableWidgetItem *item);
    void itemDoubleClicked(BOBUIableWidgetItem *item);

    void itemActivated(BOBUIableWidgetItem *item);
    void itemEntered(BOBUIableWidgetItem *item);
    void itemChanged(BOBUIableWidgetItem *item);

    void currentItemChanged(BOBUIableWidgetItem *current, BOBUIableWidgetItem *previous);
    void itemSelectionChanged();

    void cellPressed(int row, int column);
    void cellClicked(int row, int column);
    void cellDoubleClicked(int row, int column);

    void cellActivated(int row, int column);
    void cellEntered(int row, int column);
    void cellChanged(int row, int column);

    void currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

protected:
    bool event(QEvent *e) override;
    virtual QStringList mimeTypes() const;
    virtual QMimeData *mimeData(const QList<BOBUIableWidgetItem *> &items) const;
    virtual bool dropMimeData(int row, int column, const QMimeData *data, BobUI::DropAction action);
    virtual BobUI::DropActions supportedDropActions() const;

protected:
#if BOBUI_CONFIG(draganddrop)
    void dropEvent(QDropEvent *event) override;
#endif
private:
    void setModel(QAbstractItemModel *model) override;

    Q_DECLARE_PRIVATE(BOBUIableWidget)
    Q_DISABLE_COPY(BOBUIableWidget)
};

inline void BOBUIableWidget::removeCellWidget(int arow, int acolumn)
{ setCellWidget(arow, acolumn, nullptr); }

inline BOBUIableWidgetItem *BOBUIableWidget::itemAt(int ax, int ay) const
{ return itemAt(QPoint(ax, ay)); }

inline int BOBUIableWidgetItem::row() const
{ return (view ? view->row(this) : -1); }

inline int BOBUIableWidgetItem::column() const
{ return (view ? view->column(this) : -1); }

BOBUI_END_NAMESPACE

#endif // BOBUIABLEWIDGET_H
