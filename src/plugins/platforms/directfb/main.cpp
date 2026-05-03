// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qpa/qplatformintegrationplugin.h>
#include "qdirectfbintegration.h"
#include "qdirectfb_egl.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

#ifdef DIRECTFB_GL_EGL
#define BOBUI_EGL_BACKEND_STRING(list) list << "directfbegl";
#define BOBUI_EGL_BACKEND_CREATE(list, out) \
    if (list.toLower() == "directfbegl") \
        out = new QDirectFbIntegrationEGL;
#else
#define BOBUI_EGL_BACKEND_STRING(list)
#define BOBUI_EGL_BACKEND_CREATE(system, out)
#endif

class QDirectFbIntegrationPlugin : public QPlatformIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformIntegrationFactoryInterface_iid FILE "directfb.json")
public:
    QPlatformIntegration *create(const QString&, const QStringList&) override;
};

QPlatformIntegration * QDirectFbIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);
    QDirectFbIntegration *integration = nullptr;

    if (!system.compare("directfb"_L1, BobUI::CaseInsensitive))
        integration = new QDirectFbIntegration;
    BOBUI_EGL_BACKEND_CREATE(system, integration)

    if (!integration)
        return 0;

    integration->connectToDirectFb();

    return integration;
}

BOBUI_END_NAMESPACE

#include "main.moc"
