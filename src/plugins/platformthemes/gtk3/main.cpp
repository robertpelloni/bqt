// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qpa/qplatformthemeplugin.h>
#include "qgtk3theme.h"

BOBUI_BEGIN_NAMESPACE

class QGtk3ThemePlugin : public QPlatformThemePlugin
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID QPlatformThemeFactoryInterface_iid FILE "gtk3.json")

public:
    QPlatformTheme *create(const QString &key, const QStringList &params) override;
};

QPlatformTheme *QGtk3ThemePlugin::create(const QString &key, const QStringList &params)
{
    Q_UNUSED(params);
    if (!key.compare(QLatin1StringView(QGtk3Theme::name), BobUI::CaseInsensitive))
        return new QGtk3Theme;

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
