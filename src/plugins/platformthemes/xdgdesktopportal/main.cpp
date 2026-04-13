// Copyright (C) 2017-2018 Red Hat, Inc
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qpa/qplatformthemeplugin.h>
#include "qxdgdesktopportaltheme.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QXdgDesktopPortalThemePlugin : public QPlatformThemePlugin
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID QPlatformThemeFactoryInterface_iid FILE "xdgdesktopportal.json")

public:
    QPlatformTheme *create(const QString &key, const QStringList &params) override;
};

QPlatformTheme *QXdgDesktopPortalThemePlugin::create(const QString &key, const QStringList &params)
{
    Q_UNUSED(params);
    return QXdgDesktopPortalTheme::isXdgPlugin(key) ? new QXdgDesktopPortalTheme : nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
