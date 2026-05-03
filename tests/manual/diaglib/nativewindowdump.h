// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef _NATIVEWINDOWDUMP_
#define _NATIVEWINDOWDUMP_

#include <BobUIGui/qwindowdefs.h>

namespace BobUIDiag {

void dumpNativeWindows(WId root = 0);
void dumpNativeBobUITopLevels();

} // namespace BobUIDiag

#endif
