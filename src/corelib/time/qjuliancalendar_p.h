// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QJULIAN_CALENDAR_P_H
#define QJULIAN_CALENDAR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of calendar implementations.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qromancalendar_p.h"

BOBUI_BEGIN_NAMESPACE

class Q_CORE_EXPORT QJulianCalendar : public QRomanCalendar
{
public:
    // Calendar properties:
    QString name() const override;
    static QStringList nameList();
    // Date queries:
    bool isLeapYear(int year) const override;
    // Julian Day conversions:
    bool dateToJulianDay(int year, int month, int day, qint64 *jd) const override;
    QCalendar::YearMonthDay julianDayToDate(qint64 jd) const override;
};

BOBUI_END_NAMESPACE

#endif // QJULIAN_CALENDAR_P_H
