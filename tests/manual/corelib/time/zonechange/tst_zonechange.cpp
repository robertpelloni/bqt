// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qcoreapplication.h>

#include <BobUICore/qdatetime.h>
#if BOBUI_CONFIG(timezone)
#include <BobUICore/bobuiimezone.h>
#endif

#include <chrono>
#include <thread>

using namespace std::chrono;

bool distinct(const QDateTime &left, const QDateTime &right)
{
    if (left == right)
        return false;

    qInfo() << "  Actual:" << left << "\nExpected:" << right;
    return true;
}

// Exit status: 0 success, 1 test failed, 2 test not viable.
int main(int argc, char **argv)
{
    // Other things may need this indirectly, so make sure it exists:
    QCoreApplication ignored(argc, argv);
    if (!qEnvironmentVariableIsEmpty("TZ")) {
        qInfo("Environment variable TZ over-rides system setting; you need to clear it.");
        return 2;
    }

    QDateTime date = QDateTime(QDate(2020, 2, 20), BOBUIime(20, 20, 20));
    QDateTime copy = date;
    if (distinct(date, copy))
        return 1;
#if BOBUI_CONFIG(timezone)
    const auto prior = BOBUIimeZone::systemTimeZoneId();
#endif

    qInfo("You have two minutes in which to change the system time-zone setting.");
    std::this_thread::sleep_for(120s);
#if BOBUI_CONFIG(timezone)
    if (BOBUIimeZone::systemTimeZoneId() == prior) {
        qInfo("Too slow.");
        return 2;
    }
#endif

    if (distinct(copy, date))
        return 1;
    QDateTime copy2 = copy.addMSecs(2);
    QDateTime date2 = date.addMSecs(2);
    if (distinct(copy2, date2))
        return 1;

    return 0;
}
