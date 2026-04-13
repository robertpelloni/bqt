// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2022 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSYSTEMSEMAPHORE_P_H
#define QSYSTEMSEMAPHORE_P_H

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

#include "qsystemsemaphore.h"

#if BOBUI_CONFIG(systemsemaphore)

#include "qcoreapplication.h"
#include "bobuiipccommon_p.h"
#include "private/bobuicore-config_p.h"

#include <sys/types.h>
#if BOBUI_CONFIG(posix_sem)
#  include <semaphore.h>
#endif
#ifndef SEM_FAILED
#  define SEM_FAILED     nullptr
struct sem_t;
#endif
#if BOBUI_CONFIG(sysv_sem)
#  include <sys/sem.h>
#endif

BOBUI_BEGIN_NAMESPACE

class QSystemSemaphorePrivate;

struct QSystemSemaphorePosix
{
    static constexpr bool Enabled = BOBUI_CONFIG(posix_sem);
    static bool supports(QNativeIpcKey::Type type)
    { return type == QNativeIpcKey::Type::PosixRealtime; }
    static bool runtimeSupportCheck();

    bool handle(QSystemSemaphorePrivate *self, QSystemSemaphore::AccessMode mode);
    void cleanHandle(QSystemSemaphorePrivate *self);
    bool modifySemaphore(QSystemSemaphorePrivate *self, int count);

    sem_t *semaphore = SEM_FAILED;
    bool createdSemaphore = false;
};

struct QSystemSemaphoreSystemV
{
    static constexpr bool Enabled = BOBUI_CONFIG(sysv_sem);
    static bool supports(QNativeIpcKey::Type type)
    { return quint16(type) <= 0xff; }
    static bool runtimeSupportCheck();

#if BOBUI_CONFIG(sysv_sem)
    key_t handle(QSystemSemaphorePrivate *self, QSystemSemaphore::AccessMode mode);
    void cleanHandle(QSystemSemaphorePrivate *self);
    bool modifySemaphore(QSystemSemaphorePrivate *self, int count);

    QByteArray nativeKeyFile;
    key_t unix_key = -1;
    int semaphore = -1;
    bool createdFile = false;
    bool createdSemaphore = false;
#endif
};

struct QSystemSemaphoreWin32
{
#ifdef Q_OS_WIN32
    static constexpr bool Enabled = true;
#else
    static constexpr bool Enabled = false;
#endif
    static bool supports(QNativeIpcKey::Type type)
    { return type == QNativeIpcKey::Type::Windows; }
    static bool runtimeSupportCheck() { return Enabled; }

    // we can declare the members without the #if
    BobUI::HANDLE handle(QSystemSemaphorePrivate *self, QSystemSemaphore::AccessMode mode);
    void cleanHandle(QSystemSemaphorePrivate *self);
    bool modifySemaphore(QSystemSemaphorePrivate *self, int count);

    BobUI::HANDLE semaphore = nullptr;
};

class QSystemSemaphorePrivate
{
public:
    QSystemSemaphorePrivate(QNativeIpcKey::Type type) : nativeKey(type)
    { constructBackend(); }
    ~QSystemSemaphorePrivate() { destructBackend(); }

    void setWindowsErrorString(QLatin1StringView function);    // Windows only
    void setUnixErrorString(QLatin1StringView function);
    inline void setError(QSystemSemaphore::SystemSemaphoreError e, const QString &message)
    { error = e; errorString = message; }
    inline void clearError()
    { setError(QSystemSemaphore::NoError, QString()); }

    QNativeIpcKey nativeKey;
    QString errorString;
    int initialValue;
    QSystemSemaphore::SystemSemaphoreError error = QSystemSemaphore::NoError;

    union Backend {
        Backend() {}
        ~Backend() {}
        QSystemSemaphorePosix posix;
        QSystemSemaphoreSystemV sysv;
        QSystemSemaphoreWin32 win32;
    };
    BobUIIpcCommon::IpcStorageVariant<&Backend::posix, &Backend::sysv, &Backend::win32> backend;

    void constructBackend();
    void destructBackend();

    template <typename Lambda> auto visit(const Lambda &lambda)
    {
        return backend.visit(nativeKey.type(), lambda);
    }

    void handle(QSystemSemaphore::AccessMode mode)
    {
        visit([&](auto p) { p->handle(this, mode); });
    }
    void cleanHandle()
    {
        visit([&](auto p) { p->cleanHandle(this); });
    }
    bool modifySemaphore(int count)
    {
        return visit([&](auto p) { return p->modifySemaphore(this, count); });
    }
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(systemsemaphore)

#endif // QSYSTEMSEMAPHORE_P_H

