// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUISQLGLOBAL_H
#define BOBUISQLGLOBAL_H

#if 0
#pragma bobui_class(QSql)
#endif

#include <BobUICore/qglobal.h>
#include <BobUISql/bobuisql-config.h>
#include <BobUISql/bobuisqlexports.h>

BOBUI_BEGIN_NAMESPACE

namespace QSql
{
    enum Location
    {
        BeforeFirstRow = -1,
        AfterLastRow = -2
    };

    enum ParamTypeFlag
    {
        In = 0x00000001,
        Out = 0x00000002,
        InOut = In | Out,
        Binary = 0x00000004
    };
    Q_DECLARE_FLAGS(ParamType, ParamTypeFlag)

    enum TableType
    {
        Tables = 0x01,
        SystemTables = 0x02,
        Views = 0x04,
        AllTables = 0xff
    };

    enum NumericalPrecisionPolicy
    {
        LowPrecisionInt32    = 0x01,
        LowPrecisionInt64    = 0x02,
        LowPrecisionDouble   = 0x04,

        HighPrecision        = 0
    };
}

Q_DECLARE_OPERATORS_FOR_FLAGS(QSql::ParamType)

BOBUI_END_NAMESPACE

#endif // QSQL_H
