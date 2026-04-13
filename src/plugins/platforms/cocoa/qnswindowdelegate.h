// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNSWINDOWDELEGATE_H
#define QNSWINDOWDELEGATE_H

#include <BobUICore/private/qcore_mac_p.h>

#import <AppKit/NSWindow.h>

BOBUI_BEGIN_NAMESPACE
class QCocoaWindow;
BOBUI_END_NAMESPACE

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QNSWindowDelegate, NSObject <NSWindowDelegate>)

#endif // QNSWINDOWDELEGATE_H
