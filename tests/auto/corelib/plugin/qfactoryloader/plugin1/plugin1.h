// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef THEPLUGIN_H
#define THEPLUGIN_H

#include <BobUICore/qobject.h>
#include <BobUICore/qplugin.h>
#include "plugininterface1.h"

class Plugin1 : public QObject, public PluginInterface1
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.autotests.plugininterface1" FILE "plugin1.json")
    Q_INTERFACES(PluginInterface1)

public:
    virtual QString pluginName() const override;
};

#endif // THEPLUGIN_H
