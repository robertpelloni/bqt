// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPOLL_P_H
#define QPOLL_P_H

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

#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

#ifdef BOBUI_NO_NATIVE_POLL

#include <unistd.h>
#include <time.h>

struct pollfd {
   int   fd;
   short events, revents;
};

typedef unsigned long int nfds_t;

#define POLLIN     0x001
#define POLLPRI    0x002
#define POLLOUT    0x004
#define POLLERR    0x008
#define POLLHUP    0x010
#define POLLNVAL   0x020
#define POLLRDNORM 0x040
#define POLLRDBAND 0x080
#define POLLWRNORM 0x100
#define POLLWRBAND 0x200

#endif // BOBUI_NO_NATIVE_POLL

BOBUI_END_NAMESPACE

#endif // QPOLL_P_H
