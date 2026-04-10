// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOAACCESIBILITY_H
#define QCOCOAACCESIBILITY_H

#include <BobUIGui/bobuiguiglobal.h>

#if BOBUI_CONFIG(accessibility)

#include <qpa/qplatformaccessibility.h>

#include "qcocoaaccessibilityelement.h"

#include <functional>

BOBUI_BEGIN_NAMESPACE

class QCocoaAccessibility : public QPlatformAccessibility
{
public:
    QCocoaAccessibility();
    ~QCocoaAccessibility();
    void notifyAccessibilityUpdate(QAccessibleEvent *event) override;
    void setRootObject(QObject *o) override;
    void initialize() override;
    void cleanup() override;
};

namespace QCocoaAccessible {

/*
    BobUI Cocoa Accessibility Overview

    Cocoa accessibility is implemented in the following files:

    - qcocoaaccessibility (this file) : QCocoaAccessibility "plugin", conversion and helper functions.
    - qnsviewaccessibility            : Root accessibility implementation for QNSView
    - qcocoaaccessibilityelement      : Cocoa accessibility protocol wrapper for QAccessibleInterface

    The accessibility implementation wraps QAccessibleInterfaces in QCocoaAccessibleElements, which
    implements the cocoa accessibility protocol. The root QAccessibleInterface (the one returned
    by QWindow::accessibleRoot), is anchored to the QNSView in qnsviewaccessibility.mm.

    Cocoa explores the accessibility tree by walking the tree using the parent/child
    relationships or hit testing. When this happens we create QCocoaAccessibleElements on
    demand.
*/

#if defined(__OBJC__)
NSString *macRole(QAccessibleInterface *interface);
NSString *macSubrole(QAccessibleInterface *interface);
bool shouldBeIgnored(QAccessibleInterface *interface);
bool defaultUnignored(QAccessibleInterface *interface);
NSArray<QMacAccessibilityElement *> *unignoredChildren(QAccessibleInterface *interface,
                        const std::function<bool(QAccessibleInterface *)> &p = defaultUnignored);
NSString *getTranslatedAction(const QString &bobuiAction);
QString translateAction(NSString *nsAction, QAccessibleInterface *interface);
bool hasValueAttribute(QAccessibleInterface *interface);
id getValueAttribute(QAccessibleInterface *interface);
#endif // __OBJC__

}

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QCOCOAACCESIBILITY_H
