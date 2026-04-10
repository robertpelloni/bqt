// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QUIACCESSIBILITYELEMENT_H
#define QUIACCESSIBILITYELEMENT_H

#import <UIKit/UIKit.h>
#import <BobUIGui/BobUIGui>

#if BOBUI_CONFIG(accessibility)

@interface BOBUI_MANGLE_NAMESPACE(QMacAccessibilityElement) : UIAccessibilityElement

@property (readonly) QAccessible::Id axid;

+ (instancetype)elementWithId:(QAccessible::Id)anId;

@end

#endif
#endif
