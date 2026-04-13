// Copyright (C) 2013 David Faure <faure+bluesystems@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLOCKFILE_H
#define QLOCKFILE_H

#include <BobUICore/qstring.h>
#include <BobUICore/qscopedpointer.h>

#include <chrono>
#include <memory>

BOBUI_BEGIN_NAMESPACE

class QLockFilePrivate;

class Q_CORE_EXPORT QLockFile
{
public:
    explicit QLockFile(const QString &fileName);
    ~QLockFile();

    QString fileName() const;

    bool lock();
    BOBUI_CORE_INLINE_SINCE(6, 10)
    bool tryLock(int timeout);
    void unlock();

    BOBUI_CORE_INLINE_SINCE(6, 10)
    void setStaleLockTime(int);
    BOBUI_CORE_INLINE_SINCE(6, 10)
    int staleLockTime() const;

    bool tryLock(std::chrono::milliseconds timeout = std::chrono::milliseconds::zero());

    void setStaleLockTime(std::chrono::milliseconds value);
    std::chrono::milliseconds staleLockTimeAsDuration() const;

    bool isLocked() const;
    bool getLockInfo(qint64 *pid, QString *hostname, QString *appname) const;
    bool removeStaleLockFile();

    enum LockError {
        NoError = 0,
        LockFailedError = 1,
        PermissionError = 2,
        UnknownError = 3
    };
    LockError error() const;

private:
    std::unique_ptr<QLockFilePrivate> d_ptr;

    Q_DECLARE_PRIVATE(QLockFile)
    Q_DISABLE_COPY(QLockFile)
};

#if BOBUI_CORE_INLINE_IMPL_SINCE(6, 10)
bool QLockFile::tryLock(int timeout)
{
    return tryLock(std::chrono::milliseconds{timeout});
}

void QLockFile::setStaleLockTime(int staleLockTime)
{
    setStaleLockTime(std::chrono::milliseconds{staleLockTime});
}

int QLockFile::staleLockTime() const
{
    return int(staleLockTimeAsDuration().count());
}
#endif // BOBUI_CORE_INLINE_IMPL_SINCE(6, 10)

BOBUI_END_NAMESPACE

#endif // QLOCKFILE_H
