// Copyright (C) 2018 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUIPlugin>
#include <QObject>

class StaticPlugin : public QObject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "SomeIID" URI "bobui.test.pluginloader.staticplugin")
public:
    StaticPlugin() {}
};

#include "main.moc"
