// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUINOOP_H
#define BOBUINOOP_H

#if 0
#pragma bobui_sync_stop_processing
#endif

#ifdef __cplusplus
[[maybe_unused]] constexpr
#endif
static inline void bobui_noop(void)
#ifdef __cplusplus
    noexcept
#endif
{}

#endif // BOBUINOOP_H
