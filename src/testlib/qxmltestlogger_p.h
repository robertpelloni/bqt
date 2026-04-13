// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QXMLTESTLOGGER_P_H
#define QXMLTESTLOGGER_P_H

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

class QXmlTestLogger : public QAbstractTestLogger
{
public:
    enum XmlMode { Complete = 0, Light };

    QXmlTestLogger(XmlMode mode, const char *filename);
    ~QXmlTestLogger();

    void startLogging() override;
    void stopLogging() override;

    void enterTestFunction(const char *function) override;
    void leaveTestFunction() override;

    void addIncident(IncidentTypes type, const char *description,
                     const char *file = nullptr, int line = 0) override;
    void addBenchmarkResult(const QBenchmarkResult &result) override;

    void addMessage(MessageTypes type, const QString &message,
                    const char *file = nullptr, int line = 0) override;

    [[nodiscard]] static bool xmlCdata(BOBUIestCharBuffer *dest, char const *src);
    [[nodiscard]] static bool xmlQuote(BOBUIestCharBuffer *dest, char const *src);
private:
    [[nodiscard]] static int xmlCdata(BOBUIestCharBuffer *dest, char const *src, qsizetype n);
    [[nodiscard]] static int xmlQuote(BOBUIestCharBuffer *dest, char const *src, qsizetype n);

    XmlMode xmlmode;
};

BOBUI_END_NAMESPACE

#endif
