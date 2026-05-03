// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwindowsdirect2dintegration.h"

#include <BobUIGui/qpa/qplatformintegrationplugin.h>
#include <BobUICore/qstringlist.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QWindowsDirect2DIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "direct2d.json")
public:
    QPlatformIntegration *create(const QString&, const QStringList&) override;
};

QPlatformIntegration *QWindowsDirect2DIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    if (system.compare(system, "direct2d"_L1, BobUI::CaseInsensitive) == 0)
        return QWindowsDirect2DIntegration::create(paramList);
    return nullptr;
}

BOBUI_END_NAMESPACE

#include "qwindowsdirect2dplatformplugin.moc"
