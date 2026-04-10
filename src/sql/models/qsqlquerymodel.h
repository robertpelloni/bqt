// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSQLQUERYMODEL_H
#define QSQLQUERYMODEL_H

#include <BobUISql/bobuisqlglobal.h>
#include <BobUICore/qabstractitemmodel.h>
#include <BobUISql/qsqldatabase.h>

BOBUI_REQUIRE_CONFIG(sqlmodel);

BOBUI_BEGIN_NAMESPACE

class QSqlQueryModelPrivate;
class QSqlError;
class QSqlRecord;
class QSqlQuery;

class Q_SQL_EXPORT QSqlQueryModel: public QAbstractTableModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSqlQueryModel)

public:
    explicit QSqlQueryModel(QObject *parent = nullptr);
    virtual ~QSqlQueryModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QSqlRecord record(int row) const;
    QSqlRecord record() const;

    QVariant data(const QModelIndex &item, int role = BobUI::DisplayRole) const override;
    QVariant headerData(int section, BobUI::Orientation orientation,
                        int role = BobUI::DisplayRole) const override;
    bool setHeaderData(int section, BobUI::Orientation orientation, const QVariant &value,
                       int role = BobUI::EditRole) override;

    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

#if BOBUI_REMOVAL_BOBUI7_DEPRECATED_SINCE(6, 2)
    BOBUI_DEPRECATED_VERSION_X_6_2("QSqlQuery is not meant to be copied. Pass it by move instead.")
    void setQuery(const QSqlQuery &query);
#endif
    void setQuery(QSqlQuery &&query);
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
    void refresh();
#if BOBUI_SQL_REMOVED_SINCE(6, 5)
    QSqlQuery query() const;
#endif
    const QSqlQuery &query(BOBUI6_DECL_NEW_OVERLOAD) const;

    virtual void clear();

    QSqlError lastError() const;

    void fetchMore(const QModelIndex &parent = QModelIndex()) override;
    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

protected:
    void beginInsertRows(const QModelIndex &parent, int first, int last);
    void endInsertRows();

    void beginRemoveRows(const QModelIndex &parent, int first, int last);
    void endRemoveRows();

    void beginInsertColumns(const QModelIndex &parent, int first, int last);
    void endInsertColumns();

    void beginRemoveColumns(const QModelIndex &parent, int first, int last);
    void endRemoveColumns();

    void beginResetModel();
    void endResetModel();
    virtual void queryChange();

    virtual QModelIndex indexInQuery(const QModelIndex &item) const;
    void setLastError(const QSqlError &error);
    QSqlQueryModel(QSqlQueryModelPrivate &dd, QObject *parent = nullptr);
};

BOBUI_END_NAMESPACE

#endif // QSQLQUERYMODEL_H
