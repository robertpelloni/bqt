// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <AppKit/AppKit.h>

#include <qpa/qplatformintegrationplugin.h>
#include <qpa/qplatformthemeplugin.h>
#include "qcocoaintegration.h"
#include "qcocoatheme.h"

#include <BobUICore/private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QCocoaIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "cocoa.json")
public:
    QPlatformIntegration *create(const QString&, const QStringList&);
};

QPlatformIntegration * QCocoaIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    QMacAutoReleasePool pool;
    if (system.compare("cocoa"_L1, BobUI::CaseInsensitive) == 0)
        return new QCocoaIntegration(paramList);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
