// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAACCESIBILITYELEMENT_H
#define QCOCOAACCESIBILITYELEMENT_H

#include <BobUIGui/bobuiguiglobal.h>

#if BOBUI_CONFIG(accessibility)

#include <BobUICore/private/qcore_mac_p.h>
#include <BobUIGui/qaccessible.h>

#import <AppKit/NSAccessibilityElement.h>

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QMacAccessibilityElement, NSObject <NSAccessibilityElement>
- (instancetype)initWithId:(QAccessible::Id)anId;
- (instancetype)initWithId:(QAccessible::Id)anId role:(NSAccessibilityRole)role;
+ (instancetype)elementWithId:(QAccessible::Id)anId;
+ (instancetype)elementWithInterface:(QAccessibleInterface *)iface;
+ (void)removeElementsFromCache:(NSArray *)array;
- (void)updateTableModel;
- (QAccessibleInterface *)bobuiInterface;
)

#endif // BOBUI_CONFIG(accessibility)

#endif // QCOCOAACCESIBILITYELEMENT_H
