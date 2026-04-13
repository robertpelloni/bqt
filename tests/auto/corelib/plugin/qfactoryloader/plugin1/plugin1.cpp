// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUICore/qstring.h>
#include "plugin1.h"

QString Plugin1::pluginName() const
{
    return QLatin1String("Plugin1 ok");
}

#include "moc_plugin1.cpp"
