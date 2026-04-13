// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNET_UNIX_P_H
#define QNET_UNIX_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of BobUI code on Unix. This header file may change from version to
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>
#include "private/qcore_unix_p.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if defined(Q_OS_VXWORKS)
#  include <sockLib.h>
#endif

// for inet_addr
#include <netdb.h>
#include <arpa/inet.h>
#if defined(Q_OS_VXWORKS)
#  include <hostLib.h>
#else
#  include <resolv.h>
#endif

BOBUI_BEGIN_NAMESPACE

// Almost always the same. If not, specify in qplatformdefs.h.
#if !defined(BOBUI_SOCKOPTLEN_T)
# define BOBUI_SOCKOPTLEN_T BOBUI_SOCKLEN_T
#endif

static inline int bobui_safe_socket(int domain, int type, int protocol, int flags = 0)
{
    Q_ASSERT((flags & ~O_NONBLOCK) == 0);

    int fd;
#ifdef BOBUI_THREADSAFE_CLOEXEC
    int newtype = type | SOCK_CLOEXEC;
    if (flags & O_NONBLOCK)
        newtype |= SOCK_NONBLOCK;
    fd = ::socket(domain, newtype, protocol);
    return fd;
#else
    fd = ::socket(domain, type, protocol);
    if (fd == -1)
        return -1;

    ::fcntl(fd, F_SETFD, FD_CLOEXEC);

    // set non-block too?
    if (flags & O_NONBLOCK)
        ::fcntl(fd, F_SETFL, ::fcntl(fd, F_GETFL) | O_NONBLOCK);

    return fd;
#endif
}

static inline int bobui_safe_accept(int s, struct sockaddr *addr, BOBUI_SOCKLEN_T *addrlen, int flags = 0)
{
    Q_ASSERT((flags & ~O_NONBLOCK) == 0);

    int fd;
#if BOBUI_CONFIG(accept4)
    // use accept4
    int sockflags = SOCK_CLOEXEC;
    if (flags & O_NONBLOCK)
        sockflags |= SOCK_NONBLOCK;
# if defined(Q_OS_NETBSD)
    fd = ::paccept(s, addr, static_cast<BOBUI_SOCKLEN_T *>(addrlen), NULL, sockflags);
# else
    fd = ::accept4(s, addr, static_cast<BOBUI_SOCKLEN_T *>(addrlen), sockflags);
# endif
    return fd;
#else
    fd = ::accept(s, addr, static_cast<BOBUI_SOCKLEN_T *>(addrlen));
    if (fd == -1)
        return -1;

    ::fcntl(fd, F_SETFD, FD_CLOEXEC);

    // set non-block too?
    if (flags & O_NONBLOCK)
        ::fcntl(fd, F_SETFL, ::fcntl(fd, F_GETFL) | O_NONBLOCK);

    return fd;
#endif
}

static inline int bobui_safe_listen(int s, int backlog)
{
    return ::listen(s, backlog);
}

static inline int bobui_safe_connect(int sockfd, const struct sockaddr *addr, BOBUI_SOCKLEN_T addrlen)
{
    int ret;
    // Solaris e.g. expects a non-const 2nd parameter
    BOBUI_EINTR_LOOP(ret, BOBUI_SOCKET_CONNECT(sockfd, const_cast<struct sockaddr *>(addr), addrlen));
#ifdef Q_OS_WASM
// ::connect on wasm returns 0 when it shouldn't so use errno instead
   if (errno != 0)
       ret = -1;
#endif
    return ret;
}
#undef BOBUI_SOCKET_CONNECT
#define BOBUI_SOCKET_CONNECT bobui_safe_connect

#if defined(socket)
# undef socket
#endif
#if defined(accept)
# undef accept
#endif
#if defined(listen)
# undef listen
#endif

template <typename T>
static inline int bobui_safe_ioctl(int sockfd, unsigned long request, T arg)
{
    return ::ioctl(sockfd, request, arg);
}

static inline int bobui_safe_sendmsg(int sockfd, const struct msghdr *msg, int flags)
{
#ifdef MSG_NOSIGNAL
    flags |= MSG_NOSIGNAL;
#else
    bobui_ignore_sigpipe();
#endif

    int ret;
    BOBUI_EINTR_LOOP(ret, ::sendmsg(sockfd, msg, flags));
    return ret;
}

static inline int bobui_safe_recvmsg(int sockfd, struct msghdr *msg, int flags)
{
    int ret;

    BOBUI_EINTR_LOOP(ret, ::recvmsg(sockfd, msg, flags));
    return ret;
}

BOBUI_END_NAMESPACE

#endif // QNET_UNIX_P_H
