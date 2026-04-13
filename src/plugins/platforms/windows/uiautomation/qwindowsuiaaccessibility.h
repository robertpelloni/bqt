// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIAACCESSIBILITY_H
#define QWINDOWSUIAACCESSIBILITY_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include <BobUICore/bobui_windows.h>
#include "qwindowscontext.h"
#include <qpa/qplatformaccessibility.h>

BOBUI_BEGIN_NAMESPACE

// Windows platform accessibility implemented over UI Automation.
class QWindowsUiaAccessibility : public QPlatformAccessibility
{
public:
    explicit QWindowsUiaAccessibility();
    virtual ~QWindowsUiaAccessibility();
    static bool handleWmGetObject(HWND hwnd, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
    void notifyAccessibilityUpdate(QAccessibleEvent *event) override;
private:
    static bool m_accessibleActive;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIAACCESSIBILITY_H
