// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIREEWIDGET_H
#define BOBUIREEWIDGET_H

#include <BobUIWidgets/bobuireeview.h>
#include <BobUIWidgets/bobuireewidgetitemiterator.h>
#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qvariant.h>

BOBUI_REQUIRE_CONFIG(treewidget);

BOBUI_BEGIN_NAMESPACE

class BOBUIreeWidget;
class BOBUIreeModel;
class QWidgetItemData;
class BOBUIreeWidgetItemPrivate;

class Q_WIDGETS_EXPORT BOBUIreeWidgetItem
{
    friend class BOBUIreeModel;
    friend class BOBUIreeWidget;
    friend class BOBUIreeWidgetPrivate;
    friend class BOBUIreeWidgetItemIterator;
    friend class BOBUIreeWidgetItemPrivate;
public:
    enum ItemType { Type = 0, UserType = 1000 };
    explicit BOBUIreeWidgetItem(int type = Type);
    explicit BOBUIreeWidgetItem(const QStringList &strings, int type = Type);
    explicit BOBUIreeWidgetItem(BOBUIreeWidget *treeview, int type = Type);
    BOBUIreeWidgetItem(BOBUIreeWidget *treeview, const QStringList &strings, int type = Type);
    BOBUIreeWidgetItem(BOBUIreeWidget *treeview, BOBUIreeWidgetItem *after, int type = Type);
    explicit BOBUIreeWidgetItem(BOBUIreeWidgetItem *parent, int type = Type);
    BOBUIreeWidgetItem(BOBUIreeWidgetItem *parent, const QStringList &strings, int type = Type);
    BOBUIreeWidgetItem(BOBUIreeWidgetItem *parent, BOBUIreeWidgetItem *after, int type = Type);
    BOBUIreeWidgetItem(const BOBUIreeWidgetItem &other);
    virtual ~BOBUIreeWidgetItem();

    virtual BOBUIreeWidgetItem *clone() const;

    inline BOBUIreeWidget *treeWidget() const { return view; }

    void setSelected(bool select);
    bool isSelected() const;

    void setHidden(bool hide);
    bool isHidden() const;

    void setExpanded(bool expand);
    bool isExpanded() const;

    void setFirstColumnSpanned(bool span);
    bool isFirstColumnSpanned() const;

    inline void setDisabled(bool disabled);
    inline bool isDisabled() const;

    enum ChildIndicatorPolicy { ShowIndicator, DontShowIndicator, DontShowIndicatorWhenChildless };
    void setChildIndicatorPolicy(BOBUIreeWidgetItem::ChildIndicatorPolicy policy);
    BOBUIreeWidgetItem::ChildIndicatorPolicy childIndicatorPolicy() const;

    BobUI::ItemFlags flags() const;
    void setFlags(BobUI::ItemFlags flags);

    inline QString text(int column) const
        { return data(column, BobUI::DisplayRole).toString(); }
    inline void setText(int column, const QString &text);

    inline QIcon icon(int column) const
        { return qvariant_cast<QIcon>(data(column, BobUI::DecorationRole)); }
    inline void setIcon(int column, const QIcon &icon);

    inline QString statusTip(int column) const
        { return data(column, BobUI::StatusTipRole).toString(); }
    inline void setStatusTip(int column, const QString &statusTip);

#if BOBUI_CONFIG(tooltip)
    inline QString toolTip(int column) const
        { return data(column, BobUI::ToolTipRole).toString(); }
    inline void setToolTip(int column, const QString &toolTip);
#endif

#if BOBUI_CONFIG(whatsthis)
    inline QString whatsThis(int column) const
        { return data(column, BobUI::WhatsThisRole).toString(); }
    inline void setWhatsThis(int column, const QString &whatsThis);
#endif

    inline QFont font(int column) const
        { return qvariant_cast<QFont>(data(column, BobUI::FontRole)); }
    inline void setFont(int column, const QFont &font);

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    inline int textAlignment(int column) const
        { return data(column, BobUI::TextAlignmentRole).toInt(); }
#else
    inline BobUI::Alignment textAlignment(int column) const
    { return qvariant_cast<BobUI::Alignment>(data(column, BobUI::TextAlignmentRole)); }
#endif
#if BOBUI_DEPRECATED_SINCE(6, 4)
    BOBUI_DEPRECATED_VERSION_X_6_4("Use the overload taking BobUI::Alignment")
    inline void setTextAlignment(int column, int alignment)
        { setData(column, BobUI::TextAlignmentRole, alignment); }
    inline void setTextAlignment(int column, BobUI::AlignmentFlag alignment)
        { setData(column, BobUI::TextAlignmentRole, QVariant::fromValue(BobUI::Alignment(alignment))); }
#endif
    inline void setTextAlignment(int column, BobUI::Alignment alignment)
        { setData(column, BobUI::TextAlignmentRole, QVariant::fromValue(alignment)); }

    inline QBrush background(int column) const
        { return qvariant_cast<QBrush>(data(column, BobUI::BackgroundRole)); }
    inline void setBackground(int column, const QBrush &brush)
        { setData(column, BobUI::BackgroundRole, brush.style() != BobUI::NoBrush ? QVariant(brush) : QVariant()); }

    inline QBrush foreground(int column) const
        { return qvariant_cast<QBrush>(data(column, BobUI::ForegroundRole)); }
    inline void setForeground(int column, const QBrush &brush)
        { setData(column, BobUI::ForegroundRole, brush.style() != BobUI::NoBrush ? QVariant(brush) : QVariant()); }

    inline BobUI::CheckState checkState(int column) const
        { return qvariant_cast<BobUI::CheckState>(data(column, BobUI::CheckStateRole)); }
    inline void setCheckState(int column, BobUI::CheckState state)
        { setData(column, BobUI::CheckStateRole, state); }

    inline QSize sizeHint(int column) const
        { return qvariant_cast<QSize>(data(column, BobUI::SizeHintRole)); }
    inline void setSizeHint(int column, const QSize &size)
        { setData(column, BobUI::SizeHintRole, size.isValid() ? QVariant(size) : QVariant()); }

    virtual QVariant data(int column, int role) const;
    virtual void setData(int column, int role, const QVariant &value);

    virtual bool operator<(const BOBUIreeWidgetItem &other) const;

#ifndef BOBUI_NO_DATASTREAM
    virtual void read(QDataStream &in);
    virtual void write(QDataStream &out) const;
#endif
    BOBUIreeWidgetItem &operator=(const BOBUIreeWidgetItem &other);

    inline BOBUIreeWidgetItem *parent() const { return par; }
    inline BOBUIreeWidgetItem *child(int index) const {
        if (index < 0 || index >= children.size())
            return nullptr;
        executePendingSort();
        return children.at(index);
    }
    inline int childCount() const { return int(children.size()); }
    inline int columnCount() const { return int(values.size()); }
    inline int indexOfChild(BOBUIreeWidgetItem *child) const;

    void addChild(BOBUIreeWidgetItem *child);
    void insertChild(int index, BOBUIreeWidgetItem *child);
    void removeChild(BOBUIreeWidgetItem *child);
    BOBUIreeWidgetItem *takeChild(int index);

    void addChildren(const QList<BOBUIreeWidgetItem*> &children);
    void insertChildren(int index, const QList<BOBUIreeWidgetItem*> &children);
    QList<BOBUIreeWidgetItem*> takeChildren();

    inline int type() const { return rtti; }
    inline void sortChildren(int column, BobUI::SortOrder order)
        { sortChildren(column, order, false); }

protected:
    void emitDataChanged();

private:
    void sortChildren(int column, BobUI::SortOrder order, bool climb);
    QVariant childrenCheckState(int column) const;
    void itemChanged();
    void executePendingSort() const;
    BOBUIreeModel *treeModel(BOBUIreeWidget *v = nullptr) const;

    int rtti;
    // One item has a vector of column entries. Each column has a vector of (role, value) pairs.
    QList<QList<QWidgetItemData>> values;
    BOBUIreeWidget *view = nullptr;
    BOBUIreeWidgetItemPrivate *d;
    BOBUIreeWidgetItem *par = nullptr;
    // ### BobUI7: Move children to d-pointer and replace QList by a suitable data structure.
    //          to fix BOBUIBUG-94546
    QList<BOBUIreeWidgetItem*> children;
    BobUI::ItemFlags itemFlags = BobUI::ItemIsSelectable | BobUI::ItemIsUserCheckable | BobUI::ItemIsEnabled
            | BobUI::ItemIsDragEnabled | BobUI::ItemIsDropEnabled;
};

inline void BOBUIreeWidgetItem::setText(int column, const QString &atext)
{ setData(column, BobUI::DisplayRole, atext); }

inline void BOBUIreeWidgetItem::setIcon(int column, const QIcon &aicon)
{ setData(column, BobUI::DecorationRole, aicon); }

#if BOBUI_CONFIG(statustip)
inline void BOBUIreeWidgetItem::setStatusTip(int column, const QString &astatusTip)
{ setData(column, BobUI::StatusTipRole, astatusTip); }
#endif

#if BOBUI_CONFIG(tooltip)
inline void BOBUIreeWidgetItem::setToolTip(int column, const QString &atoolTip)
{ setData(column, BobUI::ToolTipRole, atoolTip); }
#endif

#if BOBUI_CONFIG(whatsthis)
inline void BOBUIreeWidgetItem::setWhatsThis(int column, const QString &awhatsThis)
{ setData(column, BobUI::WhatsThisRole, awhatsThis); }
#endif

inline void BOBUIreeWidgetItem::setFont(int column, const QFont &afont)
{ setData(column, BobUI::FontRole, afont); }

inline int BOBUIreeWidgetItem::indexOfChild(BOBUIreeWidgetItem *achild) const
{ executePendingSort(); return int(children.indexOf(achild)); }

#ifndef BOBUI_NO_DATASTREAM
Q_WIDGETS_EXPORT QDataStream &operator<<(QDataStream &out, const BOBUIreeWidgetItem &item);
Q_WIDGETS_EXPORT QDataStream &operator>>(QDataStream &in, BOBUIreeWidgetItem &item);
#endif

class BOBUIreeWidgetPrivate;

class Q_WIDGETS_EXPORT BOBUIreeWidget : public BOBUIreeView
{
    Q_OBJECT
    Q_PROPERTY(int columnCount READ columnCount WRITE setColumnCount)
    Q_PROPERTY(int topLevelItemCount READ topLevelItemCount)
#if BOBUI_CONFIG(draganddrop)
    Q_PROPERTY(BobUI::DropActions supportedDragActions READ supportedDragActions WRITE setSupportedDragActions)
#endif

    friend class BOBUIreeModel;
    friend class BOBUIreeWidgetItem;
public:
    explicit BOBUIreeWidget(QWidget *parent = nullptr);
    ~BOBUIreeWidget();

    int columnCount() const;
    void setColumnCount(int columns);

    BOBUIreeWidgetItem *invisibleRootItem() const;
    BOBUIreeWidgetItem *topLevelItem(int index) const;
    int topLevelItemCount() const;
    void insertTopLevelItem(int index, BOBUIreeWidgetItem *item);
    void addTopLevelItem(BOBUIreeWidgetItem *item);
    BOBUIreeWidgetItem *takeTopLevelItem(int index);
    int indexOfTopLevelItem(BOBUIreeWidgetItem *item) const;

    void insertTopLevelItems(int index, const QList<BOBUIreeWidgetItem*> &items);
    void addTopLevelItems(const QList<BOBUIreeWidgetItem*> &items);

    BOBUIreeWidgetItem *headerItem() const;
    void setHeaderItem(BOBUIreeWidgetItem *item);
    void setHeaderLabels(const QStringList &labels);
    inline void setHeaderLabel(const QString &label);

    BOBUIreeWidgetItem *currentItem() const;
    int currentColumn() const;
    void setCurrentItem(BOBUIreeWidgetItem *item);
    void setCurrentItem(BOBUIreeWidgetItem *item, int column);
    void setCurrentItem(BOBUIreeWidgetItem *item, int column, QItemSelectionModel::SelectionFlags command);

    BOBUIreeWidgetItem *itemAt(const QPoint &p) const;
    inline BOBUIreeWidgetItem *itemAt(int x, int y) const;
    QRect visualItemRect(const BOBUIreeWidgetItem *item) const;

    int sortColumn() const;
    void sortItems(int column, BobUI::SortOrder order);

    void editItem(BOBUIreeWidgetItem *item, int column = 0);
    void openPersistentEditor(BOBUIreeWidgetItem *item, int column = 0);
    void closePersistentEditor(BOBUIreeWidgetItem *item, int column = 0);
    using QAbstractItemView::isPersistentEditorOpen;
    bool isPersistentEditorOpen(BOBUIreeWidgetItem *item, int column = 0) const;

    QWidget *itemWidget(BOBUIreeWidgetItem *item, int column) const;
    void setItemWidget(BOBUIreeWidgetItem *item, int column, QWidget *widget);
    inline void removeItemWidget(BOBUIreeWidgetItem *item, int column);

    QList<BOBUIreeWidgetItem*> selectedItems() const;
    QList<BOBUIreeWidgetItem*> findItems(const QString &text, BobUI::MatchFlags flags,
                                      int column = 0) const;

    BOBUIreeWidgetItem *itemAbove(const BOBUIreeWidgetItem *item) const;
    BOBUIreeWidgetItem *itemBelow(const BOBUIreeWidgetItem *item) const;

    QModelIndex indexFromItem(const BOBUIreeWidgetItem *item, int column = 0) const;
    BOBUIreeWidgetItem *itemFromIndex(const QModelIndex &index) const;

    void setSelectionModel(QItemSelectionModel *selectionModel) override;

#if BOBUI_CONFIG(draganddrop)
    BobUI::DropActions supportedDragActions() const;
    void setSupportedDragActions(BobUI::DropActions actions);
#endif

public Q_SLOTS:
    void scrollToItem(const BOBUIreeWidgetItem *item,
                      QAbstractItemView::ScrollHint hint = EnsureVisible);
    void expandItem(const BOBUIreeWidgetItem *item);
    void collapseItem(const BOBUIreeWidgetItem *item);
    void clear();

Q_SIGNALS:
    void itemPressed(BOBUIreeWidgetItem *item, int column);
    void itemClicked(BOBUIreeWidgetItem *item, int column);
    void itemDoubleClicked(BOBUIreeWidgetItem *item, int column);
    void itemActivated(BOBUIreeWidgetItem *item, int column);
    void itemEntered(BOBUIreeWidgetItem *item, int column);
    void itemChanged(BOBUIreeWidgetItem *item, int column);
    void itemExpanded(BOBUIreeWidgetItem *item);
    void itemCollapsed(BOBUIreeWidgetItem *item);
    void currentItemChanged(BOBUIreeWidgetItem *current, BOBUIreeWidgetItem *previous);
    void itemSelectionChanged();

protected:
    bool event(QEvent *e) override;
    virtual QStringList mimeTypes() const;
    virtual QMimeData *mimeData(const QList<BOBUIreeWidgetItem *> &items) const;
    virtual bool dropMimeData(BOBUIreeWidgetItem *parent, int index,
                              const QMimeData *data, BobUI::DropAction action);
    virtual BobUI::DropActions supportedDropActions() const;

#if BOBUI_CONFIG(draganddrop)
    void dropEvent(QDropEvent *event) override;
#endif
private:
    void setModel(QAbstractItemModel *model) override;

    Q_DECLARE_PRIVATE(BOBUIreeWidget)
    Q_DISABLE_COPY(BOBUIreeWidget)
};

inline void BOBUIreeWidget::removeItemWidget(BOBUIreeWidgetItem *item, int column)
{ setItemWidget(item, column, nullptr); }

inline BOBUIreeWidgetItem *BOBUIreeWidget::itemAt(int ax, int ay) const
{ return itemAt(QPoint(ax, ay)); }

inline void BOBUIreeWidget::setHeaderLabel(const QString &alabel)
{ setHeaderLabels(QStringList(alabel)); }

inline void BOBUIreeWidgetItem::setDisabled(bool disabled)
{ setFlags(disabled ? (flags() & ~BobUI::ItemIsEnabled) : flags() | BobUI::ItemIsEnabled); }

inline bool BOBUIreeWidgetItem::isDisabled() const
{ return !(flags() & BobUI::ItemIsEnabled); }

BOBUI_END_NAMESPACE

#endif // BOBUIREEWIDGET_H
