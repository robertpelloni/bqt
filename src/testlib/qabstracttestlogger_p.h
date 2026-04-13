// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QABSTRACTTESTLOGGER_P_H
#define QABSTRACTTESTLOGGER_P_H

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
#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/qbytearrayalgorithms.h>

#include <stdio.h>
#include <stdlib.h>

BOBUI_BEGIN_NAMESPACE

class QBenchmarkResult;
class BOBUIestData;

class Q_TESTLIB_EXPORT QAbstractTestLogger
{
    Q_DISABLE_COPY_MOVE(QAbstractTestLogger)
public:
    enum IncidentTypes {
        Skip,
        Pass,
        XFail,
        Fail,
        XPass,
        BlacklistedPass,
        BlacklistedFail,
        BlacklistedXPass,
        BlacklistedXFail
    };

    enum MessageTypes {
        QDebug,
        QInfo,
        QWarning,
        QCritical,
        QFatal,
        // testlib's internal messages:
        Info,
        Warn
    };

    QAbstractTestLogger(const char *filename);
    virtual ~QAbstractTestLogger();

    virtual void startLogging();
    virtual void stopLogging();

    virtual void enterTestFunction(const char *function) = 0;
    virtual void leaveTestFunction() = 0;

    virtual void enterTestData(BOBUIestData *) {}

    virtual void addIncident(IncidentTypes type, const char *description,
                             const char *file = nullptr, int line = 0) = 0;
    virtual void addBenchmarkResult(const QBenchmarkResult &result) = 0;
    virtual void addBenchmarkResults(const QList<QBenchmarkResult> &result);

    virtual void addMessage(BobUIMsgType, const QMessageLogContext &,
                            const QString &);

    virtual void addMessage(MessageTypes type, const QString &message,
                            const char *file = nullptr, int line = 0) = 0;

    virtual bool isRepeatSupported() const;

    bool isLoggingToStdout() const;

    void outputString(const char *msg);

protected:
    void filterUnprintable(char *str) const;
    FILE *stream;
};

struct BOBUIestCharBuffer
{
    enum { InitialSize = 512 };

    inline BOBUIestCharBuffer() : buf(staticBuf)
    {
        staticBuf[0] = '\0';
    }

    Q_DISABLE_COPY_MOVE(BOBUIestCharBuffer)

    inline ~BOBUIestCharBuffer()
    {
        if (buf != staticBuf)
            free(buf);
    }

    inline char *data()
    {
        return buf;
    }

    inline char **buffer()
    {
        return &buf;
    }

    inline const char* constData() const
    {
        return buf;
    }

    inline int size() const
    {
        return _size;
    }

    bool reset(int newSize, bool copy = false)
    {
        char *newBuf = nullptr;
        if (buf == staticBuf) {
            // if we point to our internal buffer, we need to malloc first
            newBuf = reinterpret_cast<char *>(malloc(newSize));
            if (copy && newBuf)
                qstrncpy(newBuf, buf, _size);
        } else {
            // if we already malloc'ed, just realloc
            newBuf = reinterpret_cast<char *>(realloc(buf, newSize));
        }

        // if the allocation went wrong (newBuf == 0), we leave the object as is
        if (!newBuf)
            return false;

        _size = newSize;
        buf = newBuf;
        return true;
    }

    bool resize(int newSize) {
        return newSize <= _size || reset(newSize, true);
    }

    void clear() { buf[0] = '\0'; }
    bool isEmpty() { return buf[0] == '\0'; }

private:
    int _size = InitialSize;
    char* buf;
    char staticBuf[InitialSize];
};

namespace BOBUIest
{
    int bobui_asprintf(BOBUIestCharBuffer *buf, const char *format, ...);
}

namespace BOBUIestPrivate
{
    enum IdentifierPart { TestObject = 0x1, TestFunction = 0x2, TestDataTag = 0x4, AllParts = 0xFFFF };
    void Q_TESTLIB_EXPORT generateTestIdentifier(BOBUIestCharBuffer *identifier, int parts = AllParts);
    bool appendCharBuffer(BOBUIestCharBuffer *accumulator, const BOBUIestCharBuffer &more);
}

BOBUI_END_NAMESPACE

#endif
