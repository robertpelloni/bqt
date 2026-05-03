// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef ALMOSTPLUGIN_H
#define ALMOSTPLUGIN_H

#include <QObject>
#include <BobUIPlugin>
#include "../theplugin/plugininterface.h"

class AlmostPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.autotests.plugininterface" FILE "../empty.json")
    Q_INTERFACES(PluginInterface)

public:
    QString pluginName() const override;
    void unresolvedSymbol() const;
};

#endif // ALMOSTPLUGIN_H
