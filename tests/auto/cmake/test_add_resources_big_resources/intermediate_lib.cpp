// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "leaf_lib.h"

#include <BobUICore/qfile.h>

namespace intermediate_lib {

bool isLeafLibResourceAvailable()
{
    return leaf_lib::isResourceAvailable();
}

bool isResourceAvailable()
{
    return QFile::exists(u":/resource3.txt"_qs);
}

} // namespace
