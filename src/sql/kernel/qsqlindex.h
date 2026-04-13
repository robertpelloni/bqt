// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSQLINDEX_H
#define QSQLINDEX_H

#include <BobUISql/bobuisqlglobal.h>
#include <BobUISql/qsqlrecord.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmetaobject.h>
#include <BobUICore/qstring.h>

// clazy:excludeall=qproperty-without-notify
BOBUI_BEGIN_NAMESPACE


class Q_SQL_EXPORT QSqlIndex : public QSqlRecord
{
    Q_GADGET
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString cursorName READ cursorName WRITE setCursorName)

public:
    explicit QSqlIndex(const QString &cursorName = QString(), const QString &name = QString());
    QSqlIndex(const QSqlIndex &other);
    QSqlIndex(QSqlIndex &&other) noexcept = default;
    ~QSqlIndex();
    QSqlIndex &operator=(const QSqlIndex &other);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QSqlIndex)

    void swap(QSqlIndex &other) noexcept {
        QSqlRecord::swap(other);
        cursor.swap(other.cursor);
        nm.swap(other.nm);
        sorts.swap(other.sorts);
    }

    void setCursorName(const QString &cursorName);
    inline QString cursorName() const { return cursor; }
    void setName(const QString& name);
    inline QString name() const { return nm; }

    void append(const QSqlField &field);
    void append(const QSqlField &field, bool desc);

    bool isDescending(int i) const;
    void setDescending(int i, bool desc);

private:
    // ### BobUI7: move to d-ptr
    QString cursor;
    QString nm;
    QList<bool> sorts;
};

Q_DECLARE_SHARED(QSqlIndex)

BOBUI_END_NAMESPACE

#endif // QSQLINDEX_H
