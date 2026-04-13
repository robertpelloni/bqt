// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#include <qpa/qplatformintegrationplugin.h>
#include "qoffscreenintegration.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QOffscreenIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "offscreen.json")
public:
    QPlatformIntegration *create(const QString&, const QStringList&) override;
};

QPlatformIntegration *QOffscreenIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    if (!system.compare("offscreen"_L1, BobUI::CaseInsensitive))
        return QOffscreenIntegration::createOffscreenIntegration(paramList);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
