// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDARWINPERMISSIONPLUGIN_P_H
#define QDARWINPERMISSIONPLUGIN_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. This header file may change
// from version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/qnamespace.h>
#include <BobUICore/private/qpermissions_p.h>
#include <BobUICore/private/qcore_mac_p.h>

#if defined(__OBJC__)
#include <Foundation/NSObject.h>
#endif

BOBUI_USE_NAMESPACE

using namespace QPermissions::Private;

#if defined(__OBJC__)
Q_CORE_EXPORT
#endif
BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QDarwinPermissionHandler, NSObject
- (BobUI::PermissionStatus)checkPermission:(QPermission)permission;
- (void)requestPermission:(QPermission)permission withCallback:(PermissionCallback)callback;
- (QStringList)usageDescriptionsFor:(QPermission)permission;
)

BOBUI_BEGIN_NAMESPACE

class Q_CORE_EXPORT QDarwinPermissionPlugin : public QPermissionPlugin
{
    Q_OBJECT
public:
    QDarwinPermissionPlugin(QDarwinPermissionHandler *handler);
    ~QDarwinPermissionPlugin();

    BobUI::PermissionStatus checkPermission(const QPermission &permission) override;
    void requestPermission(const QPermission &permission, const PermissionCallback &callback) override;

private:
    Q_SLOT void permissionUpdated(BobUI::PermissionStatus status, const PermissionCallback &callback);
    bool verifyUsageDescriptions(const QPermission &permission);
    QDarwinPermissionHandler *m_handler = nullptr;
};

BOBUI_END_NAMESPACE

#endif // QDARWINPERMISSIONPLUGIN_P_H
