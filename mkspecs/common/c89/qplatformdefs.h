// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef Q_C89_QPLATFORMDEFS_H
#define Q_C89_QPLATFORMDEFS_H

// #include <stdio.h>

#define BOBUI_FPOS_T               fpos_t
#define BOBUI_OFF_T                long

#define BOBUI_FOPEN                ::fopen
#define BOBUI_FSEEK                ::fseek
#define BOBUI_FTELL                ::ftell
#define BOBUI_FGETPOS              ::fgetpos
#define BOBUI_FSETPOS              ::fsetpos

#endif // include guard
