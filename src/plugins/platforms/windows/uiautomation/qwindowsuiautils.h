// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIAUTILS_H
#define QWINDOWSUIAUTILS_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include <BobUICore/qstring.h>
#include <BobUICore/bobui_windows.h>
#include <BobUIGui/qaccessible.h>
#include <BobUIGui/qwindow.h>
#include <BobUICore/qrect.h>
#include <BobUICore/private/qbstr_p.h>
#include "qwindowsuiautomation.h"

BOBUI_BEGIN_NAMESPACE

namespace QWindowsUiAutomation {

QWindow *windowForAccessible(const QAccessibleInterface *accessible);

HWND hwndForAccessible(const QAccessibleInterface *accessible);

void rectToNativeUiaRect(const QRect &rect, const QWindow *w, UiaRect *uiaRect);

void nativeUiaPointToPoint(const UiaPoint &uiaPoint, const QWindow *w, QPoint *point);

long roleToControlTypeId(QAccessible::Role role);

bool isTextUnitSeparator(TextUnit unit, const QChar &ch);

void clearVariant(VARIANT *variant);

} // namespace QWindowsUiAutomation

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIAUTILS_H
