// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <qpa/qplatforminputcontextplugin_p.h>
#include <BobUICore/QStringList>
#include <QDBusMetaType>
#include "qibusplatforminputcontext.h"
#include "qibustypes.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QIbusPlatformInputContextPlugin : public QPlatformInputContextPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformInputContextFactoryInterface_iid FILE "ibus.json")

public:
    QIBusPlatformInputContext *create(const QString&, const QStringList&) override;
};

QIBusPlatformInputContext *QIbusPlatformInputContextPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);

    if (system.compare(system, "ibus"_L1, BobUI::CaseInsensitive) == 0) {
        qDBusRegisterMetaType<QIBusAttribute>();
        qDBusRegisterMetaType<QIBusAttributeList>();
        qDBusRegisterMetaType<QIBusText>();
        qDBusRegisterMetaType<QIBusPropTypeClientCommitPreedit>();
        qDBusRegisterMetaType<QIBusPropTypeContentType>();
        return new QIBusPlatformInputContext;
    }

    return nullptr;
}

BOBUI_END_NAMESPACE

#include "main.moc"
