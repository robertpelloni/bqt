// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef THEPLUGIN_H
#define THEPLUGIN_H

#include <QObject>
#include <BobUIPlugin>
#include "plugininterface.h"

class ThePlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.autotests.plugininterface" FILE "../utf8_data.json")
    Q_INTERFACES(PluginInterface)

public:
    virtual QString pluginName() const override;
    const char *architectureName() const override;
};

#endif // THEPLUGIN_H

