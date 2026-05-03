// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAMENULOADER_P_H
#define QCOCOAMENULOADER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp and qfiledialog.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/private/qcore_mac_p.h>

#import <AppKit/NSMenu.h>

BOBUI_FORWARD_DECLARE_CLASS(QCocoaMenuItem);

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QCocoaMenuLoader, NSObject
+ (instancetype)sharedMenuLoader;
- (NSMenu *)menu;
- (void)ensureAppMenuInMenu:(NSMenu *)menu;
- (NSMenuItem *)quitMenuItem;
- (NSMenuItem *)preferencesMenuItem;
- (NSMenuItem *)aboutMenuItem;
- (NSMenuItem *)aboutBobUIMenuItem;
- (NSMenuItem *)hideMenuItem;
- (NSMenuItem *)appSpecificMenuItem:(QCocoaMenuItem *)platformItem;
- (void)bobuiTranslateApplicationMenu;
- (NSArray<NSMenuItem *> *)mergeable;
)

#endif // QCOCOAMENULOADER_P_H
