// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#include "token.h"

BOBUI_BEGIN_NAMESPACE

#if defined(DEBUG_MOC)
const char *tokenTypeName(Token t)
{
    switch (t) {
#define CREATE_CASE(Name) case Name: return #Name;
    FOR_ALL_TOKENS(CREATE_CASE)
#undef CREATE_CASE
    }
    return "";
}
#endif

BOBUI_END_NAMESPACE
