// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qfile.h>

namespace leaf_lib {

bool isResourceAvailable()
{
    return QFile::exists(u":/resource2.txt"_qs);
}

} // namespace
