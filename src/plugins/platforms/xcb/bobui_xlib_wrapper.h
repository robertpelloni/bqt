// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _XDisplay Display;
    void bobui_XFlush(Display *dpy);

#ifdef __cplusplus
}
#endif
