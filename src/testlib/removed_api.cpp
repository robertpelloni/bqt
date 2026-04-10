// Copyright (C) 2021 The BobUI Company Ltd.
// Copyright (C) 2024 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#define BOBUI_TESTLIB_BUILD_REMOVED_API

#include "bobuiest.h"

#if BOBUI_TESTLIB_REMOVED_SINCE(6, 8)

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest {

Q_TESTLIB_EXPORT char *toString(const void *p)
{
    const volatile void *ptr = p;
    return toString(ptr);
}

} // namespace BOBUIest

BOBUI_END_NAMESPACE

// #include "qotherheader.h"
// implement removed functions from qotherheader.h
// order sections alphabetically to reduce chances of merge conflicts

#endif // BOBUI_TESTLIB_REMOVED_SINCE(6, 8)
