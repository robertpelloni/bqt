// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUIWidgets/qstyleplugin.h>
#include "qandroidstyle_p.h"

BOBUI_BEGIN_NAMESPACE

class QAndroidStylePlugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QStyleFactoryInterface" FILE "androidstyle.json")
public:
    QStyle *create(const QString &key);
};

QStyle *QAndroidStylePlugin::create(const QString &key)
{
    if (key.compare(QLatin1String("android"), BobUI::CaseInsensitive) == 0)
        return new QAndroidStyle();

    return 0;
}

BOBUI_END_NAMESPACE

#include "main.moc"

