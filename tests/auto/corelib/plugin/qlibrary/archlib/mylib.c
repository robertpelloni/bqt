// Copyright (C) 2024 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qglobal.h>

Q_DECL_EXPORT const char *archname()
{
#ifdef ARCH
    return ARCH;
#else
    return "";
#endif
}
