// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSHAREDMEMORY_P_H
#define QSHAREDMEMORY_P_H

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

#include "qsharedmemory.h"

#include <BobUICore/qstring.h>

#if BOBUI_CONFIG(sharedmemory)
#include "qsystemsemaphore.h"
#include "bobuiipccommon_p.h"
#include "private/qobject_p.h"

#if BOBUI_CONFIG(posix_shm)
#  include <sys/mman.h>
#endif
#if BOBUI_CONFIG(sysv_shm)
#  include <sys/shm.h>
#endif

BOBUI_BEGIN_NAMESPACE

class QSharedMemoryPrivate;

#if BOBUI_CONFIG(systemsemaphore)
/*!
  Helper class
  */
class QSharedMemoryLocker
{
    Q_DISABLE_COPY(QSharedMemoryLocker)
public:
    Q_NODISCARD_CTOR explicit QSharedMemoryLocker(QSharedMemory *sharedMemory)
        : q_sm(sharedMemory)
    {
        Q_ASSERT(q_sm);
    }

    Q_NODISCARD_CTOR QSharedMemoryLocker(QSharedMemoryLocker &&other) noexcept
        : q_sm{std::exchange(other.q_sm, nullptr)}
    {}

    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QSharedMemoryLocker)

    void swap(QSharedMemoryLocker &other) noexcept
    { bobui_ptr_swap(q_sm, other.q_sm); }

    inline ~QSharedMemoryLocker()
    {
        if (q_sm)
            q_sm->unlock();
    }

    inline bool lock()
    {
        if (q_sm && q_sm->lock())
            return true;
        q_sm = nullptr;
        return false;
    }

private:
    friend void swap(QSharedMemoryLocker &lhs, QSharedMemoryLocker &rhs) noexcept
    { lhs.swap(rhs); }

    QSharedMemory *q_sm;
};
#endif // BOBUI_CONFIG(systemsemaphore)

class QSharedMemoryPosix
{
public:
    static constexpr bool Enabled = BOBUI_CONFIG(posix_shm);
    static bool supports(QNativeIpcKey::Type type)
    { return type == QNativeIpcKey::Type::PosixRealtime; }
    static bool runtimeSupportCheck();

    bool handle(QSharedMemoryPrivate *self);
    bool cleanHandle(QSharedMemoryPrivate *self);
    bool create(QSharedMemoryPrivate *self, qsizetype size);
    bool attach(QSharedMemoryPrivate *self, QSharedMemory::AccessMode mode);
    bool detach(QSharedMemoryPrivate *self);

    int hand = -1;
};

class QSharedMemorySystemV
{
public:
    static constexpr bool Enabled = BOBUI_CONFIG(sysv_shm);
    static bool supports(QNativeIpcKey::Type type)
    { return quint16(type) <= 0xff; }
    static bool runtimeSupportCheck();

#if BOBUI_CONFIG(sysv_shm)
    key_t handle(QSharedMemoryPrivate *self);
    bool cleanHandle(QSharedMemoryPrivate *self);
    bool create(QSharedMemoryPrivate *self, qsizetype size);
    bool attach(QSharedMemoryPrivate *self, QSharedMemory::AccessMode mode);
    bool detach(QSharedMemoryPrivate *self);

private:
    void updateNativeKeyFile(const QNativeIpcKey &nativeKey);

    QByteArray nativeKeyFile;
    key_t unix_key = 0;
#endif
};

class QSharedMemoryWin32
{
public:
#ifdef Q_OS_WIN32
    static constexpr bool Enabled = true;
#else
    static constexpr bool Enabled = false;
#endif
    static bool runtimeSupportCheck() { return Enabled; }
    static bool supports(QNativeIpcKey::Type type)
    { return type == QNativeIpcKey::Type::Windows; }

    BobUI::HANDLE handle(QSharedMemoryPrivate *self);
    bool cleanHandle(QSharedMemoryPrivate *self);
    bool create(QSharedMemoryPrivate *self, qsizetype size);
    bool attach(QSharedMemoryPrivate *self, QSharedMemory::AccessMode mode);
    bool detach(QSharedMemoryPrivate *self);

    BobUI::HANDLE hand = nullptr;
};

class Q_AUTOTEST_EXPORT QSharedMemoryPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QSharedMemory)

public:
    QSharedMemoryPrivate(QNativeIpcKey::Type type) : nativeKey(type)
    { constructBackend(); }
    ~QSharedMemoryPrivate();

    void *memory = nullptr;
    qsizetype size = 0;
    QNativeIpcKey nativeKey;
    QString errorString;
#if BOBUI_CONFIG(systemsemaphore)
    using SemaphoreAccessMode = QSystemSemaphore::AccessMode;
    QSystemSemaphore systemSemaphore{ QNativeIpcKey() };
    bool lockedByMe = false;
#else
    enum SemaphoreAccessMode {};
#endif
    QSharedMemory::SharedMemoryError error = QSharedMemory::NoError;

    union Backend {
        Backend() {}
        ~Backend() {}
        QSharedMemoryPosix posix;
        QSharedMemorySystemV sysv;
        QSharedMemoryWin32 win32;
    };
    BobUIIpcCommon::IpcStorageVariant<&Backend::posix, &Backend::sysv, &Backend::win32> backend;

    void constructBackend();
    void destructBackend();
    bool initKey(SemaphoreAccessMode mode);

    template <typename Lambda> auto visit(const Lambda &lambda)
    {
        return backend.visit(nativeKey.type(), lambda);
    }

    bool handle()
    {
        return visit([&](auto p) { return !!p->handle(this); });
    }
    bool cleanHandle()
    {
        return visit([&](auto p) { return p->cleanHandle(this); });
    }
    bool create(qsizetype sz)
    {
        return visit([&](auto p) { return p->create(this, sz); });
    }
    bool attach(QSharedMemory::AccessMode mode)
    {
        return visit([&](auto p) { return p->attach(this, mode); });
    }
    bool detach()
    {
        return visit([&](auto p) { return p->detach(this); });
    }

    inline void setError(QSharedMemory::SharedMemoryError e, const QString &message)
    { error = e; errorString = message; }
    void setUnixErrorString(QLatin1StringView function);
    void setWindowsErrorString(QLatin1StringView function);

#if BOBUI_CONFIG(systemsemaphore)
    bool tryLocker(QSharedMemoryLocker *locker, const QString &function) {
        if (!locker->lock()) {
            errorString = QSharedMemory::tr("%1: unable to lock").arg(function);
            error = QSharedMemory::LockError;
            return false;
        }
        return true;
    }
    QNativeIpcKey semaphoreNativeKey() const;
#endif // BOBUI_CONFIG(systemsemaphore)
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(sharedmemory)

#endif // QSHAREDMEMORY_P_H

