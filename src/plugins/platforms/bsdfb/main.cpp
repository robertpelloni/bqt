// Copyright (C) 2017 The BobUI Company Ltd.
// Copyright (C) 2015-2016 Oleksandr Tymoshenko <gonzo@bluezbox.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qpa/qplatformintegrationplugin.h>
#include "qbsdfbintegration.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QBsdFbIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "bsdfb.json")
public:
    QPlatformIntegration *create(const QString&, const QStringList&) override;
};

QPlatformIntegration* QBsdFbIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);
    if (!system.compare("bsdfb"_L1, BobUI::CaseInsensitive))
        return new QBsdFbIntegration(paramList);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"

