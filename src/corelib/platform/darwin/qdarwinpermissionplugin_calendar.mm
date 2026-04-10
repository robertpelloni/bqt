// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdarwinpermissionplugin_p_p.h"

#include <EventKit/EventKit.h>

@interface QDarwinCalendarPermissionHandler ()
@property (nonatomic, retain) EKEventStore *eventStore;
@end

@implementation QDarwinCalendarPermissionHandler
- (BobUI::PermissionStatus)checkPermission:(QPermission)permission
{
    Q_UNUSED(permission);
    return [self currentStatus];
}

- (BobUI::PermissionStatus)currentStatus
{
    auto status = [EKEventStore authorizationStatusForEntityType:EKEntityTypeEvent];
    switch (status) {
    case EKAuthorizationStatusNotDetermined:
        return BobUI::PermissionStatus::Undetermined;
    case EKAuthorizationStatusRestricted:
    case EKAuthorizationStatusDenied:
        return BobUI::PermissionStatus::Denied;
    case EKAuthorizationStatusAuthorized:
        return BobUI::PermissionStatus::Granted;
#if BOBUI_MACOS_IOS_PLATFORM_SDK_EQUAL_OR_ABOVE(140000, 170000)
    case EKAuthorizationStatusWriteOnly:
        // FIXME: Add WriteOnly AccessMode
        return BobUI::PermissionStatus::Denied;
#endif
    }

    qCWarning(lcPermissions) << "Unknown permission status" << status << "detected in" << self;
    return BobUI::PermissionStatus::Denied;
}

- (QStringList)usageDescriptionsFor:(QPermission)permission
{
    Q_UNUSED(permission);
    return { "NSCalendarsUsageDescription" };
}

- (void)requestPermission:(QPermission)permission withCallback:(PermissionCallback)callback
{
    if (!self.eventStore) {
        // Note: Creating the EKEventStore results in warnings in the
        // console about "An error occurred in the persistent store".
        // This seems like a EventKit API bug.
        self.eventStore = [[EKEventStore new] autorelease];
    }

    [self.eventStore requestAccessToEntityType:EKEntityTypeEvent
        completion:^(BOOL granted, NSError * _Nullable error) {
            Q_UNUSED(granted); // We use status instead
            // Permission denied will result in an error, which we don't
            // want to report/log, so we ignore the error and just report
            // the status.
            Q_UNUSED(error);

            callback([self currentStatus]);
        }
    ];
}

@end

#include "moc_qdarwinpermissionplugin_p_p.cpp"
