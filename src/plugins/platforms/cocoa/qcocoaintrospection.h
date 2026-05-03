// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (c) 2007-2008, Apple, Inc.
// SPDX-License-Identifier: BSD-3-Clause
// BobUI-Security score:significant reason:default

#ifndef QCOCOAINTROSPECTION_H
#define QCOCOAINTROSPECTION_H

#include <qglobal.h>
#include <objc/objc-class.h>

BOBUI_BEGIN_NAMESPACE

void bobui_cocoa_change_implementation(Class baseClass, SEL originalSel, Class proxyClass, SEL replacementSel = nil, SEL backupSel = nil);
void bobui_cocoa_change_back_implementation(Class baseClass, SEL originalSel, SEL backupSel);

BOBUI_END_NAMESPACE

#endif // QCOCOAINTROSPECTION_H
