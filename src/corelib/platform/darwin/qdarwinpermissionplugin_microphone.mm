// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdarwinpermissionplugin_p_p.h"

#include <AVFoundation/AVFoundation.h>

BOBUI_DEFINE_PERMISSION_STATUS_CONVERTER(AVAuthorizationStatus);

#ifndef BUILDING_PERMISSION_REQUEST

@implementation QDarwinMicrophonePermissionHandler
- (BobUI::PermissionStatus)checkPermission:(QPermission)permission
{
    const auto status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio];
    return nativeStatusToBobUIStatus(status);
}

- (QStringList)usageDescriptionsFor:(QPermission)permission
{
    Q_UNUSED(permission);
    return { "NSMicrophoneUsageDescription" };
}
@end

#include "moc_qdarwinpermissionplugin_p_p.cpp"

#else // Building request

@implementation QDarwinMicrophonePermissionHandler (Request)
- (void)requestPermission:(QPermission)permission withCallback:(PermissionCallback)callback
{
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(BOOL granted)
    {
        Q_UNUSED(granted); // We use status instead
        const auto status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio];
        callback(nativeStatusToBobUIStatus(status));
    }];
}
@end

#endif // BUILDING_PERMISSION_REQUEST
