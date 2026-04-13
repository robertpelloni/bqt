// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef Q_AIX_QPLATFORMDEFS_H
#define Q_AIX_QPLATFORMDEFS_H

// Get BobUI defines/settings

#include "qglobal.h"

// Set any POSIX/XOPEN defines at the top of this file to turn on specific APIs

#include <unistd.h>


// We are hot - unistd.h should have turned on the specific APIs we requested


// uncomment if you have problems with <sys/proc.h> because your gcc
// hasn't been built on exactly the same OS version your are using now.
// typedef int crid_t;
// typedef unsigned int class_id_t;
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <dlfcn.h>
#include <strings.h> // AIX X11 headers define FD_ZERO using bzero()

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>

// Only AIX 4.3 and better support 64-bit

#define BOBUI_USE_XOPEN_LFS_EXTENSIONS
#include "../posix/qplatformdefs.h"

#undef BOBUI_SOCKLEN_T

#ifdef _AIX43
// AIX 4.3 and better
#define BOBUI_SOCKLEN_T            socklen_t
#elif _AIX42
// AIX 4.2
#define BOBUI_SOCKLEN_T            size_t
#else
// AIX 4.1
#define BOBUI_SOCKLEN_T            size_t
// override
#define BOBUI_SOCKOPTLEN_T         int
#endif

#ifdef BOBUI_LARGEFILE_SUPPORT
#undef BOBUI_DIR
#undef BOBUI_OPENDIR
#undef BOBUI_CLOSEDIR

#define BOBUI_DIR                  DIR64
#define BOBUI_OPENDIR              ::opendir64
#define BOBUI_CLOSEDIR             ::closedir64
#endif

#if defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE-0 >= 500)
// AIX 4.3 and better
#define BOBUI_SNPRINTF             ::snprintf
#define BOBUI_VSNPRINTF            ::vsnprintf
#endif

#endif // include guard
