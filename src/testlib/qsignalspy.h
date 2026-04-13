// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSIGNALSPY_H
#define QSIGNALSPY_H

#include <BobUICore/qbytearray.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmetaobject.h>
#include <BobUITest/bobuiesteventloop.h>
#include <BobUICore/qvariant.h>
#include <BobUICore/qmutex.h>

#include <memory>

BOBUI_BEGIN_NAMESPACE


class QVariant;
class QSignalSpyPrivate;
class QSignalSpy : public QList<QList<QVariant> >
{
    struct ObjectSignal {
        const QObject *obj;
        QMetaMethod sig;
    };
    friend class QSignalSpyPrivate;
    std::unique_ptr<QSignalSpyPrivate> d_ptr;
public:
    explicit QSignalSpy(const QObject *obj, const char *aSignal)
        : QSignalSpy(verify(obj, aSignal)) {}
#ifdef Q_QDOC
    template <typename PointerToMemberFunction>
    QSignalSpy(const QObject *object, PointerToMemberFunction signal);
#else
    template <typename Func>
    QSignalSpy(const typename BobUIPrivate::FunctionPointer<Func>::Object *obj, Func signal0)
        : QSignalSpy(verify(obj, QMetaMethod::fromSignal(signal0))) {}
#endif // Q_QDOC
    QSignalSpy(const QObject *obj, QMetaMethod signal)
        : QSignalSpy(verify(obj, signal)) {}
    Q_TESTLIB_EXPORT ~QSignalSpy();

    bool isValid() const noexcept { return d_ptr != nullptr; }
    inline QByteArray signal() const { return sig; }

    bool wait(int timeout)
    { return wait(std::chrono::milliseconds{timeout}); }

    Q_TESTLIB_EXPORT bool wait(std::chrono::milliseconds timeout = std::chrono::seconds{5});

private:
    Q_TESTLIB_EXPORT explicit QSignalSpy(ObjectSignal os);

    Q_TESTLIB_EXPORT static ObjectSignal verify(const QObject *obj, QMetaMethod signal);
    Q_TESTLIB_EXPORT static ObjectSignal verify(const QObject *obj, const char *aSignal);

    Q_TESTLIB_EXPORT void appendArgs(void **a);

    // the full, normalized signal name
    const QByteArray sig;
    // holds the QMetaType types for the argument list of the signal
    const QList<int> args;

    BOBUIestEventLoop m_loop;
    bool m_waiting = false;
    QMutex m_mutex; // protects m_waiting and the QList base class, between appendArgs() and wait()
};

BOBUI_END_NAMESPACE

#endif
