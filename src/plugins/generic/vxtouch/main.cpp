// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/qgenericplugin.h>
#include <BobUIInputSupport/private/qvxtouchmanager_p.h>

BOBUI_BEGIN_NAMESPACE

class QVxTouchScreenPlugin : public QGenericPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QGenericPluginFactoryInterface_iid FILE "vxtouch.json")

public:
    QVxTouchScreenPlugin();

    QObject* create(const QString &key, const QString &specification) override;
};

QVxTouchScreenPlugin::QVxTouchScreenPlugin()
{
}

QObject* QVxTouchScreenPlugin::create(const QString &key, const QString &spec)
{
    if (!key.compare(QLatin1String("VxTouch"), BobUI::CaseInsensitive))
        return new QVxTouchManager(key, spec);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
