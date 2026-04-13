// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QNATIVEINTERFACE_P_H
#define QNATIVEINTERFACE_P_H

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

#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/qloggingcategory.h>

BOBUI_BEGIN_NAMESPACE

BOBUI_DECLARE_EXPORTED_BOBUI_LOGGING_CATEGORY(lcNativeInterface, Q_CORE_EXPORT)

// Provides a definition for the interface destructor
#define BOBUI_DEFINE_NATIVE_INTERFACE_2(Namespace, InterfaceClass)                                    \
    BOBUI_PREPEND_NAMESPACE(Namespace)::InterfaceClass::~InterfaceClass() = default

#define BOBUI_DEFINE_NATIVE_INTERFACE(...)                                                            \
    BOBUI_OVERLOADED_MACRO(BOBUI_DEFINE_NATIVE_INTERFACE, QNativeInterface, __VA_ARGS__)
#define BOBUI_DEFINE_PRIVATE_NATIVE_INTERFACE(...)                                                    \
    BOBUI_OVERLOADED_MACRO(BOBUI_DEFINE_NATIVE_INTERFACE, QNativeInterface::Private, __VA_ARGS__)

#define BOBUI_NATIVE_INTERFACE_RETURN_IF(NativeInterface, baseType)                                   \
    {                                                                                              \
        using QNativeInterface::Private::TypeInfo;                                                 \
        qCDebug(lcNativeInterface, "Comparing requested interface name %s with available %s",      \
                name, TypeInfo<NativeInterface>::name());                                          \
        if (qstrcmp(name, TypeInfo<NativeInterface>::name()) == 0) {                               \
            qCDebug(lcNativeInterface,                                                             \
                    "Match for interface %s. Comparing revisions (requested %d / available %d)",   \
                    name, revision, TypeInfo<NativeInterface>::revision());                        \
            if (revision == TypeInfo<NativeInterface>::revision()) {                               \
                qCDebug(lcNativeInterface, "Full match. Returning dynamic cast of %p", baseType);  \
                return dynamic_cast<NativeInterface *>(baseType);                                  \
            } else {                                                                               \
                qCWarning(lcNativeInterface,                                                       \
                          "Native interface revision mismatch (requested %d / available %d) for "  \
                          "interface %s",                                                          \
                          revision, TypeInfo<NativeInterface>::revision(), name);                  \
                return nullptr;                                                                    \
            }                                                                                      \
        } else {                                                                                   \
            qCDebug(lcNativeInterface, "No match for requested interface name %s", name);          \
        }                                                                                          \
    }

BOBUI_END_NAMESPACE

#endif // QNATIVEINTERFACE_P_H
