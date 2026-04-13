// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qpa/qplatforminputcontextplugin_p.h>

#include <BobUICore/QStringList>

#include "qcomposeplatforminputcontext.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QComposePlatformInputContextPlugin : public QPlatformInputContextPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformInputContextFactoryInterface_iid FILE "compose.json")

public:
    QComposeInputContext *create(const QString &, const QStringList &) override;
};

QComposeInputContext *QComposePlatformInputContextPlugin::create(const QString &system, const QStringList &paramList)
{
    Q_UNUSED(paramList);

    if (system.compare(system, "compose"_L1, BobUI::CaseInsensitive) == 0
            || system.compare(system, "xim"_L1, BobUI::CaseInsensitive) == 0)
        return new QComposeInputContext;
    return nullptr;
}

BOBUI_END_NAMESPACE

#include "qcomposeplatforminputcontextmain.moc"
