// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <BobUICore/BobUIGlobal>

struct PluginInterface {
    virtual ~PluginInterface() {}
    virtual QString pluginName() const = 0;
    virtual const char *architectureName() const { return ""; };
};

BOBUI_BEGIN_NAMESPACE

#define PluginInterface_iid "org.bobui-project.BobUI.autotests.plugininterface"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

BOBUI_END_NAMESPACE

#endif // PLUGININTERFACE_H

