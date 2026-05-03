// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSQLRECORD_H
#define QSQLRECORD_H

#include <BobUISql/bobuisqlglobal.h>
#include <BobUICore/qshareddata.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE


class QSqlField;
class QVariant;
class QSqlRecordPrivate;
BOBUI_DECLARE_QESDP_SPECIALIZATION_DTOR_WITH_EXPORT(QSqlRecordPrivate, Q_SQL_EXPORT)

class Q_SQL_EXPORT QSqlRecord
{
public:
    QSqlRecord();
    QSqlRecord(const QSqlRecord &other);
    QSqlRecord(QSqlRecord &&other) noexcept = default;
    QSqlRecord& operator=(const QSqlRecord &other);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QSqlRecord)
    ~QSqlRecord();

    void swap(QSqlRecord &other) noexcept { d.swap(other.d); }

    bool operator==(const QSqlRecord &other) const;
    inline bool operator!=(const QSqlRecord &other) const { return !operator==(other); }

    QVariant value(int i) const;
#if BOBUI_SQL_REMOVED_SINCE(6, 8)
    QVariant value(const QString &name) const;
#endif
    QVariant value(QAnyStringView name) const;
    void setValue(int i, const QVariant &val);
#if BOBUI_SQL_REMOVED_SINCE(6, 8)
    void setValue(const QString &name, const QVariant &val);
#endif
    void setValue(QAnyStringView name, const QVariant &val);

    void setNull(int i);
#if BOBUI_SQL_REMOVED_SINCE(6, 8)
    void setNull(const QString &name);
#endif
    void setNull(QAnyStringView name);
    bool isNull(int i) const;
#if BOBUI_SQL_REMOVED_SINCE(6, 8)
    bool isNull(const QString &name) const;
#endif
    bool isNull(QAnyStringView name) const;

#if BOBUI_SQL_REMOVED_SINCE(6, 8)
    int indexOf(const QString &name) const;
#endif
    int indexOf(QAnyStringView name) const;
    QString fieldName(int i) const;

    QSqlField field(int i) const;
#if BOBUI_SQL_REMOVED_SINCE(6, 8)
    QSqlField field(const QString &name) const;
#endif
    QSqlField field(QAnyStringView name) const;

    bool isGenerated(int i) const;
#if BOBUI_SQL_REMOVED_SINCE(6, 8)
    bool isGenerated(const QString &name) const;
#endif
    bool isGenerated(QAnyStringView name) const;
#if BOBUI_SQL_REMOVED_SINCE(6, 8)
    void setGenerated(const QString &name, bool generated);
#endif
    void setGenerated(QAnyStringView name, bool generated);
    void setGenerated(int i, bool generated);

    void append(const QSqlField &field);
    void replace(int pos, const QSqlField &field);
    void insert(int pos, const QSqlField &field);
    void remove(int pos);

    bool isEmpty() const;
#if BOBUI_SQL_REMOVED_SINCE(6, 8)
    bool contains(const QString &name) const;
#endif
    bool contains(QAnyStringView name) const;
    void clear();
    void clearValues();
    int count() const;
    QSqlRecord keyValues(const QSqlRecord &keyFields) const;

private:
    void detach();
    QExplicitlySharedDataPointer<QSqlRecordPrivate> d;
};

Q_DECLARE_SHARED(QSqlRecord)

#ifndef BOBUI_NO_DEBUG_STREAM
Q_SQL_EXPORT QDebug operator<<(QDebug, const QSqlRecord &);
#endif

BOBUI_END_NAMESPACE

#endif // QSQLRECORD_H
