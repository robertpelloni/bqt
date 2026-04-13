// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSTANDARDITEMMODEL_H
#define QSTANDARDITEMMODEL_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qabstractitemmodel.h>
#include <BobUIGui/qbrush.h>
#include <BobUIGui/qfont.h>
#include <BobUIGui/qicon.h>
#ifndef BOBUI_NO_DATASTREAM
#include <BobUICore/qdatastream.h>
#endif

BOBUI_REQUIRE_CONFIG(standarditemmodel);

BOBUI_BEGIN_NAMESPACE

class QStandardItemModel;

class QStandardItemPrivate;
class Q_GUI_EXPORT QStandardItem
{
public:
    QStandardItem();
    explicit QStandardItem(const QString &text);
    QStandardItem(const QIcon &icon, const QString &text);
    explicit QStandardItem(int rows, int columns = 1);
    virtual ~QStandardItem();

    virtual QVariant data(int role = BobUI::UserRole + 1) const;
    virtual void multiData(QModelRoleDataSpan roleDataSpan) const;
    virtual void setData(const QVariant &value, int role = BobUI::UserRole + 1);
    void clearData();

    inline QString text() const {
        return qvariant_cast<QString>(data(BobUI::DisplayRole));
    }
    inline void setText(const QString &text);

    inline QIcon icon() const {
        return qvariant_cast<QIcon>(data(BobUI::DecorationRole));
    }
    inline void setIcon(const QIcon &icon);

    inline QString toolTip() const {
        return qvariant_cast<QString>(data(BobUI::ToolTipRole));
    }
    inline void setToolTip(const QString &toolTip);

#ifndef BOBUI_NO_STATUSTIP
    inline QString statusTip() const {
        return qvariant_cast<QString>(data(BobUI::StatusTipRole));
    }
    inline void setStatusTip(const QString &statusTip);
#endif

#if BOBUI_CONFIG(whatsthis)
    inline QString whatsThis() const {
        return qvariant_cast<QString>(data(BobUI::WhatsThisRole));
    }
    inline void setWhatsThis(const QString &whatsThis);
#endif

    inline QSize sizeHint() const {
        return qvariant_cast<QSize>(data(BobUI::SizeHintRole));
    }
    inline void setSizeHint(const QSize &sizeHint);

    inline QFont font() const {
        return qvariant_cast<QFont>(data(BobUI::FontRole));
    }
    inline void setFont(const QFont &font);

    inline BobUI::Alignment textAlignment() const {
        return qvariant_cast<BobUI::Alignment>(data(BobUI::TextAlignmentRole));
    }
    inline void setTextAlignment(BobUI::Alignment textAlignment);

    inline QBrush background() const {
        return qvariant_cast<QBrush>(data(BobUI::BackgroundRole));
    }
    inline void setBackground(const QBrush &brush);

    inline QBrush foreground() const {
        return qvariant_cast<QBrush>(data(BobUI::ForegroundRole));
    }
    inline void setForeground(const QBrush &brush);

    inline BobUI::CheckState checkState() const {
        return BobUI::CheckState(qvariant_cast<int>(data(BobUI::CheckStateRole)));
    }
    inline void setCheckState(BobUI::CheckState checkState);

    inline QString accessibleText() const {
        return qvariant_cast<QString>(data(BobUI::AccessibleTextRole));
    }
    inline void setAccessibleText(const QString &accessibleText);

    inline QString accessibleDescription() const {
        return qvariant_cast<QString>(data(BobUI::AccessibleDescriptionRole));
    }
    inline void setAccessibleDescription(const QString &accessibleDescription);

    BobUI::ItemFlags flags() const;
    void setFlags(BobUI::ItemFlags flags);

    inline bool isEnabled() const {
        return bool(flags() & BobUI::ItemIsEnabled);
    }
    void setEnabled(bool enabled);

    inline bool isEditable() const {
        return bool(flags() & BobUI::ItemIsEditable);
    }
    void setEditable(bool editable);

    inline bool isSelectable() const {
        return bool(flags() & BobUI::ItemIsSelectable);
    }
    void setSelectable(bool selectable);

    inline bool isCheckable() const {
        return bool(flags() & BobUI::ItemIsUserCheckable);
    }
    void setCheckable(bool checkable);

    inline bool isAutoTristate() const {
        return bool(flags() & BobUI::ItemIsAutoTristate);
    }
    void setAutoTristate(bool tristate);

    inline bool isUserTristate() const {
        return bool(flags() & BobUI::ItemIsUserTristate);
    }
    void setUserTristate(bool tristate);

#if BOBUI_CONFIG(draganddrop)
    inline bool isDragEnabled() const {
        return bool(flags() & BobUI::ItemIsDragEnabled);
    }
    void setDragEnabled(bool dragEnabled);

    inline bool isDropEnabled() const {
        return bool(flags() & BobUI::ItemIsDropEnabled);
    }
    void setDropEnabled(bool dropEnabled);
#endif // BOBUI_CONFIG(draganddrop)

    QStandardItem *parent() const;
    int row() const;
    int column() const;
    QModelIndex index() const;
    QStandardItemModel *model() const;

    int rowCount() const;
    void setRowCount(int rows);
    int columnCount() const;
    void setColumnCount(int columns);

    bool hasChildren() const;
    QStandardItem *child(int row, int column = 0) const;
    void setChild(int row, int column, QStandardItem *item);
    inline void setChild(int row, QStandardItem *item);

    void insertRow(int row, const QList<QStandardItem*> &items);
    void insertColumn(int column, const QList<QStandardItem*> &items);
    void insertRows(int row, const QList<QStandardItem*> &items);
    void insertRows(int row, int count);
    void insertColumns(int column, int count);

    void removeRow(int row);
    void removeColumn(int column);
    void removeRows(int row, int count);
    void removeColumns(int column, int count);

    inline void appendRow(const QList<QStandardItem*> &items);
    inline void appendRows(const QList<QStandardItem*> &items);
    inline void appendColumn(const QList<QStandardItem*> &items);
    inline void insertRow(int row, QStandardItem *item);
    inline void appendRow(QStandardItem *item);

    QStandardItem *takeChild(int row, int column = 0);
    QList<QStandardItem*> takeRow(int row);
    QList<QStandardItem*> takeColumn(int column);

    void sortChildren(int column, BobUI::SortOrder order = BobUI::AscendingOrder);

    virtual QStandardItem *clone() const;

    enum ItemType { Type = 0, UserType = 1000 };
    virtual int type() const;

#ifndef BOBUI_NO_DATASTREAM
    virtual void read(QDataStream &in);
    virtual void write(QDataStream &out) const;
#endif
    virtual bool operator<(const QStandardItem &other) const;

protected:
    QStandardItem(const QStandardItem &other);
    QStandardItem(QStandardItemPrivate &dd);
    QStandardItem &operator=(const QStandardItem &other);
    QScopedPointer<QStandardItemPrivate> d_ptr;

    void emitDataChanged();

private:
    Q_DECLARE_PRIVATE(QStandardItem)
    friend class QStandardItemModelPrivate;
    friend class QStandardItemModel;
};

inline void QStandardItem::setText(const QString &atext)
{ setData(atext, BobUI::DisplayRole); }

inline void QStandardItem::setIcon(const QIcon &aicon)
{ setData(aicon, BobUI::DecorationRole); }

inline void QStandardItem::setToolTip(const QString &atoolTip)
{ setData(atoolTip, BobUI::ToolTipRole); }

#ifndef BOBUI_NO_STATUSTIP
inline void QStandardItem::setStatusTip(const QString &astatusTip)
{ setData(astatusTip, BobUI::StatusTipRole); }
#endif

#if BOBUI_CONFIG(whatsthis)
inline void QStandardItem::setWhatsThis(const QString &awhatsThis)
{ setData(awhatsThis, BobUI::WhatsThisRole); }
#endif

inline void QStandardItem::setSizeHint(const QSize &asizeHint)
{ setData(asizeHint, BobUI::SizeHintRole); }

inline void QStandardItem::setFont(const QFont &afont)
{ setData(afont, BobUI::FontRole); }

inline void QStandardItem::setTextAlignment(BobUI::Alignment atextAlignment)
{ setData(QVariant::fromValue(atextAlignment), BobUI::TextAlignmentRole); }

inline void QStandardItem::setBackground(const QBrush &abrush)
{ setData(abrush, BobUI::BackgroundRole); }

inline void QStandardItem::setForeground(const QBrush &abrush)
{ setData(abrush, BobUI::ForegroundRole); }

inline void QStandardItem::setCheckState(BobUI::CheckState acheckState)
{ setData(acheckState, BobUI::CheckStateRole); }

inline void QStandardItem::setAccessibleText(const QString &aaccessibleText)
{ setData(aaccessibleText, BobUI::AccessibleTextRole); }

inline void QStandardItem::setAccessibleDescription(const QString &aaccessibleDescription)
{ setData(aaccessibleDescription, BobUI::AccessibleDescriptionRole); }

inline void QStandardItem::setChild(int arow, QStandardItem *aitem)
{ setChild(arow, 0, aitem); }

inline void QStandardItem::appendRow(const QList<QStandardItem*> &aitems)
{ insertRow(rowCount(), aitems); }

inline void QStandardItem::appendRows(const QList<QStandardItem*> &aitems)
{ insertRows(rowCount(), aitems); }

inline void QStandardItem::appendColumn(const QList<QStandardItem*> &aitems)
{ insertColumn(columnCount(), aitems); }

inline void QStandardItem::insertRow(int arow, QStandardItem *aitem)
{ insertRow(arow, QList<QStandardItem*>() << aitem); }

inline void QStandardItem::appendRow(QStandardItem *aitem)
{ insertRow(rowCount(), aitem); }

class QStandardItemModelPrivate;

class Q_GUI_EXPORT QStandardItemModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(int sortRole READ sortRole WRITE setSortRole BINDABLE bindableSortRole)

public:
    explicit QStandardItemModel(QObject *parent = nullptr);
    QStandardItemModel(int rows, int columns, QObject *parent = nullptr);
    ~QStandardItemModel();

    void setItemRoleNames(const QHash<int,QByteArray> &roleNames);
    QHash<int, QByteArray> roleNames() const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override;
    void multiData(const QModelIndex &index, QModelRoleDataSpan roleDataSpan) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = BobUI::EditRole) override;
    bool clearItemData(const QModelIndex &index) override;

    QVariant headerData(int section, BobUI::Orientation orientation,
                        int role = BobUI::DisplayRole) const override;
    bool setHeaderData(int section, BobUI::Orientation orientation, const QVariant &value,
                       int role = BobUI::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    BobUI::ItemFlags flags(const QModelIndex &index) const override;
    BobUI::DropActions supportedDropActions() const override;

    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;

    void clear();

    using QObject::parent;

    void sort(int column, BobUI::SortOrder order = BobUI::AscendingOrder) override;

    QStandardItem *itemFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromItem(const QStandardItem *item) const;

    QStandardItem *item(int row, int column = 0) const;
    void setItem(int row, int column, QStandardItem *item);
    inline void setItem(int row, QStandardItem *item);
    QStandardItem *invisibleRootItem() const;

    QStandardItem *horizontalHeaderItem(int column) const;
    void setHorizontalHeaderItem(int column, QStandardItem *item);
    QStandardItem *verticalHeaderItem(int row) const;
    void setVerticalHeaderItem(int row, QStandardItem *item);

    void setHorizontalHeaderLabels(const QStringList &labels);
    void setVerticalHeaderLabels(const QStringList &labels);

    void setRowCount(int rows);
    void setColumnCount(int columns);

    void appendRow(const QList<QStandardItem*> &items);
    void appendColumn(const QList<QStandardItem*> &items);
    inline void appendRow(QStandardItem *item);

    void insertRow(int row, const QList<QStandardItem*> &items);
    void insertColumn(int column, const QList<QStandardItem*> &items);
    inline void insertRow(int row, QStandardItem *item);

    inline bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    inline bool insertColumn(int column, const QModelIndex &parent = QModelIndex());

    QStandardItem *takeItem(int row, int column = 0);
    QList<QStandardItem*> takeRow(int row);
    QList<QStandardItem*> takeColumn(int column);

    QStandardItem *takeHorizontalHeaderItem(int column);
    QStandardItem *takeVerticalHeaderItem(int row);

    const QStandardItem *itemPrototype() const;
    void setItemPrototype(const QStandardItem *item);

    QList<QStandardItem*> findItems(const QString &text,
                                    BobUI::MatchFlags flags = BobUI::MatchExactly,
                                    int column = 0) const;

    int sortRole() const;
    void setSortRole(int role);
    QBindable<int> bindableSortRole();

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData (const QMimeData *data, BobUI::DropAction action, int row, int column, const QModelIndex &parent) override;

Q_SIGNALS:
    void itemChanged(QStandardItem *item);

protected:
    QStandardItemModel(QStandardItemModelPrivate &dd, QObject *parent = nullptr);

private:
    friend class QStandardItemPrivate;
    friend class QStandardItem;
    Q_DISABLE_COPY(QStandardItemModel)
    Q_DECLARE_PRIVATE(QStandardItemModel)

    Q_PRIVATE_SLOT(d_func(), void _q_emitItemChanged(const QModelIndex &topLeft,
                                                     const QModelIndex &bottomRight))
};

inline void QStandardItemModel::setItem(int arow, QStandardItem *aitem)
{ setItem(arow, 0, aitem); }

inline void QStandardItemModel::appendRow(QStandardItem *aitem)
{ appendRow(QList<QStandardItem*>() << aitem); }

inline void QStandardItemModel::insertRow(int arow, QStandardItem *aitem)
{ insertRow(arow, QList<QStandardItem*>() << aitem); }

inline bool QStandardItemModel::insertRow(int arow, const QModelIndex &aparent)
{ return QAbstractItemModel::insertRow(arow, aparent); }
inline bool QStandardItemModel::insertColumn(int acolumn, const QModelIndex &aparent)
{ return QAbstractItemModel::insertColumn(acolumn, aparent); }

#ifndef BOBUI_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &in, QStandardItem &item);
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &out, const QStandardItem &item);
#endif

BOBUI_END_NAMESPACE

#endif //QSTANDARDITEMMODEL_H
