// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qpa/qplatformintegrationplugin.h>
#include "qminimalintegration.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QMinimalIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "minimal.json")
public:
    QPlatformIntegration *create(const QString&, const QStringList&) override;
};

QPlatformIntegration *QMinimalIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    if (!system.compare("minimal"_L1, BobUI::CaseInsensitive))
        return new QMinimalIntegration(paramList);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
