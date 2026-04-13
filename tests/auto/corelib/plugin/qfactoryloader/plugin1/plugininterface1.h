// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef PLUGININTERFACE1_H
#define PLUGININTERFACE1_H

#include <BobUICore/BobUIGlobal>

struct PluginInterface1 {
    virtual ~PluginInterface1() {}
    virtual QString pluginName() const = 0;
};

BOBUI_BEGIN_NAMESPACE

#define PluginInterface1_iid "org.bobui-project.BobUI.autotests.plugininterface1"

Q_DECLARE_INTERFACE(PluginInterface1, PluginInterface1_iid)

BOBUI_END_NAMESPACE

#endif // PLUGININTERFACE1_H
