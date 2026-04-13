// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QJUNITTESTLOGGER_P_H
#define QJUNITTESTLOGGER_P_H

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

#include <BobUITest/bobuitestglobal.h>

#include <BobUITest/private/qabstracttestlogger_p.h>
#include <BobUITest/private/bobuiestelementattribute_p.h>
#include <BobUICore/qmutex.h>

#include <vector>

BOBUI_BEGIN_NAMESPACE

class BOBUIestJUnitStreamer;
class BOBUIestElement;

class QJUnitTestLogger : public QAbstractTestLogger
{
    public:
        QJUnitTestLogger(const char *filename);
        ~QJUnitTestLogger();

        void startLogging() override;
        void stopLogging() override;

        void enterTestFunction(const char *function) override;
        void leaveTestFunction() override;

        void enterTestData(BOBUIestData *) override;

        void addIncident(IncidentTypes type, const char *description,
                     const char *file = nullptr, int line = 0) override;
        void addMessage(MessageTypes type, const QString &message,
                    const char *file = nullptr, int line = 0) override;

        void addBenchmarkResult(const QBenchmarkResult &) override {}

    private:
        void enterTestCase(const char *name);
        void leaveTestCase();

        void addFailure(BOBUIest::LogElementType elementType,
            const char *failureType, const QString &failureDescription);

        BOBUIestElement *currentTestSuite = nullptr;
        std::vector<BOBUIestElement*> listOfTestcases;
        BOBUIestElement *currentTestCase = nullptr;
        BOBUIestElement *systemOutputElement = nullptr;
        BOBUIestElement *systemErrorElement = nullptr;
        BOBUIestJUnitStreamer *logFormatter = nullptr;
        // protects currentTestCase, systemOutputElement and systemErrorElement
        // in case of qDebug()/qWarning() etc. from threads
        QMutex mutex;

        int testCounter = 0;
        int failureCounter = 0;
        int errorCounter = 0;
};

BOBUI_END_NAMESPACE

#endif // QJUNITTESTLOGGER_P_H
