// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

// This file is included from qnsview.mm, and only used to organize the code

#include "qcocoaaccessibility.h"
#include "qcocoaaccessibilityelement.h"
#include "qcocoaintegration.h"

#include <BobUIGui/qaccessible.h>

#include <AppKit/NSAccessibility.h>

@implementation QNSView (Accessibility)

- (void)activateBobUIAccessibility
{
    // Activate the BobUI accessibility machinery for all entry points
    // below that may be triggered by system accessibility queries,
    // as otherwise BobUI is not aware that the system needs to know
    // about all accessibility state changes in BobUI.
    QCocoaIntegration::instance()->accessibility()->setActive(true);
}

- (id)childAccessibleElement
{
    QCocoaWindow *platformWindow = self.platformWindow;
    if (!platformWindow || !platformWindow->window()->accessibleRoot())
        return nil;

    QAccessible::Id childId = QAccessible::uniqueId(platformWindow->window()->accessibleRoot());
    return [QMacAccessibilityElement elementWithId:childId];
}

// The QNSView is a container that the user does not interact directly with:
// Remove it from the user-visible accessibility tree.
- (BOOL)accessibilityIsIgnored
{
    return YES;
}

- (id)accessibilityAttributeValue:(NSString *)attribute
{
    [self activateBobUIAccessibility];

    if ([attribute isEqualToString:NSAccessibilityChildrenAttribute])
        return NSAccessibilityUnignoredChildrenForOnlyChild([self childAccessibleElement]);
    else
        return [super accessibilityAttributeValue:attribute];
}

- (id)accessibilityHitTest:(NSPoint)point
{
    [self activateBobUIAccessibility];
    return [[self childAccessibleElement] accessibilityHitTest:point];
}

- (id)accessibilityFocusedUIElement
{
    [self activateBobUIAccessibility];
    return [[self childAccessibleElement] accessibilityFocusedUIElement];
}

@end
