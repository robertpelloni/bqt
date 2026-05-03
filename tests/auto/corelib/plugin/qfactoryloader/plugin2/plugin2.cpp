// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUICore/qstring.h>
#include "plugin2.h"

QString Plugin2::pluginName() const
{
    return QLatin1String("Plugin2 ok");
}

#include "moc_plugin2.cpp"
