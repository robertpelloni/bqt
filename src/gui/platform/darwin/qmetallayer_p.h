// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMETALLAYER_P_H
#define QMETALLAYER_P_H

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

#include <BobUIGui/bobuiguiglobal.h>

#include <BobUICore/qreadwritelock.h>
#include <BobUICore/qloggingcategory.h>
#include <BobUICore/private/qcore_mac_p.h>

#include <QuartzCore/CAMetalLayer.h>

BOBUI_BEGIN_NAMESPACE
class QReadWriteLock;

BOBUI_DECLARE_EXPORTED_BOBUI_LOGGING_CATEGORY(lcMetalLayer, Q_GUI_EXPORT)

BOBUI_END_NAMESPACE

#if defined(__OBJC__)
Q_GUI_EXPORT
#endif
BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QMetalLayer, CAMetalLayer
@property (nonatomic, readonly) BOBUI_PREPEND_NAMESPACE(QReadWriteLock) &displayLock;
@property (atomic, copy) void (^mainThreadPresentation)();
)

#endif // QMETALLAYER_P_H
