// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CALENDARPLUGIN_H
#define CALENDARPLUGIN_H

#include "calendarbackend.h"
#include "calendarBackendInterface.h"

#include <BobUIPlugin>
//![0]
class JulianGregorianPlugin : public QObject, public RequestedCalendarInterface
{
    Q_OBJECT
    Q_INTERFACES(RequestedCalendarInterface)
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.Examples."
                          "CalendarBackend."
                          "RequestedCalendarInterface/1.0")
public:
    JulianGregorianPlugin();
    QCalendar::SystemId loadCalendar(QAnyStringView request) override;
    ~JulianGregorianPlugin();
};
//![0]
#endif // CALENDARPLUGIN_H
