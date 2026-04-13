// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef PLUGININTERFACE2_H
#define PLUGININTERFACE2_H

#include <BobUICore/BobUIGlobal>

struct PluginInterface2 {
    virtual ~PluginInterface2() {}
    virtual QString pluginName() const = 0;
};

BOBUI_BEGIN_NAMESPACE

#define PluginInterface2_iid "org.bobui-project.BobUI.autotests.plugininterface2"

Q_DECLARE_INTERFACE(PluginInterface2, PluginInterface2_iid)

BOBUI_END_NAMESPACE

#endif // PLUGININTERFACE2_H
