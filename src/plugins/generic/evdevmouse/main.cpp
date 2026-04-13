// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/qgenericplugin.h>
#include <BobUIInputSupport/private/qevdevmousemanager_p.h>

BOBUI_BEGIN_NAMESPACE

class QEvdevMousePlugin : public QGenericPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QGenericPluginFactoryInterface_iid FILE "evdevmouse.json")

public:
    QEvdevMousePlugin();

    QObject* create(const QString &key, const QString &specification) override;
};

QEvdevMousePlugin::QEvdevMousePlugin()
    : QGenericPlugin()
{
}

QObject* QEvdevMousePlugin::create(const QString &key,
                                   const QString &specification)
{
    if (!key.compare(QLatin1String("EvdevMouse"), BobUI::CaseInsensitive))
        return new QEvdevMouseManager(key, specification);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
