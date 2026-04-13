// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/qgenericplugin.h>
#include <BobUIInputSupport/private/bobuislib_p.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIsLibPlugin : public QGenericPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QGenericPluginFactoryInterface_iid FILE "tslib.json")

public:
    QObject* create(const QString &key, const QString &specification) override;
};

QObject* BOBUIsLibPlugin::create(const QString &key,
                              const QString &specification)
{
    if (!key.compare(QLatin1String("Tslib"), BobUI::CaseInsensitive)
        || !key.compare(QLatin1String("TslibRaw"), BobUI::CaseInsensitive))
        return new BOBUIsLibMouseHandler(key, specification);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
