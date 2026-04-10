// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMDEFS_H
#define QPLATFORMDEFS_H

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

// Get BobUI defines/settings

#include <BobUICore/qglobal.h>

#define _POSIX_
#include <limits.h>
#undef _POSIX_

#include <tchar.h>
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>

#ifdef BOBUI_LARGEFILE_SUPPORT
#define BOBUI_STATBUF              struct _stati64         // non-ANSI defs
#define BOBUI_STATBUF4TSTAT        struct _stati64         // non-ANSI defs
#define BOBUI_STAT                 ::_stati64
#define BOBUI_FSTAT                ::_fstati64
#else
#define BOBUI_STATBUF              struct _stat            // non-ANSI defs
#define BOBUI_STATBUF4TSTAT        struct _stat            // non-ANSI defs
#define BOBUI_STAT                 ::_stat
#define BOBUI_FSTAT                ::_fstat
#endif
#define BOBUI_STAT_REG             _S_IFREG
#define BOBUI_STAT_DIR             _S_IFDIR
#define BOBUI_STAT_MASK            _S_IFMT
#if defined(_S_IFLNK)
#  define BOBUI_STAT_LNK           _S_IFLNK
#else
#  define BOBUI_STAT_LNK           0120000
#endif
#define BOBUI_FILENO               _fileno
#define BOBUI_OPEN                 ::_open
#define BOBUI_CLOSE                ::_close
#ifdef BOBUI_LARGEFILE_SUPPORT
#define BOBUI_LSEEK                ::_lseeki64
#define BOBUI_TSTAT                ::_tstati64
#else
#define BOBUI_LSEEK                ::_lseek
#define BOBUI_TSTAT                ::_tstat
#endif
#define BOBUI_READ                 ::_read
#define BOBUI_WRITE                ::_write
#define BOBUI_ACCESS               ::_access
#define BOBUI_GETCWD               ::_getcwd
#define BOBUI_CHDIR                ::_chdir
#define BOBUI_MKDIR                ::_mkdir
#define BOBUI_RMDIR                ::_rmdir
#define BOBUI_OPEN_LARGEFILE       0
#define BOBUI_OPEN_RDONLY          _O_RDONLY
#define BOBUI_OPEN_WRONLY          _O_WRONLY
#define BOBUI_OPEN_RDWR            _O_RDWR
#define BOBUI_OPEN_CREAT           _O_CREAT
#define BOBUI_OPEN_TRUNC           _O_TRUNC
#define BOBUI_OPEN_APPEND          _O_APPEND
#if defined(O_TEXT)
# define BOBUI_OPEN_TEXT           _O_TEXT
# define BOBUI_OPEN_BINARY         _O_BINARY
#endif

#include "../common/c89/qplatformdefs.h"

#ifdef BOBUI_LARGEFILE_SUPPORT
#undef BOBUI_FSEEK
#undef BOBUI_FTELL
#undef BOBUI_OFF_T

#define BOBUI_FSEEK                ::_fseeki64
#define BOBUI_FTELL                ::_ftelli64
#define BOBUI_OFF_T                __int64
#endif

#define BOBUI_SIGNAL_ARGS          int

#define BOBUI_VSNPRINTF(buffer, count, format, arg) \
    vsnprintf_s(buffer, count, count-1, format, arg)

#define BOBUI_SNPRINTF             ::_snprintf

# define F_OK   0
# define X_OK   1
# define W_OK   2
# define R_OK   4

typedef int mode_t;

#endif // QPLATFORMDEFS_H
