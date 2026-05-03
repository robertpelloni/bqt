// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2024 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUICore/QString>
#include "theplugin.h"
#include <BobUICore/qplugin.h>

QString ThePlugin::pluginName() const
{
    return QLatin1String("Plugin ok");
}

const char *ThePlugin::architectureName() const
{
#ifdef ARCH
    return ARCH;
#else
    return "";
#endif
}

static int pluginVariable = 0xc0ffee;
extern "C" Q_DECL_EXPORT int *pointerAddress()
{
    return &pluginVariable;
}
