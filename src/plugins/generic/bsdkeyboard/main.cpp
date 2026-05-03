// Copyright (C) 2015-2016 Oleksandr Tymoshenko <gonzo@bluezbox.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/qgenericplugin.h>
#include "qbsdkeyboard.h"

BOBUI_BEGIN_NAMESPACE

class QBsdKeyboardPlugin : public QGenericPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QGenericPluginFactoryInterface" FILE "bsdkeyboard.json")

public:
    QObject *create(const QString &key, const QString &specification) override;
};

QObject *QBsdKeyboardPlugin::create(const QString &key,
                                   const QString &specification)
{
    if (!key.compare(QLatin1String("BsdKeyboard"), BobUI::CaseInsensitive))
        return new QBsdKeyboardHandler(key, specification);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
