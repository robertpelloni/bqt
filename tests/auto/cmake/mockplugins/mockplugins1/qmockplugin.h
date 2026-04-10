// Copyright (C) 2018 Kitware, Inc.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QMOCKPLUGIN_H
#define QMOCKPLUGIN_H

#include <BobUICore/QString>
#include <BobUICore/BobUIPlugin>

BOBUI_BEGIN_NAMESPACE

#define QMockPlugin_iid "org.bobui-project.BobUI.Tests.QMockPlugin"

class QMockPlugin
{
public:
    virtual ~QMockPlugin() {}
    virtual QString pluginName() const = 0;
};

Q_DECLARE_INTERFACE(QMockPlugin, QMockPlugin_iid)

BOBUI_END_NAMESPACE

#endif // QMOCKPLUGIN_H
