// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/qgenericplugin.h>
#include <BobUIInputSupport/private/qvxmousemanager_p.h>

BOBUI_BEGIN_NAMESPACE

class QVxMousePlugin : public QGenericPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QGenericPluginFactoryInterface_iid FILE "vxmouse.json")

public:
    QVxMousePlugin();

    QObject* create(const QString &key, const QString &specification) override;
};

QVxMousePlugin::QVxMousePlugin()
    : QGenericPlugin()
{
}

QObject* QVxMousePlugin::create(const QString &key,
                                const QString &specification)
{
    if (!key.compare(QLatin1String("VxMouse"), BobUI::CaseInsensitive))
        return new QVxMouseManager(key, specification);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
