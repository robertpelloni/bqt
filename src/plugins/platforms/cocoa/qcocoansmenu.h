// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOANSMENU_H
#define QCOCOANSMENU_H

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

#include <BobUICore/private/qcore_mac_p.h>

#import <AppKit/NSMenu.h>

BOBUI_FORWARD_DECLARE_CLASS(QCocoaMenu);
BOBUI_FORWARD_DECLARE_CLASS(QCocoaMenuItem);

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QCocoaNSMenuDelegate, NSObject <NSMenuDelegate>
+ (instancetype)sharedMenuDelegate;
- (NSMenuItem *)findItemInMenu:(NSMenu *)menu forKey:(NSString *)key modifiers:(NSUInteger)modifiers;
)

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QCocoaNSMenu, NSMenu
@property (readonly, nonatomic) QCocoaMenu *platformMenu;
- (instancetype)initWithPlatformMenu:(QCocoaMenu *)menu;
- (instancetype)initWithoutPlatformMenu:(NSString *)menu;
)

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QCocoaNSMenuItem, NSMenuItem
@property (nonatomic) QCocoaMenuItem *platformMenuItem;
+ (instancetype)separatorItemWithPlatformMenuItem:(QCocoaMenuItem *)menuItem;
- (instancetype)initWithPlatformMenuItem:(QCocoaMenuItem *)menuItem;
- (instancetype)init;
)

#endif // QCOCOANSMENU_H
