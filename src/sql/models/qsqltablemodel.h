// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSQLTABLEMODEL_H
#define QSQLTABLEMODEL_H

#include <BobUISql/bobuisqlglobal.h>
#include <BobUISql/qsqldatabase.h>
#include <BobUISql/qsqlquerymodel.h>

BOBUI_REQUIRE_CONFIG(sqlmodel);

BOBUI_BEGIN_NAMESPACE


class QSqlTableModelPrivate;
class QSqlRecord;
class QSqlField;
class QSqlIndex;

class Q_SQL_EXPORT QSqlTableModel: public QSqlQueryModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSqlTableModel)
    Q_MOC_INCLUDE(<BobUISql/qsqlrecord.h>)

public:
    enum EditStrategy {OnFieldChange, OnRowChange, OnManualSubmit};

    explicit QSqlTableModel(QObject *parent = nullptr, const QSqlDatabase &db = QSqlDatabase());
    virtual ~QSqlTableModel();

    virtual void setTable(const QString &tableName);
    QString tableName() const;

    BobUI::ItemFlags flags(const QModelIndex &index) const override;

    QSqlRecord record() const;
    QSqlRecord record(int row) const;
    QVariant data(const QModelIndex &idx, int role = BobUI::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = BobUI::EditRole) override;
    bool clearItemData(const QModelIndex &index) override;

    QVariant headerData(int section, BobUI::Orientation orientation, int role = BobUI::DisplayRole) const override;

    bool isDirty() const;
    bool isDirty(const QModelIndex &index) const;

    void clear() override;

    virtual void setEditStrategy(EditStrategy strategy);
    EditStrategy editStrategy() const;

    QSqlIndex primaryKey() const;
    QSqlDatabase database() const;
    int fieldIndex(const QString &fieldName) const;

    void sort(int column, BobUI::SortOrder order) override;
    virtual void setSort(int column, BobUI::SortOrder order);

    QString filter() const;
    virtual void setFilter(const QString &filter);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool insertRecord(int row, const QSqlRecord &record);
    bool setRecord(int row, const QSqlRecord &record);

    virtual void revertRow(int row);

public Q_SLOTS:
    virtual bool select();
    virtual bool selectRow(int row);

    bool submit() override;
    void revert() override;

    bool submitAll();
    void revertAll();

Q_SIGNALS:
    void primeInsert(int row, QSqlRecord &record);

    void beforeInsert(QSqlRecord &record);
    void beforeUpdate(int row, QSqlRecord &record);
    void beforeDelete(int row);

protected:
    QSqlTableModel(QSqlTableModelPrivate &dd, QObject *parent = nullptr, const QSqlDatabase &db = QSqlDatabase());

    virtual bool updateRowInTable(int row, const QSqlRecord &values);
    virtual bool insertRowIntoTable(const QSqlRecord &values);
    virtual bool deleteRowFromTable(int row);
    virtual QString orderByClause() const;
    virtual QString selectStatement() const;

    void setPrimaryKey(const QSqlIndex &key);
#if BOBUI_SQL_REMOVED_SINCE(6, 5)
    void setQuery(const QSqlQuery &query);
#endif
    QModelIndex indexInQuery(const QModelIndex &item) const override;
    QSqlRecord primaryValues(int row) const;
};

BOBUI_END_NAMESPACE

#endif // QSQLTABLEMODEL_H
