// Copyright (C) 2022 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "forwarddeclared.h"

struct MyForwardDeclaredType { };
static MyForwardDeclaredType t;

const MyForwardDeclaredType &getForwardDeclaredType() noexcept
{
    return t;
}

MyForwardDeclaredType *getForwardDeclaredPointer() noexcept
{
    return &t;
}
