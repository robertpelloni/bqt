// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdarwinpermissionplugin_p_p.h"

#include <deque>

#include <CoreBluetooth/CoreBluetooth.h>

@interface QDarwinBluetoothPermissionHandler () <CBCentralManagerDelegate>
@property (nonatomic, retain) CBCentralManager *manager;
@end

@implementation QDarwinBluetoothPermissionHandler {
    std::deque<PermissionCallback> m_callbacks;
}

- (instancetype)init
{
    if ((self = [super init]))
        self.manager = nil;

    return self;
}

- (BobUI::PermissionStatus)checkPermission:(QPermission)permission
{
    Q_UNUSED(permission);
    return [self currentStatus];
}

- (BobUI::PermissionStatus)currentStatus
{
    auto status = CBCentralManager.authorization;
    switch (status) {
    case CBManagerAuthorizationNotDetermined:
        return BobUI::PermissionStatus::Undetermined;
    case CBManagerAuthorizationRestricted:
    case CBManagerAuthorizationDenied:
        return BobUI::PermissionStatus::Denied;
    case CBManagerAuthorizationAllowedAlways:
        return BobUI::PermissionStatus::Granted;
    }

    qCWarning(lcPermissions) << "Unknown permission status" << status << "detected in" << self;
    return BobUI::PermissionStatus::Denied;
}

- (void)requestPermission:(QPermission)permission withCallback:(PermissionCallback)callback
{
    m_callbacks.push_back(callback);
    if (!self.manager) {
        self.manager = [[[CBCentralManager alloc]
            initWithDelegate:self queue:dispatch_get_main_queue()] autorelease];
    }
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)manager
{
    Q_ASSERT(manager == self.manager);
    Q_ASSERT(!m_callbacks.empty());

    auto status = [self currentStatus];

    for (auto callback : m_callbacks)
        callback(status);

    m_callbacks = {};
    self.manager.delegate = nil;
    self.manager = nil;
}

- (QStringList)usageDescriptionsFor:(QPermission)permission
{
    Q_UNUSED(permission);
#ifdef Q_OS_MACOS
    if (QOperatingSystemVersion::current() > QOperatingSystemVersion::MacOSBigSur)
#endif
    {
        return { "NSBluetoothAlwaysUsageDescription" };
    }

    return {};
}
@end

#include "moc_qdarwinpermissionplugin_p_p.cpp"
