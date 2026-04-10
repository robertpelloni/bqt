// Copyright (C) 2018 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUIPlugin>
#include <QObject>

class StaticPlugin1 : public QObject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "StaticPlugin1" FILE "plugin.json")
public:
    StaticPlugin1() {}
};

class StaticPlugin2 : public QObject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "StaticPlugin2" FILE "plugin.json")
public:
    StaticPlugin2() {}
};

#include "main.moc"
