// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSQLCACHEDRESULT_P_H
#define QSQLCACHEDRESULT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUISql/private/bobuisqlglobal_p.h>
#include "BobUISql/qsqlresult.h"
#include "BobUISql/private/qsqlresult_p.h"
#include <BobUICore/qcontainerfwd.h>

BOBUI_BEGIN_NAMESPACE

class QVariant;

class QSqlCachedResultPrivate;

class Q_SQL_EXPORT QSqlCachedResult: public QSqlResult
{
    Q_DECLARE_PRIVATE(QSqlCachedResult)

public:
    typedef QList<QVariant> ValueCache;

protected:
    QSqlCachedResult(QSqlCachedResultPrivate &d);

    void init(int colCount);
    void cleanup();
    void clearValues();

    virtual bool gotoNext(ValueCache &values, int index) = 0;

    QVariant data(int i) override;
    bool isNull(int i) override;
    bool fetch(int i) override;
    bool fetchNext() override;
    bool fetchPrevious() override;
    bool fetchFirst() override;
    bool fetchLast() override;

    int colCount() const;
    ValueCache &cache();

    void virtual_hook(int id, void *data) override;
    void detachFromResultSet() override;
    void setNumericalPrecisionPolicy(QSql::NumericalPrecisionPolicy policy) override;
private:
    bool cacheNext();
};

class Q_SQL_EXPORT QSqlCachedResultPrivate: public QSqlResultPrivate
{
    Q_DECLARE_PUBLIC(QSqlCachedResult)

public:
    using QSqlResultPrivate::QSqlResultPrivate;

    bool canSeek(int i) const;
    inline int cacheCount() const;
    void init(int count, bool fo);
    void cleanup();
    int nextIndex();
    void revertLast();

    QSqlCachedResult::ValueCache cache;
    int rowCacheEnd = 0;
    int colCount = 0;
    bool atEnd = false;
};

BOBUI_END_NAMESPACE

#endif // QSQLCACHEDRESULT_P_H
