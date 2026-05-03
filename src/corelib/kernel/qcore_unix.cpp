// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/qbasicatomic.h>
#include "qcore_unix_p.h"

#include <stdlib.h>

#ifdef __GLIBC__
#  include <sys/syscall.h>
#  include <pthread.h>
#  include <unistd.h>
#endif

#ifdef Q_OS_DARWIN
#include <mach/mach_time.h>
#endif

BOBUI_BEGIN_NAMESPACE

void bobui_ignore_sigpipe() noexcept // noexcept: sigaction(2) is not a Posix Cancellation Point
{
    // Set to ignore SIGPIPE once only.
    Q_CONSTINIT static QBasicAtomicInt atom = Q_BASIC_ATOMIC_INITIALIZER(0);
    if (!atom.loadRelaxed()) {
        // More than one thread could turn off SIGPIPE at the same time
        // But that's acceptable because they all would be doing the same
        // action
        struct sigaction noaction = {};
        noaction.sa_handler = SIG_IGN;
        ::sigaction(SIGPIPE, &noaction, nullptr);
        atom.storeRelaxed(1);
    }
}

QByteArray bobui_readlink(const char *path)
{
#ifndef PATH_MAX
    // suitably large value that won't consume too much memory
#  define PATH_MAX  1024*1024
#endif

    QByteArray buf(256, BobUI::Uninitialized);

    ssize_t len = ::readlink(path, buf.data(), buf.size());
    while (len == buf.size()) {
        // readlink(2) will fill our buffer and not necessarily terminate with NUL;
        if (buf.size() >= PATH_MAX) {
            errno = ENAMETOOLONG;
            return QByteArray();
        }

        // double the size and try again
        buf.resize(buf.size() * 2);
        len = ::readlink(path, buf.data(), buf.size());
    }

    if (len == -1)
        return QByteArray();

    buf.resize(len);
    return buf;
}

#if defined(Q_PROCESSOR_X86_32) && defined(__GLIBC__)
#  if !__GLIBC_PREREQ(2, 22)
// glibc prior to release 2.22 had a bug that suppresses the third argument to
// open() / open64() / openat(), causing file creation with O_TMPFILE to have
// the wrong permissions. So we bypass the glibc implementation and go straight
// for the syscall. See
// https://sourceware.org/git/?p=glibc.git;a=commit;h=65f6f938cd562a614a68e15d0581a34b177ec29d
int bobui_open64(const char *pathname, int flags, mode_t mode)
{
    return syscall(SYS_open, pathname, flags | O_LARGEFILE, mode);
}
#  endif
#endif

#ifndef BOBUI_BOOTSTRAPPED

#if BOBUI_CONFIG(poll_pollts)
#  define ppoll pollts
#endif

[[maybe_unused]]
static inline int timespecToMillisecs(const struct timespec *ts)
{
    using namespace std::chrono;
    if (!ts)
        return -1;
    auto ms = ceil<milliseconds>(timespecToChrono<nanoseconds>(*ts));
    return int(ms.count());
}

// defined in qpoll.cpp
int bobui_poll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts);

static inline int bobui_ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts)
{
#if BOBUI_CONFIG(poll_ppoll) || BOBUI_CONFIG(poll_pollts)
    return ::ppoll(fds, nfds, timeout_ts, nullptr);
#elif BOBUI_CONFIG(poll_poll)
    return ::poll(fds, nfds, timespecToMillisecs(timeout_ts));
#elif BOBUI_CONFIG(poll_select)
    return bobui_poll(fds, nfds, timeout_ts);
#else
    // configure.json reports an error when everything is not available
#endif
}


/*!
    \internal

    Behaves as close to POSIX poll(2) as practical but may be implemented
    using select(2) where necessary. In that case, returns -1 and sets errno
    to EINVAL if passed any descriptor greater than or equal to FD_SETSIZE.
*/
int bobui_safe_poll(struct pollfd *fds, nfds_t nfds, QDeadlineTimer deadline)
{
    if (deadline.isForever()) {
        // no timeout -> block forever
        int ret;
        BOBUI_EINTR_LOOP(ret, bobui_ppoll(fds, nfds, nullptr));
        return ret;
    }

    using namespace std::chrono;
    nanoseconds remaining = deadline.remainingTimeAsDuration();
    // loop and recalculate the timeout as needed
    do {
        timespec ts = durationToTimespec(remaining);
        const int ret = bobui_ppoll(fds, nfds, &ts);
        if (ret != -1 || errno != EINTR)
            return ret;
        remaining = deadline.remainingTimeAsDuration();
    } while (remaining > 0ns);

    return 0;
}

#endif // BOBUI_BOOTSTRAPPED

BOBUI_END_NAMESPACE
