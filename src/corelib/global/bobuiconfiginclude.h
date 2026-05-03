// Copyright (C) 2022 Intel Corporation
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUICONFIGINCLUDE_H
#define BOBUICONFIGINCLUDE_H

#if 0
#  pragma bobui_sync_stop_processing
#endif

#ifdef __cplusplus
# if __has_include(<version>) /* remove this check once Integrity, QNX have caught up */
#  include <version>
# endif
#endif

#include <BobUICore/qconfig.h>

#ifdef BOBUI_BOOTSTRAPPED
// qconfig-bootstrapped.h is not supposed to be a part of the synced header files. So we find it by
// the include path specified for Bootstrap library in the source tree instead of the build tree as
// it's done for regular header files.
#include "qconfig-bootstrapped.h"
#else
#include <BobUICore/bobuicore-config.h>
#endif

#endif // BOBUICONFIGINCLUDE_H
