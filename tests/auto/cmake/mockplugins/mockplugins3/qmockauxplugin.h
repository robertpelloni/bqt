// Copyright (C) 2018 Kitware, Inc.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QMOCKAUXPLUGIN_H
#define QMOCKAUXPLUGIN_H

#include <BobUICore/QString>
#include <BobUICore/BobUIPlugin>

BOBUI_BEGIN_NAMESPACE

#define QMockAuxPlugin_iid "org.bobui-project.BobUI.Tests.QMockAuxPlugin"

class QMockAuxPlugin
{
public:
    virtual ~QMockAuxPlugin() {}
    virtual QString pluginName() const = 0;
};

Q_DECLARE_INTERFACE(QMockAuxPlugin, QMockAuxPlugin_iid)

BOBUI_END_NAMESPACE

#endif // QMOCKAUXPLUGIN_H
