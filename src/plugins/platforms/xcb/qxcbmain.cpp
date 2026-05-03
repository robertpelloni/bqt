// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qpa/qplatformintegrationplugin.h>
#include "qxcbintegration.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QXcbIntegrationPlugin : public QPlatformIntegrationPlugin
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "xcb.json")
public:
    QPlatformIntegration *create(const QString&, const QStringList&, int &, char **) override;
};

QPlatformIntegration* QXcbIntegrationPlugin::create(const QString& system, const QStringList& parameters, int &argc, char **argv)
{
    if (!system.compare("xcb"_L1, BobUI::CaseInsensitive)) {
        auto xcbIntegration = new QXcbIntegration(parameters, argc, argv);
        if (!xcbIntegration->hasConnection()) {
            delete xcbIntegration;
            return nullptr;
        }
        return xcbIntegration;
    }

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "qxcbmain.moc"
