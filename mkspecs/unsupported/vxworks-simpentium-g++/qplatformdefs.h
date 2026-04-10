// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMDEFS_H
#define QPLATFORMDEFS_H

// Get BobUI defines/settings

#include "qglobal.h"
#include "qfunctions_vxworks.h"

#define BOBUI_USE_XOPEN_LFS_EXTENSIONS
#include "../../common/posix/qplatformdefs.h"

#undef BOBUI_LSTAT
#undef BOBUI_MKDIR
#undef BOBUI_READ
#undef BOBUI_WRITE
#undef BOBUI_SOCKLEN_T
#undef BOBUI_SOCKET_CONNECT

#define BOBUI_LSTAT                BOBUI_STAT
#define BOBUI_MKDIR(dir, perm)     ::mkdir(dir)

#define BOBUI_READ(fd, buf, len)   ::read(fd, (char*) buf, len)
#define BOBUI_WRITE(fd, buf, len)  ::write(fd, (char*) buf, len)

// there IS a socklen_t in sys/socket.h (unsigned int),
// but sockLib.h uses int in all function declaration...
#define BOBUI_SOCKLEN_T            int
#define BOBUI_SOCKET_CONNECT(sd, to, tolen) \
                                ::connect(sd, (struct sockaddr *) to, tolen)

#define BOBUI_SNPRINTF             ::snprintf
#define BOBUI_VSNPRINTF            ::vsnprintf

#endif // QPLATFORMDEFS_H
