// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qpa/qplatformintegrationplugin.h>
#include <qpa/qplatformthemeplugin.h>
#include "qiosintegration.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QIOSIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
        Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "ios.json")
    public:
        QPlatformIntegration *create(const QString&, const QStringList&);
};

QPlatformIntegration * QIOSIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);
    if (!system.compare("ios"_L1, BobUI::CaseInsensitive)
        || !system.compare("tvos"_L1, BobUI::CaseInsensitive)) {
        return new QIOSIntegration;
    }

    return 0;
}

BOBUI_END_NAMESPACE

#include "plugin.moc"

// Dummy function that we explicitly tell the linker to look for,
// so that the plugin's static initializer is included and run.
extern "C" void bobui_registerPlatformPlugin() {}

Q_IMPORT_PLUGIN(QIOSIntegrationPlugin)
