// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTASSERT_H
#define BOBUIESTASSERT_H

#include <BobUICore/qglobal.h>

BOBUI_BEGIN_NAMESPACE

#define BOBUIEST_ASSERT(cond) ((cond) ? static_cast<void>(0) : bobui_assert(#cond, __FILE__, __LINE__))

#define BOBUIEST_ASSERT_X(cond, where, what) ((cond) ? static_cast<void>(0) : bobui_assert_x(where, what, __FILE__, __LINE__))

BOBUI_END_NAMESPACE

#endif
