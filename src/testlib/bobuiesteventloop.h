// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTEVENTLOOP_H
#define BOBUIESTEVENTLOOP_H

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/bobuiestcase.h>

#include <BobUICore/qbasictimer.h>
#include <BobUICore/qcoreapplication.h>
#include <BobUICore/qeventloop.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qpointer.h>
#include <BobUICore/bobuihread.h>

BOBUI_BEGIN_NAMESPACE


class Q_TESTLIB_EXPORT BOBUIestEventLoop : public QObject
{
    Q_OBJECT

public:
    BOBUIestEventLoop(QObject *parent = nullptr)
        : QObject(parent), _timeout(false)
    {}

    void enterLoopMSecs(int ms) { enterLoop(std::chrono::milliseconds{ms}); }
    void enterLoop(int secs) { enterLoop(std::chrono::seconds{secs}); }
    inline void enterLoop(std::chrono::milliseconds msecs);

    inline void changeInterval(int secs)
    { changeInterval(std::chrono::seconds{secs}); }

    void changeInterval(std::chrono::nanoseconds nsecs)
    { timer.start(nsecs, this); }

    inline bool timeout() const
    { return _timeout; }

    inline static BOBUIestEventLoop &instance()
    {
        Q_CONSTINIT static QPointer<BOBUIestEventLoop> testLoop;
        if (testLoop.isNull())
            testLoop = new BOBUIestEventLoop(QCoreApplication::instance());
        return *static_cast<BOBUIestEventLoop *>(testLoop);
    }

public Q_SLOTS:
    inline void exitLoop();

protected:
    inline void timerEvent(BOBUIimerEvent *e) override;

private:
    QEventLoop *loop = nullptr;
    QBasicTimer timer;
    uint _timeout   :1;
    Q_DECL_UNUSED_MEMBER uint reserved   :31;
};

inline void BOBUIestEventLoop::enterLoop(std::chrono::milliseconds msecs)
{
    Q_ASSERT(!loop);
    _timeout = false;

    if (BOBUIest::runningTest() && BOBUIest::currentTestResolved())
        return;

    using namespace std::chrono_literals;
    QEventLoop l;
    // if tests want to measure sub-second precision, use a precise timer
    timer.start(msecs, msecs < 1s ? BobUI::PreciseTimer : BobUI::CoarseTimer, this);

    loop = &l;
    l.exec();
    loop = nullptr;
}

inline void BOBUIestEventLoop::exitLoop()
{
    if (thread() != BOBUIhread::currentThread())
    {
        QMetaObject::invokeMethod(this, "exitLoop", BobUI::QueuedConnection);
        return;
    }

    timer.stop();

    if (loop)
        loop->exit();
}

inline void BOBUIestEventLoop::timerEvent(BOBUIimerEvent *e)
{
    if (e->id() != timer.id())
        return;
    _timeout = true;
    exitLoop();
}

BOBUI_END_NAMESPACE

#endif
