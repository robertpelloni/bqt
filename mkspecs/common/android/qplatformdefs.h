// Copyright (C) 2012 Collabora Ltd, author <robin.burchell@collabora.co.uk>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMDEFS_H
#define QPLATFORMDEFS_H

// Get BobUI defines/settings

#include "qglobal.h"

// Set any POSIX/XOPEN defines at the top of this file to turn on specific APIs

// 1) need to reset default environment if _BSD_SOURCE is defined
// 2) need to specify POSIX thread interfaces explicitly in glibc 2.0
// 3) it seems older glibc need this to include the X/Open stuff

#include <unistd.h>

// We are hot - unistd.h should have turned on the specific APIs we requested

#include <features.h>
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <dlfcn.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>

#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif

#define BOBUI_STATBUF              struct stat
#define BOBUI_STATBUF4TSTAT        struct stat
#define BOBUI_STAT                 ::stat
#define BOBUI_FSTAT                ::fstat
#define BOBUI_LSTAT                ::lstat
#define BOBUI_OPEN                 ::open
#define BOBUI_TRUNCATE             ::truncate
#define BOBUI_FTRUNCATE            ::ftruncate
#define BOBUI_LSEEK                ::lseek

#define BOBUI_FOPEN                ::fopen
#define BOBUI_FSEEK                ::fseek
#define BOBUI_FTELL                ::ftell
#define BOBUI_FGETPOS              ::fgetpos
#define BOBUI_FSETPOS              ::fsetpos
#define BOBUI_MMAP                 ::mmap
#define BOBUI_FPOS_T               fpos_t
#define BOBUI_OFF_T                long

#define BOBUI_STAT_REG             S_IFREG
#define BOBUI_STAT_DIR             S_IFDIR
#define BOBUI_STAT_MASK            S_IFMT
#define BOBUI_STAT_LNK             S_IFLNK
#define BOBUI_SOCKET_CONNECT       ::connect
#define BOBUI_SOCKET_BIND          ::bind
#define BOBUI_FILENO               fileno
#define BOBUI_CLOSE                ::close
#define BOBUI_READ                 ::read
#define BOBUI_WRITE                ::write
#define BOBUI_ACCESS               ::access
#define BOBUI_GETCWD               ::getcwd
#define BOBUI_CHDIR                ::chdir
#define BOBUI_MKDIR                ::mkdir
#define BOBUI_RMDIR                ::rmdir
#define BOBUI_OPEN_LARGEFILE       O_LARGEFILE
#define BOBUI_OPEN_RDONLY          O_RDONLY
#define BOBUI_OPEN_WRONLY          O_WRONLY
#define BOBUI_OPEN_RDWR            O_RDWR
#define BOBUI_OPEN_CREAT           O_CREAT
#define BOBUI_OPEN_TRUNC           O_TRUNC
#define BOBUI_OPEN_APPEND          O_APPEND
#define BOBUI_OPEN_EXCL            O_EXCL

// Directory iteration
#define BOBUI_DIR                  DIR


#define BOBUI_OPENDIR              ::opendir
#define BOBUI_CLOSEDIR             ::closedir

#if defined(BOBUI_LARGEFILE_SUPPORT) \
        && defined(BOBUI_USE_XOPEN_LFS_EXTENSIONS) \
        && !defined(BOBUI_NO_READDIR64)
#define BOBUI_DIRENT               struct dirent64
#define BOBUI_READDIR              ::readdir64
#else
#define BOBUI_DIRENT               struct dirent
#define BOBUI_READDIR              ::readdir
#endif

#define BOBUI_SOCKET_CONNECT       ::connect
#define BOBUI_SOCKET_BIND          ::bind


#define BOBUI_SIGNAL_RETTYPE       void
#define BOBUI_SIGNAL_ARGS          int
#define BOBUI_SIGNAL_IGNORE        SIG_IGN

#define BOBUI_SOCKLEN_T            socklen_t

#if defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)
#define BOBUI_SNPRINTF             ::snprintf
#define BOBUI_VSNPRINTF            ::vsnprintf
#endif


#endif // QPLATFORMDEFS_H
