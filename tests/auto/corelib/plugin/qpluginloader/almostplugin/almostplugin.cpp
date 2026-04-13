// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUICore/QString>
#include "almostplugin.h"
#include <BobUICore/qplugin.h>

QString AlmostPlugin::pluginName() const
{
    unresolvedSymbol();
    return QLatin1String("Plugin ok");
}
