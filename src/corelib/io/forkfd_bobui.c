// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#include <BobUICore/qglobal.h>

#define FORKFD_NO_SPAWNFD
#if defined(BOBUI_NO_DEBUG) && !defined(NDEBUG)
#  define NDEBUG
#endif

#include <forkfd.h>
#include "../../3rdparty/forkfd/forkfd.c"
