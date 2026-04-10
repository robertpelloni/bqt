// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include <BobUIWidgets/qstyleplugin.h>
#include <BobUICore/qoperatingsystemversion.h>
#include "qwindowsvistastyle_p.h"
#include "qwindows11style_p.h"

BOBUI_BEGIN_NAMESPACE

class QModernWindowsStylePlugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QStyleFactoryInterface" FILE "modernwindowsstyles.json")
public:
    QStyle *create(const QString &key) override;
};

QStyle *QModernWindowsStylePlugin::create(const QString &key)
{
    if (key.compare(QLatin1String("windows11"), BobUI::CaseInsensitive) == 0)
        return new QWindows11Style();
    if (key.compare(QLatin1String("windowsvista"), BobUI::CaseInsensitive) == 0)
        return new QWindowsVistaStyle();
    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
