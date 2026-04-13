// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSQLTABLEMODEL_P_H
#define QSQLTABLEMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qsql*model.h .  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include <BobUISql/private/bobuisqlglobal_p.h>
#include "private/qsqlquerymodel_p.h"
#include "BobUISql/qsqlindex.h"
#include "BobUICore/qmap.h"

BOBUI_REQUIRE_CONFIG(sqlmodel);

BOBUI_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QSqlTableModelPrivate: public QSqlQueryModelPrivate
{
    Q_DECLARE_PUBLIC(QSqlTableModel)

public:
    QSqlTableModelPrivate()
        : sortColumn(-1),
          sortOrder(BobUI::AscendingOrder),
          strategy(QSqlTableModel::OnRowChange),
          busyInsertingRows(false)
    {}

    void clear();
    virtual void clearCache();
    QSqlRecord record(const QList<QVariant> &values) const;

    bool exec(const QString &stmt, bool prepStatement,
              const QSqlRecord &rec, const QSqlRecord &whereValues);
    virtual void revertCachedRow(int row);
    virtual int nameToIndex(const QString &name) const;
    QString strippedFieldName(const QString &name) const;
    int insertCount(int maxRow = -1) const;
    void initRecordAndPrimaryIndex();

    QSqlDatabase db;

    int sortColumn;
    BobUI::SortOrder sortOrder;

    QSqlTableModel::EditStrategy strategy;
    bool busyInsertingRows;

    QSqlQuery editQuery = { QSqlQuery(nullptr) };
    QSqlIndex primaryIndex;
    QString tableName;
    QString filter;
    QString autoColumn;

    enum Op { None, Insert, Update, Delete };

    class ModifiedRow
    {
    public:
        inline ModifiedRow(Op o = None, const QSqlRecord &r = QSqlRecord())
            : m_op(None), m_db_values(r), m_insert(o == Insert)
        { setOp(o); }
        inline Op op() const { return m_op; }
        inline void setOp(Op o)
        {
            if (o == None)
                m_submitted = true;
            if (o == m_op)
                return;
            m_submitted = (o != Insert && o != Delete);
            m_op = o;
            m_rec = m_db_values;
            setGenerated(m_rec, m_op == Delete);
        }
        inline const QSqlRecord &rec() const { return m_rec; }
        inline QSqlRecord& recRef() { return m_rec; }
        inline void setValue(int c, const QVariant &v)
        {
            m_submitted = false;
            m_rec.setValue(c, v);
            m_rec.setGenerated(c, true);
        }
        inline bool submitted() const { return m_submitted; }
        inline void setSubmitted()
        {
            m_submitted = true;
            setGenerated(m_rec, false);
            if (m_op == Delete) {
                m_rec.clearValues();
            }
            else {
                m_op = Update;
                m_db_values = m_rec;
                setGenerated(m_db_values, true);
            }
        }
        inline void refresh(bool exists, const QSqlRecord& newvals)
        {
            m_submitted = true;
            if (exists) {
                m_op = Update;
                m_db_values = newvals;
                m_rec = newvals;
                setGenerated(m_rec, false);
            } else {
                m_op = Delete;
                m_rec.clear();
                m_db_values.clear();
            }
        }
        inline bool insert() const { return m_insert; }
        inline void revert()
        {
            if (m_submitted)
                return;
            if (m_op == Delete)
                m_op = Update;
            m_rec = m_db_values;
            setGenerated(m_rec, false);
            m_submitted = true;
        }
        inline QSqlRecord primaryValues(const QSqlRecord& pi) const
        {
            if (m_op == None || m_op == Insert)
                return QSqlRecord();

            return m_db_values.keyValues(pi);
        }
    private:
        inline static void setGenerated(QSqlRecord& r, bool g)
        {
            for (int i = r.count() - 1; i >= 0; --i)
                r.setGenerated(i, g);
        }
        Op m_op;
        QSqlRecord m_rec;
        QSqlRecord m_db_values;
        bool m_submitted;
        bool m_insert;
    };

    typedef QMap<int, ModifiedRow> CacheMap;
    CacheMap cache;
};

BOBUI_END_NAMESPACE

#endif // QSQLTABLEMODEL_P_H
