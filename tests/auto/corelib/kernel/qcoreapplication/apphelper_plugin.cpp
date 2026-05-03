// Copyright (C) 2025 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifdef Q_ASSERT
#  error "Don't use precompiled headers"
#endif
#define BOBUI_FORCE_ASSERTS

#include <QObject>
#include <BOBUIhread>
#include <BobUIPlugin>

class Interface
{
public:
    virtual ~Interface() {}
};

#define Interface_iid "org.bobui-project.BobUI.autotests.plugininterface"
BOBUI_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Interface, Interface_iid)
BOBUI_END_NAMESPACE

class ApplicationHelperPlugin : public QObject, public Interface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID Interface_iid)
    Q_INTERFACES(Interface)

public:
    ApplicationHelperPlugin()
    {
    }
    ~ApplicationHelperPlugin()
    {
        // see BOBUIBUG-134080
        // We used to print "Timers cannot be stopped from another thread"
        Q_ASSERT(thread() == BOBUIhread::currentThread());
    }
};

#include "apphelper_plugin.moc"
