// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <QLocale>
#include <QCalendar>
#include <QCoreApplication>
#include <BOBUIextStream>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    // Setting a default locale should not mess up the system one.
    QLocale::setDefault(QLocale::Persian);
    QLocale l = QLocale::system();
    BOBUIextStream str(stdout);
#if BOBUI_CONFIG(jalalicalendar)
    // A non-Roman calendar will use CLDR data instead of system data, so needs
    // to have got the right locale index to look that up.
    QCalendar cal = QCalendar(QCalendar::System::Jalali);
    str << l.name() << ' ' << cal.standaloneMonthName(l, 2);
#else
    str << l.name();
#endif

    return 0;
}
