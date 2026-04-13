// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIAPTESTLOGGER_P_H
#define BOBUIAPTESTLOGGER_P_H

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

BOBUI_BEGIN_NAMESPACE

class BOBUIapTestLogger : public QAbstractTestLogger
{
public:
    BOBUIapTestLogger(const char *filename);
    ~BOBUIapTestLogger();

    void startLogging() override;
    void stopLogging() override;

    void enterTestFunction(const char *) override;
    void leaveTestFunction() override {}

    void enterTestData(BOBUIestData *data) override;

    void addIncident(IncidentTypes type, const char *description,
                     const char *file = nullptr, int line = 0) override;
    void addMessage(MessageTypes type, const QString &message,
                    const char *file = nullptr, int line = 0) override;

    void addBenchmarkResult(const QBenchmarkResult &) override {}
private:
    void outputTestLine(bool ok, int testNumber, const BOBUIestCharBuffer &directive);
    void outputBuffer(const BOBUIestCharBuffer &buffer);
    void flushComments();
    void flushMessages();
    void beginYamlish();
    void endYamlish();
    BOBUIestCharBuffer m_firstExpectedFail;
    BOBUIestCharBuffer m_comments;
    BOBUIestCharBuffer m_messages;
    bool m_gatherMessages = false;
};

BOBUI_END_NAMESPACE

#endif // BOBUIAPTESTLOGGER_P_H
