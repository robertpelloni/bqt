// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSORTFILTERPROXYMODEL_H
#define QSORTFILTERPROXYMODEL_H

#include <BobUICore/qabstractproxymodel.h>

#include <BobUICore/qregularexpression.h>

BOBUI_REQUIRE_CONFIG(sortfilterproxymodel);

BOBUI_BEGIN_NAMESPACE


class QSortFilterProxyModelPrivate;
class QSortFilterProxyModelLessThan;
class QSortFilterProxyModelGreaterThan;

class Q_CORE_EXPORT QSortFilterProxyModel : public QAbstractProxyModel
{
    friend class QSortFilterProxyModelLessThan;
    friend class QSortFilterProxyModelGreaterThan;

    Q_OBJECT
    Q_PROPERTY(QRegularExpression filterRegularExpression READ filterRegularExpression
               WRITE setFilterRegularExpression BINDABLE bindableFilterRegularExpression)
    Q_PROPERTY(int filterKeyColumn READ filterKeyColumn WRITE setFilterKeyColumn
               BINDABLE bindableFilterKeyColumn)
    Q_PROPERTY(bool dynamicSortFilter READ dynamicSortFilter WRITE setDynamicSortFilter
               BINDABLE bindableDynamicSortFilter)
    Q_PROPERTY(BobUI::CaseSensitivity filterCaseSensitivity READ filterCaseSensitivity
               WRITE setFilterCaseSensitivity NOTIFY filterCaseSensitivityChanged
               BINDABLE bindableFilterCaseSensitivity)
    Q_PROPERTY(BobUI::CaseSensitivity sortCaseSensitivity READ sortCaseSensitivity
               WRITE setSortCaseSensitivity NOTIFY sortCaseSensitivityChanged
               BINDABLE bindableSortCaseSensitivity)
    Q_PROPERTY(bool isSortLocaleAware READ isSortLocaleAware WRITE setSortLocaleAware
               NOTIFY sortLocaleAwareChanged BINDABLE bindableIsSortLocaleAware)
    Q_PROPERTY(int sortRole READ sortRole WRITE setSortRole NOTIFY sortRoleChanged
               BINDABLE bindableSortRole)
    Q_PROPERTY(int filterRole READ filterRole WRITE setFilterRole NOTIFY filterRoleChanged
               BINDABLE bindableFilterRole)
    Q_PROPERTY(bool recursiveFilteringEnabled READ isRecursiveFilteringEnabled
               WRITE setRecursiveFilteringEnabled NOTIFY recursiveFilteringEnabledChanged
               BINDABLE bindableRecursiveFilteringEnabled)
    Q_PROPERTY(bool autoAcceptChildRows READ autoAcceptChildRows WRITE setAutoAcceptChildRows
               NOTIFY autoAcceptChildRowsChanged BINDABLE bindableAutoAcceptChildRows)

public:
    explicit QSortFilterProxyModel(QObject *parent = nullptr);
    ~QSortFilterProxyModel();

    void setSourceModel(QAbstractItemModel *sourceModel) override;

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    QItemSelection mapSelectionToSource(const QItemSelection &proxySelection) const override;
    QItemSelection mapSelectionFromSource(const QItemSelection &sourceSelection) const override;

    QRegularExpression filterRegularExpression() const;
    QBindable<QRegularExpression> bindableFilterRegularExpression();

    int filterKeyColumn() const;
    void setFilterKeyColumn(int column);
    QBindable<int> bindableFilterKeyColumn();

    BobUI::CaseSensitivity filterCaseSensitivity() const;
    void setFilterCaseSensitivity(BobUI::CaseSensitivity cs);
    QBindable<BobUI::CaseSensitivity> bindableFilterCaseSensitivity();

    BobUI::CaseSensitivity sortCaseSensitivity() const;
    void setSortCaseSensitivity(BobUI::CaseSensitivity cs);
    QBindable<BobUI::CaseSensitivity> bindableSortCaseSensitivity();

    bool isSortLocaleAware() const;
    void setSortLocaleAware(bool on);
    QBindable<bool> bindableIsSortLocaleAware();

    int sortColumn() const;
    BobUI::SortOrder sortOrder() const;

    bool dynamicSortFilter() const;
    void setDynamicSortFilter(bool enable);
    QBindable<bool> bindableDynamicSortFilter();

    int sortRole() const;
    void setSortRole(int role);
    QBindable<int> bindableSortRole();

    int filterRole() const;
    void setFilterRole(int role);
    QBindable<int> bindableFilterRole();

    bool isRecursiveFilteringEnabled() const;
    void setRecursiveFilteringEnabled(bool recursive);
    QBindable<bool> bindableRecursiveFilteringEnabled();

    bool autoAcceptChildRows() const;
    void setAutoAcceptChildRows(bool accept);
    QBindable<bool> bindableAutoAcceptChildRows();

    enum class Direction {
        Rows        = 0x01,
        Columns     = 0x02,
        Both        = Rows | Columns,
    };
    Q_DECLARE_FLAGS(Directions, Direction)

public Q_SLOTS:
    void setFilterRegularExpression(const QString &pattern);
    void setFilterRegularExpression(const QRegularExpression &regularExpression);
    void setFilterWildcard(const QString &pattern);
    void setFilterFixedString(const QString &pattern);
    void invalidate();

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    virtual bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

    void beginFilterChange();
    void endFilterChange(Directions directions = Direction::Both);
#if BOBUI_DEPRECATED_SINCE(6, 13)
    BOBUI_DEPRECATED_VERSION_X_6_13("Use begin/endFilterChange() instead")
    void invalidateFilter();
    BOBUI_DEPRECATED_VERSION_X_6_13("Use begin/endFilterChange(QSortFilterProxyModel::Direction::Rows) instead")
    void invalidateRowsFilter();
    BOBUI_DEPRECATED_VERSION_X_6_13("Use begin/endFilterChange(QSortFilterProxyModel::Direction::Columns) instead")
    void invalidateColumnsFilter();
#endif

public:
    using QObject::parent;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = BobUI::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = BobUI::EditRole) override;

    QVariant headerData(int section, BobUI::Orientation orientation, int role = BobUI::DisplayRole) const override;
    bool setHeaderData(int section, BobUI::Orientation orientation,
            const QVariant &value, int role = BobUI::EditRole) override;

    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, BobUI::DropAction action,
                      int row, int column, const QModelIndex &parent) override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    void fetchMore(const QModelIndex &parent) override;
    bool canFetchMore(const QModelIndex &parent) const override;
    BobUI::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex buddy(const QModelIndex &index) const override;
    QModelIndexList match(const QModelIndex &start, int role,
                          const QVariant &value, int hits = 1,
                          BobUI::MatchFlags flags =
                          BobUI::MatchFlags(BobUI::MatchStartsWith|BobUI::MatchWrap)) const override;
    QSize span(const QModelIndex &index) const override;
    void sort(int column, BobUI::SortOrder order = BobUI::AscendingOrder) override;

    QStringList mimeTypes() const override;
    BobUI::DropActions supportedDropActions() const override;

Q_SIGNALS:
    void dynamicSortFilterChanged(bool dynamicSortFilter);
    void filterCaseSensitivityChanged(BobUI::CaseSensitivity filterCaseSensitivity);
    void sortCaseSensitivityChanged(BobUI::CaseSensitivity sortCaseSensitivity);
    void sortLocaleAwareChanged(bool sortLocaleAware);
    void sortRoleChanged(int sortRole);
    void filterRoleChanged(int filterRole);
    void recursiveFilteringEnabledChanged(bool recursiveFilteringEnabled);
    void autoAcceptChildRowsChanged(bool autoAcceptChildRows);

private:
    Q_DECLARE_PRIVATE(QSortFilterProxyModel)
    Q_DISABLE_COPY(QSortFilterProxyModel)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QSortFilterProxyModel::Directions)

BOBUI_END_NAMESPACE

#endif // QSORTFILTERPROXYMODEL_H
