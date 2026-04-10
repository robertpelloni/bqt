// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobui_xlib_wrapper.h"

#include <X11/Xlib.h>

void bobui_XFlush(Display *dpy) { XFlush(dpy); }
