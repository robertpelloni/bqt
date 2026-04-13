// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef Q_POSIX_QPLATFORMDEFS_H
#define Q_POSIX_QPLATFORMDEFS_H

#include <signal.h>

#include <sys/types.h>
#ifndef BOBUI_NO_SOCKET_H
#  include <sys/socket.h>
#endif
#include <sys/stat.h>

#if defined(BOBUI_USE_XOPEN_LFS_EXTENSIONS) && defined(BOBUI_LARGEFILE_SUPPORT)

#define BOBUI_STATBUF              struct stat64
#define BOBUI_FPOS_T               fpos64_t
#define BOBUI_OFF_T                off64_t

#define BOBUI_STAT                 ::stat64
#define BOBUI_LSTAT                ::lstat64
#define BOBUI_TRUNCATE             ::truncate64

// File I/O
#define BOBUI_OPEN                 ::open64
#define BOBUI_LSEEK                ::lseek64
#define BOBUI_FSTAT                ::fstat64
#define BOBUI_FTRUNCATE            ::ftruncate64

// Standard C89
#define BOBUI_FOPEN                ::fopen64
#define BOBUI_FSEEK                ::fseeko64
#define BOBUI_FTELL                ::ftello64
#define BOBUI_FGETPOS              ::fgetpos64
#define BOBUI_FSETPOS              ::fsetpos64

#define BOBUI_MMAP                 ::mmap64

#else // !defined(BOBUI_USE_XOPEN_LFS_EXTENSIONS) || !defined(BOBUI_LARGEFILE_SUPPORT)

#include "../c89/qplatformdefs.h"

#define BOBUI_STATBUF              struct stat

#define BOBUI_STAT                 ::stat
#define BOBUI_LSTAT                ::lstat
#define BOBUI_TRUNCATE             ::truncate

// File I/O
#define BOBUI_OPEN                 ::open
#define BOBUI_LSEEK                ::lseek
#define BOBUI_FSTAT                ::fstat
#define BOBUI_FTRUNCATE            ::ftruncate

// Posix extensions to C89
#if !defined(BOBUI_USE_XOPEN_LFS_EXTENSIONS) && !defined(BOBUI_NO_USE_FSEEKO)
#undef BOBUI_OFF_T
#undef BOBUI_FSEEK
#undef BOBUI_FTELL

#define BOBUI_OFF_T                off_t

#define BOBUI_FSEEK                ::fseeko
#define BOBUI_FTELL                ::ftello
#endif

#define BOBUI_MMAP                 ::mmap

#endif // !defined (BOBUI_USE_XOPEN_LFS_EXTENSIONS) || !defined(BOBUI_LARGEFILE_SUPPORT)

#define BOBUI_STAT_MASK            S_IFMT
#define BOBUI_STAT_REG             S_IFREG
#define BOBUI_STAT_DIR             S_IFDIR
#define BOBUI_STAT_LNK             S_IFLNK

#define BOBUI_ACCESS               ::access
#define BOBUI_GETCWD               ::getcwd
#define BOBUI_CHDIR                ::chdir
#define BOBUI_MKDIR                ::mkdir
#define BOBUI_RMDIR                ::rmdir

// File I/O
#define BOBUI_CLOSE                ::close
#define BOBUI_READ                 ::read
#define BOBUI_WRITE                ::write

#define BOBUI_OPEN_LARGEFILE       O_LARGEFILE
#define BOBUI_OPEN_RDONLY          O_RDONLY
#define BOBUI_OPEN_WRONLY          O_WRONLY
#define BOBUI_OPEN_RDWR            O_RDWR
#define BOBUI_OPEN_CREAT           O_CREAT
#define BOBUI_OPEN_TRUNC           O_TRUNC
#define BOBUI_OPEN_APPEND          O_APPEND
#define BOBUI_OPEN_EXCL            O_EXCL

// Posix extensions to C89
#define BOBUI_FILENO               fileno

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

#define BOBUI_SOCKLEN_T            socklen_t

#define BOBUI_SOCKET_CONNECT       ::connect
#define BOBUI_SOCKET_BIND          ::bind

#define BOBUI_SIGNAL_RETTYPE       void
#define BOBUI_SIGNAL_ARGS          int
#define BOBUI_SIGNAL_IGNORE        SIG_IGN

#endif // include guard
