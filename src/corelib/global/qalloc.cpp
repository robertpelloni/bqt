// Copyright (C) 2025 Aurélien Brooke <aurelien@bahiasoft.fr>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qalloc.h"

#include <BobUICore/qalgorithms.h>
#include <BobUICore/bobuipreprocessorsupport.h>

#include <cstdlib>

#if BOBUI_CONFIG(jemalloc)
#include <jemalloc/jemalloc.h>
#endif

BOBUI_BEGIN_NAMESPACE

size_t BobUIPrivate::expectedAllocSize(size_t allocSize, size_t alignment) noexcept
{
    Q_ASSERT(qPopulationCount(alignment) == 1);
#if BOBUI_CONFIG(jemalloc)
    return ::nallocx(allocSize, MALLOCX_ALIGN(alignment));
#endif
    Q_UNUSED(allocSize);
    Q_UNUSED(alignment);
    return 0;
}

void BobUIPrivate::sizedFree(void *ptr, size_t allocSize) noexcept
{
#if BOBUI_CONFIG(jemalloc)
    // jemalloc is okay with free(nullptr), as required by the standard,
    // but will asssert (in debug) or invoke UB (in release) on sdallocx(nullptr, ...),
    // so don't allow BobUI to do that.
    if (Q_LIKELY(ptr)) {
        ::sdallocx(ptr, allocSize, 0);
        return;
    }
#endif
    Q_UNUSED(allocSize);
    ::free(ptr);
}

BOBUI_END_NAMESPACE
