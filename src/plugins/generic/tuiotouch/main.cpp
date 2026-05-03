// Copyright (C) 2014 Robin Burchell <robin.burchell@viroteck.net>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/qgenericplugin.h>
#include <QCoreApplication>

#include "bobuiuiohandler_p.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIuioTouchPlugin : public QGenericPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QGenericPluginFactoryInterface_iid FILE "tuiotouch.json")

public:
    BOBUIuioTouchPlugin();

    QObject* create(const QString &key, const QString &specification) override;
};

BOBUIuioTouchPlugin::BOBUIuioTouchPlugin()
{
}

QObject* BOBUIuioTouchPlugin::create(const QString &key,
                                         const QString &spec)
{
    if (!key.compare(QLatin1String("TuioTouch"), BobUI::CaseInsensitive))
        return new BOBUIuioHandler(spec);

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
