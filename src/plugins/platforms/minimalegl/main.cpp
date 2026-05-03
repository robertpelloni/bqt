// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qpa/qplatformintegrationplugin.h>
#include "qminimaleglintegration.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QMinimalEglIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "minimalegl.json")
public:
    QPlatformIntegration *create(const QString&, const QStringList&) override;
};

QPlatformIntegration* QMinimalEglIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);
    if (!system.compare("minimalegl"_L1, BobUI::CaseInsensitive))
        return new QMinimalEglIntegration;

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
