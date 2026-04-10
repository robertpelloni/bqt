// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qpermissions.h"
#include "qpermissions_p.h"

#include <BobUICore/private/qfactoryloader_p.h>
#include <BobUICore/private/qcoreapplication_p.h>
#include <BobUICore/qcborarray.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

namespace {

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, pluginLoader,
    (QPermissionPluginInterface_iid, QLatin1String("/permissions"), BobUI::CaseInsensitive))

QPermissionPlugin *permissionPlugin(const QPermission &permission)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    const char *permissionType = permission.type().name();
    qCDebug(lcPermissions, "Looking for permission plugin for %s", permissionType);

    if (Q_UNLIKELY(!pluginLoader)) {
        qCWarning(lcPermissions, "Cannot check or request permissions during application shutdown");
        return nullptr;
    }

    auto metaDataList = pluginLoader()->metaData();
    for (int i = 0; i < metaDataList.size(); ++i) {
        auto metaData = metaDataList.at(i).value(BobUIPluginMetaDataKeys::MetaData).toMap();
        auto permissions = metaData.value("Permissions"_L1).toArray();
        if (permissions.contains(QString::fromUtf8(permissionType))) {
            auto className = metaDataList.at(i).value(BobUIPluginMetaDataKeys::ClassName).toString();
            qCDebug(lcPermissions) << "Found matching plugin" << qUtf8Printable(className);
            auto *plugin = static_cast<QPermissionPlugin*>(pluginLoader()->instance(i));
            return plugin;
        }
    }

    qCWarning(lcPermissions).nospace() << "Could not find permission plugin for "
        << permission.type().name() << ". Please make sure you have included the "
        << "required usage description in your Info.plist";

    return nullptr;
}

} // Unnamed namespace

namespace QPermissions::Private
{
    BobUI::PermissionStatus checkPermission(const QPermission &permission)
    {
        if (auto *plugin = permissionPlugin(permission))
            return plugin->checkPermission(permission);
        else
            return BobUI::PermissionStatus::Denied;
    }

    void requestPermission(const QPermission &permission, const QPermissions::Private::PermissionCallback &callback)
    {
        if (auto *plugin = permissionPlugin(permission))
            plugin->requestPermission(permission, callback);
        else
            callback(BobUI::PermissionStatus::Denied);
    }
}

BOBUI_END_NAMESPACE
