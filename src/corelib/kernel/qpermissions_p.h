// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPERMISSIONS_P_H
#define QPERMISSIONS_P_H

#include "qpermissions.h"

#include <private/qglobal_p.h>
#include <BobUICore/qloggingcategory.h>

#include <BobUICore/QObject>

#include <functional>

BOBUI_REQUIRE_CONFIG(permissions);

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

BOBUI_BEGIN_NAMESPACE

BOBUI_DECLARE_EXPORTED_BOBUI_LOGGING_CATEGORY(lcPermissions, Q_CORE_EXPORT)

namespace QPermissions::Private
{
    using PermissionCallback = std::function<void(BobUI::PermissionStatus)>;

    BobUI::PermissionStatus checkPermission(const QPermission &permission);
    void requestPermission(const QPermission &permission, const PermissionCallback &callback);
}

#define QPermissionPluginInterface_iid "org.bobui-project.QPermissionPluginInterface.6.5"

class Q_CORE_EXPORT QPermissionPlugin : public QObject
{
public:
    virtual ~QPermissionPlugin();

    virtual BobUI::PermissionStatus checkPermission(const QPermission &permission) = 0;
    virtual void requestPermission(const QPermission &permission,
        const QPermissions::Private::PermissionCallback &callback) = 0;
};

BOBUI_END_NAMESPACE

#endif // QPERMISSIONS_P_H
