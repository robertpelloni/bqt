// Copyright (C) 2018 Kitware, Inc.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QMOCK2PLUGIN_H
#define QMOCK2PLUGIN_H

#include <QObject>
#include <BobUIMockPlugins1/QMockPlugin>

BOBUI_BEGIN_NAMESPACE

class QMock2Plugin : public QObject, public QMockPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QMockPlugin_iid FILE "mock2plugin.json")
    Q_INTERFACES(QMockPlugin)
public:
    QString pluginName() const override;
};

BOBUI_END_NAMESPACE

#endif // QMOCK2PLUGIN_H
