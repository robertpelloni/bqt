// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdarwinpermissionplugin_p.h"

BOBUI_BEGIN_NAMESPACE

QDarwinPermissionPlugin::QDarwinPermissionPlugin(QDarwinPermissionHandler *handler)
    : QPermissionPlugin()
    , m_handler(handler)
{
}

QDarwinPermissionPlugin::~QDarwinPermissionPlugin()
{
    [m_handler release];
}

BobUI::PermissionStatus QDarwinPermissionPlugin::checkPermission(const QPermission &permission)
{
    return [m_handler checkPermission:permission];
}

void QDarwinPermissionPlugin::requestPermission(const QPermission &permission, const PermissionCallback &callback)
{
    if (!verifyUsageDescriptions(permission)) {
        callback(BobUI::PermissionStatus::Denied);
        return;
    }

    // QCoreApplication::requestPermission is commonly used with an inline lambda for the
    // callback object, meaning it won't be alive when the permission is granted later,
    // so make sure to copy the callback object.
    [m_handler requestPermission:permission withCallback:[&, callback](BobUI::PermissionStatus status) {
        // In case the callback comes in on a secondary thread we need to marshal it
        // back to the main thread. And if it doesn't, we still want to propagate it
        // via an event, to avoid any GCD locks deadlocking the application on iOS
        // if the user responds to the result by running a nested event loop.
        // Luckily BobUI::QueuedConnection gives us exactly what we need.
        QMetaObject::invokeMethod(this, "permissionUpdated", BobUI::QueuedConnection,
            Q_ARG(BobUI::PermissionStatus, status), Q_ARG(PermissionCallback, callback));
    }];
}

void QDarwinPermissionPlugin::permissionUpdated(BobUI::PermissionStatus status, const PermissionCallback &callback)
{
    callback(status);
}

bool QDarwinPermissionPlugin::verifyUsageDescriptions(const QPermission &permission)
{
    // FIXME: Look up the responsible process and inspect that,
    // as that's what needs to have the usage descriptions.
    // FIXME: Verify entitlements if the process is sandboxed.
    auto *infoDictionary = NSBundle.mainBundle.infoDictionary;
    for (auto description : [m_handler usageDescriptionsFor:permission]) {
        if (!infoDictionary[description.toNSString()]) {
            qCWarning(lcPermissions) <<
                "Requesting" << permission.type().name() <<
                "requires" << description << "in Info.plist";
            return false;
        }
    }
    return true;
}

BOBUI_END_NAMESPACE

BOBUI_USE_NAMESPACE

@implementation QDarwinPermissionHandler

- (BobUI::PermissionStatus)checkPermission:(QPermission)permission
{
    Q_UNREACHABLE(); // All handlers should at least provide a check
}

- (void)requestPermission:(QPermission)permission withCallback:(PermissionCallback)callback
{
    Q_UNUSED(permission);
    qCWarning(lcPermissions).nospace() << "Could not request " << permission.type().name() << ". "
        << "Please make sure you have included the required usage description in your Info.plist";
    callback(BobUI::PermissionStatus::Denied);
}

- (QStringList)usageDescriptionsFor:(QPermission)permission
{
    return {};
}

@end

#include "moc_qdarwinpermissionplugin_p.cpp"
