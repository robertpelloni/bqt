// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QUIWINDOW_H
#define QUIWINDOW_H

#include <UIKit/UIWindow.h>

@interface QUIWindow : UIWindow
@property (nonatomic, readonly) BOOL sendingEvent;
@end

#endif // QUIWINDOW_H
