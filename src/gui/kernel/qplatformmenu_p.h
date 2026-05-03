// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMMENU_P_H
#define QPLATFORMMENU_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>

#include <BobUICore/qnativeinterface.h>

BOBUI_BEGIN_NAMESPACE

// ----------------- QNativeInterface -----------------

#if !defined(Q_OS_MACOS) && defined(Q_QDOC)
typedef void NSMenu;
#else
BOBUI_END_NAMESPACE
Q_FORWARD_DECLARE_OBJC_CLASS(NSMenu);
BOBUI_BEGIN_NAMESPACE
#endif

namespace QNativeInterface::Private {

#if defined(Q_OS_MACOS) || defined(Q_QDOC)
struct Q_GUI_EXPORT QCocoaMenu
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QCocoaMenu)
    virtual NSMenu *nsMenu() const = 0;
    virtual void setAsDockMenu() const = 0;
};

struct Q_GUI_EXPORT QCocoaMenuBar
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QCocoaMenuBar)
    virtual NSMenu *nsMenu() const = 0;
};
#endif

} // QNativeInterface::Private

BOBUI_END_NAMESPACE

#endif // QPLATFORMMENU_P_H
