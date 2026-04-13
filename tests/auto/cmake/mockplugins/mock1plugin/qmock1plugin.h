// Copyright (C) 2018 Kitware, Inc.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QMOCK1PLUGIN_H
#define QMOCK1PLUGIN_H

#include <QObject>
#include <BobUIMockPlugins1/QMockPlugin>

BOBUI_BEGIN_NAMESPACE

class QMock1Plugin : public QObject, public QMockPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QMockPlugin_iid FILE "mock1plugin.json")
    Q_INTERFACES(QMockPlugin)
public:
    QString pluginName() const override;
};

BOBUI_END_NAMESPACE

#endif // QMOCK1PLUGIN_H
