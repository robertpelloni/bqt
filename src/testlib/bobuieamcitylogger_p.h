// Copyright (C) 2016 Borgar Ovsthus
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEAMCITYLOGGER_P_H
#define BOBUIEAMCITYLOGGER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUITest/private/qabstracttestlogger_p.h>

#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIeamCityLogger : public QAbstractTestLogger
{
public:
    BOBUIeamCityLogger(const char *filename);
    ~BOBUIeamCityLogger();

    void startLogging() override;
    void stopLogging() override;

    void enterTestFunction(const char *function) override;
    void leaveTestFunction() override;

    void addIncident(IncidentTypes type, const char *description,
                     const char *file = nullptr, int line = 0) override;
    void addBenchmarkResult(const QBenchmarkResult &result) override;

    void addMessage(MessageTypes type, const QString &message,
                    const char *file = nullptr, int line = 0) override;

private:
    BOBUIestCharBuffer currTestFuncName;
    BOBUIestCharBuffer pendingMessages;
    BOBUIestCharBuffer flowID;

    void tcEscapedString(BOBUIestCharBuffer *buf, const char *str) const;
    void escapedTestFuncName(BOBUIestCharBuffer *buf) const;
    void addPendingMessage(const char *type, const char *msg, const char *file, int line);
};

BOBUI_END_NAMESPACE

#endif
