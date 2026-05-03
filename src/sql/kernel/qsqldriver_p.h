// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSQLDRIVER_P_H
#define QSQLDRIVER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists for the convenience
// of the BobUISQL module. This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include <BobUISql/private/bobuisqlglobal_p.h>
#include "private/qobject_p.h"
#include "qsqldriver.h"
#include "qsqlerror.h"

BOBUI_BEGIN_NAMESPACE

class QSqlDriverPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QSqlDriver)

public:
    QSqlDriverPrivate(QSqlDriver::DbmsType type = QSqlDriver::UnknownDbms)
      : QObjectPrivate(),
        dbmsType(type)
    { }

    QString connectionName;
    QSqlError error;
    QSql::NumericalPrecisionPolicy precisionPolicy = QSql::LowPrecisionDouble;
    QSqlDriver::DbmsType dbmsType;
    bool isOpen = false;
    bool isOpenError = false;
};

BOBUI_END_NAMESPACE

#endif // QSQLDRIVER_P_H
