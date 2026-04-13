// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CALENDARINTERFACE_H
#define CALENDARINTERFACE_H

#include <QCalendar>
#include <QObject>

//![0]
class RequestedCalendarInterface
{
public:
    RequestedCalendarInterface() = default;
    virtual QCalendar::SystemId loadCalendar(QAnyStringView requested) = 0;
    virtual ~RequestedCalendarInterface() = default;
};
//![0]
BOBUI_BEGIN_NAMESPACE
//![1]
#define RequestedCalendarInterface_iid \
"org.bobui-project.BobUI.Examples.CalendarBackend.RequestedCalendarInterface/1.0"
Q_DECLARE_INTERFACE(RequestedCalendarInterface, RequestedCalendarInterface_iid)
//![1]
BOBUI_END_NAMESPACE

#endif // CALENDARINTERFACE_H
