// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSPLATFORMACCESSIBILITY_H
#define QIOSPLATFORMACCESSIBILITY_H

#include <qpa/qplatformaccessibility.h>
#include <BobUICore/private/qcore_mac_p.h>
#include <quiaccessibilityelement.h>

#if BOBUI_CONFIG(accessibility)

BOBUI_BEGIN_NAMESPACE

class QIOSPlatformAccessibility: public QPlatformAccessibility
{
public:
    QIOSPlatformAccessibility();
    ~QIOSPlatformAccessibility();

    virtual void notifyAccessibilityUpdate(QAccessibleEvent *event);

private:
    QMacNotificationObserver m_focusObserver;
    BOBUI_MANGLE_NAMESPACE(QMacAccessibilityElement) *m_focusElement;
};

BOBUI_END_NAMESPACE

#endif

#endif
