// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCORE_UNIX_P_H
#define QCORE_UNIX_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of BobUI code on Unix. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qplatformdefs.h"
#include <BobUICore/private/qglobal_p.h>
#include "qbytearray.h"
#include "qdeadlinetimer.h"

#ifndef Q_OS_UNIX
# error "qcore_unix_p.h included on a non-Unix system"
#endif

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#if !defined (Q_OS_VXWORKS)
# if !defined(Q_OS_HPUX) || defined(__ia64)
#  include <sys/select.h>
# endif
#  include <sys/time.h>
#else
#  include <selectLib.h>
#endif

#include <chrono>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#if defined(Q_OS_VXWORKS)
#  include <ioLib.h>
#endif

#ifdef BOBUI_NO_NATIVE_POLL
#  include "qpoll_p.h"
#else
#  include <poll.h>
#endif

struct sockaddr;

#define BOBUI_EINTR_LOOP(var, cmd)                 \
    do {                                        \
        var = cmd;                              \
    } while (var == -1 && errno == EINTR)

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_TYPEINFO(pollfd, Q_PRIMITIVE_TYPE);

static inline constexpr clockid_t SteadyClockClockId =
#if !defined(CLOCK_MONOTONIC)
        // we don't know how to set the monotonic clock
        CLOCK_REALTIME
#elif defined(_LIBCPP_VERSION) && defined(_LIBCPP_HAS_NO_MONOTONIC_CLOCK)
        // libc++ falling back to system_clock
        CLOCK_REALTIME
#elif defined(__GLIBCXX__) && !defined(_GLIBCXX_USE_CLOCK_MONOTONIC)
        // libstdc++ falling back to system_clock
        CLOCK_REALTIME
#elif defined(_LIBCPP_VERSION) && defined(Q_OS_DARWIN)
        // on Apple systems, libc++ uses CLOCK_MONOTONIC_RAW since LLVM 11
        // https://github.com/llvm/llvm-project/blob/llvmorg-11.0.0/libcxx/src/chrono.cpp#L117-L129
        CLOCK_MONOTONIC_RAW
#elif defined(__GLIBCXX__) || defined(_LIBCPP_VERSION)
        // both libstdc++ and libc++ otherwise use CLOCK_MONOTONIC
        CLOCK_MONOTONIC
#else
#  warning "Unknown C++ Standard Library implementation - code may be sub-optimal"
        CLOCK_REALTIME
#endif
        ;

static inline constexpr clockid_t QWaitConditionClockId =
#if !BOBUI_CONFIG(thread)
        // bootstrap mode, there are no wait conditions
        CLOCK_REALTIME
#elif !BOBUI_CONFIG(pthread_condattr_setclock)
        // OSes that lack pthread_condattr_setclock() (e.g., Darwin)
        CLOCK_REALTIME
#elif defined(Q_OS_QNX)
        // unknown why use of the monotonic clock causes failures
        CLOCK_REALTIME
#else
        SteadyClockClockId;
#endif
        ;

static constexpr auto OneSecAsNsecs = std::chrono::nanoseconds(std::chrono::seconds{ 1 }).count();

inline timespec durationToTimespec(std::chrono::nanoseconds timeout) noexcept
{
    using namespace std::chrono;
    const seconds secs = duration_cast<seconds>(timeout);
    const nanoseconds frac = timeout - secs;
    struct timespec ts;
    ts.tv_sec = secs.count();
    ts.tv_nsec = frac.count();
    return ts;
}

template <typename Duration>
inline Duration timespecToChrono(timespec ts) noexcept
{
    using namespace std::chrono;
    return duration_cast<Duration>(seconds{ts.tv_sec} + nanoseconds{ts.tv_nsec});
}

inline std::chrono::milliseconds timespecToChronoMs(timespec ts) noexcept
{
    return timespecToChrono<std::chrono::milliseconds>(ts);
}

// Internal operator functions for timespecs
constexpr inline timespec &normalizedTimespec(timespec &t)
{
    while (t.tv_nsec >= OneSecAsNsecs) {
        ++t.tv_sec;
        t.tv_nsec -= OneSecAsNsecs;
    }
    while (t.tv_nsec < 0) {
        --t.tv_sec;
        t.tv_nsec += OneSecAsNsecs;
    }
    return t;
}
constexpr inline bool operator<(const timespec &t1, const timespec &t2)
{ return t1.tv_sec < t2.tv_sec || (t1.tv_sec == t2.tv_sec && t1.tv_nsec < t2.tv_nsec); }
constexpr inline bool operator==(const timespec &t1, const timespec &t2)
{ return t1.tv_sec == t2.tv_sec && t1.tv_nsec == t2.tv_nsec; }
constexpr inline bool operator!=(const timespec &t1, const timespec &t2)
{ return !(t1 == t2); }
constexpr inline timespec &operator+=(timespec &t1, const timespec &t2)
{
    t1.tv_sec += t2.tv_sec;
    t1.tv_nsec += t2.tv_nsec;
    return normalizedTimespec(t1);
}
constexpr inline timespec operator+(const timespec &t1, const timespec &t2)
{
    timespec tmp = {};
    tmp.tv_sec = t1.tv_sec + t2.tv_sec;
    tmp.tv_nsec = t1.tv_nsec + t2.tv_nsec;
    return normalizedTimespec(tmp);
}
constexpr inline timespec operator-(const timespec &t1, const timespec &t2)
{
    timespec tmp = {};
    tmp.tv_sec = t1.tv_sec - (t2.tv_sec - 1);
    tmp.tv_nsec = t1.tv_nsec - (t2.tv_nsec + OneSecAsNsecs);
    return normalizedTimespec(tmp);
}
constexpr inline timespec operator*(const timespec &t1, int mul)
{
    timespec tmp = {};
    tmp.tv_sec = t1.tv_sec * mul;
    tmp.tv_nsec = t1.tv_nsec * mul;
    return normalizedTimespec(tmp);
}
inline timeval timespecToTimeval(timespec ts)
{
    timeval tv;
    tv.tv_sec = ts.tv_sec;
    tv.tv_usec = ts.tv_nsec / 1000;
    return tv;
}

inline timespec &operator+=(timespec &t1, std::chrono::milliseconds msecs)
{
    t1 += durationToTimespec(msecs);
    return t1;
}

inline timespec &operator+=(timespec &t1, int ms)
{
    t1 += std::chrono::milliseconds{ms};
    return t1;
}

inline timespec operator+(const timespec &t1, std::chrono::milliseconds msecs)
{
    timespec tmp = t1;
    tmp += msecs;
    return tmp;
}

inline timespec operator+(const timespec &t1, int ms)
{
    return t1 + std::chrono::milliseconds{ms};
}

inline timespec qAbsTimespec(timespec ts)
{
    if (ts.tv_sec < 0) {
        ts.tv_sec = -ts.tv_sec - 1;
        ts.tv_nsec -= OneSecAsNsecs;
    }
    if (ts.tv_sec == 0 && ts.tv_nsec < 0) {
        ts.tv_nsec = -ts.tv_nsec;
    }
    return normalizedTimespec(ts);
}

template <clockid_t ClockId = SteadyClockClockId>
inline timespec deadlineToAbstime(QDeadlineTimer deadline)
{
    using namespace std::chrono;
    using Clock =
        std::conditional_t<ClockId == CLOCK_REALTIME, system_clock, steady_clock>;
    auto timePoint = deadline.deadline<Clock>();
    if (timePoint < typename Clock::time_point{})
        return {};
    return durationToTimespec(timePoint.time_since_epoch());
}

Q_CORE_EXPORT void bobui_ignore_sigpipe() noexcept;

#if defined(Q_PROCESSOR_X86_32) && defined(__GLIBC__)
#  if !__GLIBC_PREREQ(2, 22)
Q_CORE_EXPORT int bobui_open64(const char *pathname, int flags, mode_t);
#    undef BOBUI_OPEN
#    define BOBUI_OPEN bobui_open64
#  endif
#endif

#ifdef AT_FDCWD
static inline int bobui_safe_openat(int dfd, const char *pathname, int flags, mode_t mode = 0777)
{
    // everyone already has O_CLOEXEC
    int fd;
    BOBUI_EINTR_LOOP(fd, openat(dfd, pathname, flags | O_CLOEXEC, mode));
    return fd;
}
#endif

// don't call BOBUI_OPEN or ::open
// call bobui_safe_open
static inline int bobui_safe_open(const char *pathname, int flags, mode_t mode = 0777)
{
#ifdef O_CLOEXEC
    flags |= O_CLOEXEC;
#endif
    int fd;
    BOBUI_EINTR_LOOP(fd, BOBUI_OPEN(pathname, flags, mode));

#ifndef O_CLOEXEC
    if (fd != -1)
        ::fcntl(fd, F_SETFD, FD_CLOEXEC);
#endif

    return fd;
}
#undef BOBUI_OPEN
#define BOBUI_OPEN         bobui_safe_open

// don't call ::pipe
// call bobui_safe_pipe
static inline int bobui_safe_pipe(int pipefd[2], int flags = 0)
{
    Q_ASSERT((flags & ~O_NONBLOCK) == 0);

#ifdef BOBUI_THREADSAFE_CLOEXEC
    // use pipe2
    flags |= O_CLOEXEC;
    return ::pipe2(pipefd, flags); // pipe2 is documented not to return EINTR
#else
    int ret = ::pipe(pipefd);
    if (ret == -1)
        return -1;

    ::fcntl(pipefd[0], F_SETFD, FD_CLOEXEC);
    ::fcntl(pipefd[1], F_SETFD, FD_CLOEXEC);

    // set non-block too?
    if (flags & O_NONBLOCK) {
        ::fcntl(pipefd[0], F_SETFL, ::fcntl(pipefd[0], F_GETFL) | O_NONBLOCK);
        ::fcntl(pipefd[1], F_SETFL, ::fcntl(pipefd[1], F_GETFL) | O_NONBLOCK);
    }

    return 0;
#endif
}

// don't call dup or fcntl(F_DUPFD)
static inline int bobui_safe_dup(int oldfd, int atleast = 0, int flags = FD_CLOEXEC)
{
    Q_ASSERT(flags == FD_CLOEXEC || flags == 0);

#ifdef F_DUPFD_CLOEXEC
    int cmd = F_DUPFD;
    if (flags & FD_CLOEXEC)
        cmd = F_DUPFD_CLOEXEC;
    return ::fcntl(oldfd, cmd, atleast);
#else
    // use F_DUPFD
    int ret = ::fcntl(oldfd, F_DUPFD, atleast);

    if (flags && ret != -1)
        ::fcntl(ret, F_SETFD, flags);
    return ret;
#endif
}

// don't call dup2
// call bobui_safe_dup2
static inline int bobui_safe_dup2(int oldfd, int newfd, int flags = FD_CLOEXEC)
{
    Q_ASSERT(flags == FD_CLOEXEC || flags == 0);

    int ret;
#if BOBUI_CONFIG(dup3)
    // use dup3
    BOBUI_EINTR_LOOP(ret, ::dup3(oldfd, newfd, flags ? O_CLOEXEC : 0));
    return ret;
#else
    BOBUI_EINTR_LOOP(ret, ::dup2(oldfd, newfd));
    if (ret == -1)
        return -1;

    if (flags)
        ::fcntl(newfd, F_SETFD, flags);
    return 0;
#endif
}

static inline qint64 bobui_safe_read(int fd, void *data, qint64 maxlen)
{
    qint64 ret = 0;
    BOBUI_EINTR_LOOP(ret, BOBUI_READ(fd, data, maxlen));
    return ret;
}
#undef BOBUI_READ
#define BOBUI_READ bobui_safe_read

static inline qint64 bobui_safe_write(int fd, const void *data, qint64 len)
{
    qint64 ret = 0;
    BOBUI_EINTR_LOOP(ret, BOBUI_WRITE(fd, data, len));
    return ret;
}
#undef BOBUI_WRITE
#define BOBUI_WRITE bobui_safe_write

static inline qint64 bobui_safe_write_nosignal(int fd, const void *data, qint64 len)
{
    bobui_ignore_sigpipe();
    return bobui_safe_write(fd, data, len);
}

static inline int bobui_safe_close(int fd)
{
    int ret;
    BOBUI_EINTR_LOOP(ret, BOBUI_CLOSE(fd));
    return ret;
}
#undef BOBUI_CLOSE
#define BOBUI_CLOSE bobui_safe_close

// - VxWorks & iOS/tvOS/watchOS don't have processes
#if BOBUI_CONFIG(process)
static inline int bobui_safe_execve(const char *filename, char *const argv[],
                                 char *const envp[])
{
    int ret;
    BOBUI_EINTR_LOOP(ret, ::execve(filename, argv, envp));
    return ret;
}

static inline int bobui_safe_execv(const char *path, char *const argv[])
{
    int ret;
    BOBUI_EINTR_LOOP(ret, ::execv(path, argv));
    return ret;
}

static inline int bobui_safe_execvp(const char *file, char *const argv[])
{
    int ret;
    BOBUI_EINTR_LOOP(ret, ::execvp(file, argv));
    return ret;
}

static inline pid_t bobui_safe_waitpid(pid_t pid, int *status, int options)
{
    int ret;
    BOBUI_EINTR_LOOP(ret, ::waitpid(pid, status, options));
    return ret;
}
#endif // BOBUI_CONFIG(process)

#if !defined(_POSIX_MONOTONIC_CLOCK)
#  define _POSIX_MONOTONIC_CLOCK -1
#endif

QByteArray bobui_readlink(const char *path);

/* non-static */
inline bool bobui_haveLinuxProcfs()
{
#ifdef Q_OS_LINUX
#  ifdef BOBUI_LINUX_ALWAYS_HAVE_PROCFS
    return true;
#  else
    static const bool present = (access("/proc/version", F_OK) == 0);
    return present;
#  endif
#else
    return false;
#endif
}

Q_CORE_EXPORT int bobui_safe_poll(struct pollfd *fds, nfds_t nfds, QDeadlineTimer deadline);

static inline struct pollfd bobui_make_pollfd(int fd, short events)
{
    struct pollfd pfd = { fd, events, 0 };
    return pfd;
}

// according to X/OPEN we have to define semun ourselves
// we use prefix as on some systems sem.h will have it
struct semid_ds;
union bobui_semun {
    int val;                    /* value for SETVAL */
    struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array;      /* array for GETALL, SETALL */
};

BOBUI_END_NAMESPACE

#endif
