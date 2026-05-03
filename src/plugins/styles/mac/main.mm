// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUIWidgets/qstyleplugin.h>
#include "qmacstyle_mac_p.h"

#include <BobUICore/private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE

class QMacStylePlugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QStyleFactoryInterface" FILE "macstyle.json")
public:
    QStyle *create(const QString &key);
};

QStyle *QMacStylePlugin::create(const QString &key)
{
    QMacAutoReleasePool pool;
    if (key.compare(QLatin1String("macos"), BobUI::CaseInsensitive) == 0)
        return QMacStyle::create();

    return 0;
}

BOBUI_END_NAMESPACE

#include "main.moc"

