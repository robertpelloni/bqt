// Copyright (C) 2013 David Faure <faure+bluesystems@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLOCKFILE_P_H
#define QLOCKFILE_P_H

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

#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/qlockfile.h>
#include <BobUICore/qfile.h>

BOBUI_BEGIN_NAMESPACE

class QLockFilePrivate
{
public:
    struct LockFileInfo
    {
        qint64 pid;
        QString appname;
        QString hostname;
        QByteArray hostid;
        QByteArray bootid;
    };

    explicit QLockFilePrivate(const QString &fn);
    ~QLockFilePrivate();

    QLockFile::LockError tryLock_sys();
    bool removeStaleLock();
    QByteArray lockFileContents() const;
    // Returns \c true if the lock belongs to dead PID, or is old.
    // The attempt to delete it will tell us if it was really stale or not, though.
    bool isApparentlyStale() const;

    // used in dbusmenu
    Q_CORE_EXPORT static QString processNameByPid(qint64 pid);
    static bool isProcessRunning(qint64 pid, const QString &appname);

    QString fileName;

    static bool getLockInfo_helper(const QString &fileName, LockFileInfo *info);
    static int openNewFileDescriptor(const QString &fileName);

#ifdef Q_OS_WIN
    BobUI::HANDLE fileHandle;
#else
    int fileHandle;
#endif

    std::chrono::milliseconds staleLockTime = std::chrono::seconds{30};
    QLockFile::LockError lockError = QLockFile::NoError;
    bool isLocked = false;

    // used in tst_QLockFile:
    static auto getLockFileHandle(QLockFile *f)
    {
        return f->d_func()->fileHandle;
    }
};

BOBUI_END_NAMESPACE

#endif /* QLOCKFILE_P_H */
