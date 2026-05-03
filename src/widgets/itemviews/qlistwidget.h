// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLISTWIDGET_H
#define QLISTWIDGET_H

#include <BobUIWidgets/qlistview.h>
#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qitemselectionmodel.h>
#include <BobUICore/qvariant.h>

BOBUI_REQUIRE_CONFIG(listwidget);

BOBUI_BEGIN_NAMESPACE

class QListWidget;
class QListModel;
class QWidgetItemData;
class QListWidgetItemPrivate;

class Q_WIDGETS_EXPORT QListWidgetItem
{
    friend class QListModel;
    friend class QListWidget;
public:
    enum ItemType { Type = 0, UserType = 1000 };
    explicit QListWidgetItem(QListWidget *listview = nullptr, int type = Type);
    explicit QListWidgetItem(const QString &text, QListWidget *listview = nullptr, int type = Type);
    explicit QListWidgetItem(const QIcon &icon, const QString &text,
                             QListWidget *listview = nullptr, int type = Type);
    QListWidgetItem(const QListWidgetItem &other);
    virtual ~QListWidgetItem();

    virtual QListWidgetItem *clone() const;

    inline QListWidget *listWidget() const { return view; }

    void setSelected(bool select);
    bool isSelected() const;

    inline void setHidden(bool hide);
    inline bool isHidden() const;

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
        { setData(BobUI::CheckStateRole, static_cast<int>(state)); }

    inline QSize sizeHint() const
        { return qvariant_cast<QSize>(data(BobUI::SizeHintRole)); }
    inline void setSizeHint(const QSize &size)
        { setData(BobUI::SizeHintRole, size.isValid() ? QVariant(size) : QVariant()); }

    virtual QVariant data(int role) const;
    virtual void setData(int role, const QVariant &value);

    virtual bool operator<(const QListWidgetItem &other) const;

#ifndef BOBUI_NO_DATASTREAM
    virtual void read(QDataStream &in);
    virtual void write(QDataStream &out) const;
#endif
    QListWidgetItem &operator=(const QListWidgetItem &other);

    inline int type() const { return rtti; }

private:
    QListModel *listModel() const;
    int rtti;
    QListWidget *view;
    QListWidgetItemPrivate *d;
    BobUI::ItemFlags itemFlags;
};

inline void QListWidgetItem::setText(const QString &atext)
{ setData(BobUI::DisplayRole, atext); }

inline void QListWidgetItem::setIcon(const QIcon &aicon)
{ setData(BobUI::DecorationRole, aicon); }

inline void QListWidgetItem::setStatusTip(const QString &astatusTip)
{ setData(BobUI::StatusTipRole, astatusTip); }

#if BOBUI_CONFIG(tooltip)
inline void QListWidgetItem::setToolTip(const QString &atoolTip)
{ setData(BobUI::ToolTipRole, atoolTip); }
#endif

#if BOBUI_CONFIG(whatsthis)
inline void QListWidgetItem::setWhatsThis(const QString &awhatsThis)
{ setData(BobUI::WhatsThisRole, awhatsThis); }
#endif

inline void QListWidgetItem::setFont(const QFont &afont)
{ setData(BobUI::FontRole, afont); }

#ifndef BOBUI_NO_DATASTREAM
Q_WIDGETS_EXPORT QDataStream &operator<<(QDataStream &out, const QListWidgetItem &item);
Q_WIDGETS_EXPORT QDataStream &operator>>(QDataStream &in, QListWidgetItem &item);
#endif

class QListWidgetPrivate;

class Q_WIDGETS_EXPORT QListWidget : public QListView
{
    Q_OBJECT
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(int currentRow READ currentRow WRITE setCurrentRow NOTIFY currentRowChanged
               USER true)
    Q_PROPERTY(bool sortingEnabled READ isSortingEnabled WRITE setSortingEnabled)
#if BOBUI_CONFIG(draganddrop)
    Q_PROPERTY(BobUI::DropActions supportedDragActions READ supportedDragActions WRITE setSupportedDragActions)
#endif

    friend class QListWidgetItem;
    friend class QListModel;
public:
    explicit QListWidget(QWidget *parent = nullptr);
    ~QListWidget();

    void setSelectionModel(QItemSelectionModel *selectionModel) override;

    QListWidgetItem *item(int row) const;
    int row(const QListWidgetItem *item) const;
    void insertItem(int row, QListWidgetItem *item);
    void insertItem(int row, const QString &label);
    void insertItems(int row, const QStringList &labels);
    inline void addItem(const QString &label) { insertItem(count(), label); }
    inline void addItem(QListWidgetItem *item);
    inline void addItems(const QStringList &labels) { insertItems(count(), labels); }
    QListWidgetItem *takeItem(int row);
    int count() const;

    QListWidgetItem *currentItem() const;
    void setCurrentItem(QListWidgetItem *item);
    void setCurrentItem(QListWidgetItem *item, QItemSelectionModel::SelectionFlags command);

    int currentRow() const;
    void setCurrentRow(int row);
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command);

    QListWidgetItem *itemAt(const QPoint &p) const;
    inline QListWidgetItem *itemAt(int x, int y) const;
    QRect visualItemRect(const QListWidgetItem *item) const;

    void sortItems(BobUI::SortOrder order = BobUI::AscendingOrder);
    void setSortingEnabled(bool enable);
    bool isSortingEnabled() const;

    void editItem(QListWidgetItem *item);
    void openPersistentEditor(QListWidgetItem *item);
    void closePersistentEditor(QListWidgetItem *item);
    using QAbstractItemView::isPersistentEditorOpen;
    bool isPersistentEditorOpen(QListWidgetItem *item) const;

    QWidget *itemWidget(QListWidgetItem *item) const;
    void setItemWidget(QListWidgetItem *item, QWidget *widget);
    inline void removeItemWidget(QListWidgetItem *item);

    QList<QListWidgetItem*> selectedItems() const;
    QList<QListWidgetItem*> findItems(const QString &text, BobUI::MatchFlags flags) const;

    QList<QListWidgetItem*> items(const QMimeData *data) const;

    QModelIndex indexFromItem(const QListWidgetItem *item) const;
    QListWidgetItem *itemFromIndex(const QModelIndex &index) const;

#if BOBUI_CONFIG(draganddrop)
    void setSupportedDragActions(BobUI::DropActions actions);
    BobUI::DropActions supportedDragActions() const;
#endif
protected:
#if BOBUI_CONFIG(draganddrop)
    void dropEvent(QDropEvent *event) override;
#endif
public Q_SLOTS:
    void scrollToItem(const QListWidgetItem *item, QAbstractItemView::ScrollHint hint = EnsureVisible);
    void clear();

Q_SIGNALS:
    void itemPressed(QListWidgetItem *item);
    void itemClicked(QListWidgetItem *item);
    void itemDoubleClicked(QListWidgetItem *item);
    void itemActivated(QListWidgetItem *item);
    void itemEntered(QListWidgetItem *item);
    void itemChanged(QListWidgetItem *item);

    void currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void currentTextChanged(const QString &currentText);
    void currentRowChanged(int currentRow);

    void itemSelectionChanged();

protected:
    bool event(QEvent *e) override;
    virtual QStringList mimeTypes() const;
    virtual QMimeData *mimeData(const QList<QListWidgetItem *> &items) const;
#if BOBUI_CONFIG(draganddrop)
    virtual bool dropMimeData(int index, const QMimeData *data, BobUI::DropAction action);
    virtual BobUI::DropActions supportedDropActions() const;
#endif

private:
    void setModel(QAbstractItemModel *model) override;
    BobUI::SortOrder sortOrder() const;

    Q_DECLARE_PRIVATE(QListWidget)
    Q_DISABLE_COPY(QListWidget)
};

inline void QListWidget::removeItemWidget(QListWidgetItem *aItem)
{ setItemWidget(aItem, nullptr); }

inline void QListWidget::addItem(QListWidgetItem *aitem)
{ insertItem(count(), aitem); }

inline QListWidgetItem *QListWidget::itemAt(int ax, int ay) const
{ return itemAt(QPoint(ax, ay)); }

inline void QListWidgetItem::setHidden(bool ahide)
{ if (view) view->setRowHidden(view->row(this), ahide); }

inline bool QListWidgetItem::isHidden() const
{ return (view ? view->isRowHidden(view->row(this)) : false); }

BOBUI_END_NAMESPACE

#endif // QLISTWIDGET_H
