// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QAPPLETESTLOGGER_P_H
#define QAPPLETESTLOGGER_P_H

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

#include <BobUICore/private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE

#if defined(BOBUI_USE_APPLE_UNIFIED_LOGGING)
class QAppleTestLogger : public QAbstractTestLogger
{
public:
    static bool debugLoggingEnabled();

    QAppleTestLogger();

    void enterTestFunction(const char *function) override;
    void leaveTestFunction() override;

    void addIncident(IncidentTypes type, const char *description,
                     const char *file = nullptr, int line = 0) override;
    void addMessage(BobUIMsgType, const QMessageLogContext &,
            const QString &) override;
    void addMessage(MessageTypes type, const QString &message,
                            const char *file = nullptr, int line = 0) override;

    void addBenchmarkResult(const QBenchmarkResult &result) override
    { Q_UNUSED(result); }

private:
    QString subsystem() const;
    QString testIdentifier() const;
};
#endif

BOBUI_END_NAMESPACE

#endif // QAPPLETESTLOGGER_P_H
