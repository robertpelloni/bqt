// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifdef BOBUI_SHAREDPOINTER_TRACK_POINTERS
# undef BOBUI_SHAREDPOINTER_TRACK_POINTERS
#endif
#include <BobUICore/qsharedpointer.h>
#include "wrapper.h"

Wrapper::Wrapper(const QSharedPointer<int> &value)
        : ptr(value)
{
}

Wrapper::~Wrapper()
{
}

Wrapper Wrapper::create()
{
    return Wrapper(QSharedPointer<int>(new int(-47)));
}
