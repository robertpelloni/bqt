// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDARWINPERMISSIONPLUGIN_P_P_H
#define QDARWINPERMISSIONPLUGIN_P_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. This header file may change
// from version to version without notice, or even be removed.
//
// We mean it.
//

#if !defined(BOBUI_DARWIN_PERMISSION_PLUGIN)
#error "This header should only be included from permission plugins"
#endif

#include <BobUICore/qnamespace.h>
#include <BobUICore/private/qpermissions_p.h>
#include <BobUICore/private/qcore_mac_p.h>

#include "qdarwinpermissionplugin_p.h"

using namespace QPermissions::Private;

#ifndef BOBUI_JOIN
#define BOBUI_JOIN_IMPL(A, B) A ## B
#define BOBUI_JOIN(A, B) BOBUI_JOIN_IMPL(A, B)
#endif

#define PERMISSION_PLUGIN_NAME(SUFFIX) \
    BOBUI_JOIN(BOBUI_JOIN(BOBUI_JOIN( \
        QDarwin, BOBUI_DARWIN_PERMISSION_PLUGIN), Permission), SUFFIX)

#define PERMISSION_PLUGIN_CLASSNAME PERMISSION_PLUGIN_NAME(Plugin)
#define PERMISSION_PLUGIN_HANDLER PERMISSION_PLUGIN_NAME(Handler)

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(
    PERMISSION_PLUGIN_HANDLER,
    QDarwinPermissionHandler
)

BOBUI_BEGIN_NAMESPACE

class Q_CORE_EXPORT PERMISSION_PLUGIN_CLASSNAME : public QDarwinPermissionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(
        IID QPermissionPluginInterface_iid
        FILE "QDarwin" BOBUI_STRINGIFY(BOBUI_DARWIN_PERMISSION_PLUGIN) "PermissionPlugin.json")
public:
    PERMISSION_PLUGIN_CLASSNAME()
        : QDarwinPermissionPlugin([[PERMISSION_PLUGIN_HANDLER alloc] init])
    {}
};

BOBUI_END_NAMESPACE

// Request
#if defined(BUILDING_PERMISSION_REQUEST)
// dummy symbol to be resolved by the linker to force loading of static library
// (compare -Wl,-u)
extern "C" void BOBUI_MANGLE_NAMESPACE(PERMISSION_PLUGIN_NAME(Request))() { }
#endif

// -------------------------------------------------------

namespace {
template <typename NativeStatus>
struct NativeStatusHelper;

template <typename NativeStatus>
BobUI::PermissionStatus nativeStatusToBobUIStatus(NativeStatus status)
{
    using Converter = NativeStatusHelper<NativeStatus>;
    switch (status) {
    case Converter::Authorized:
        return BobUI::PermissionStatus::Granted;
    case Converter::Denied:
    case Converter::Restricted:
        return BobUI::PermissionStatus::Denied;
    case Converter::Undetermined:
        return BobUI::PermissionStatus::Undetermined;
    }
    qCWarning(lcPermissions) << "Unknown permission status" << status << "detected in"
        << BOBUI_STRINGIFY(BOBUI_DARWIN_PERMISSION_PLUGIN);
    return BobUI::PermissionStatus::Denied;
}
} // namespace

#define BOBUI_DEFINE_PERMISSION_STATUS_CONVERTER(NativeStatus) \
namespace { template<> \
struct NativeStatusHelper<NativeStatus> \
{\
    enum { \
        Authorized = NativeStatus##Authorized, \
        Denied = NativeStatus##Denied, \
        Restricted = NativeStatus##Restricted, \
        Undetermined = NativeStatus##NotDetermined \
    }; \
}; }

#endif // QDARWINPERMISSIONPLUGIN_P_P_H
