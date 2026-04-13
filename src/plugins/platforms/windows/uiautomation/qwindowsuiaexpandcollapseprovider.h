// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIAEXPANDCOLLAPSEPROVIDER_H
#define QWINDOWSUIAEXPANDCOLLAPSEPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

// Implements the Expand/Collapse control pattern provider. Used for menu items with submenus.
class QWindowsUiaExpandCollapseProvider : public QWindowsUiaBaseProvider,
                                          public QComObject<IExpandCollapseProvider>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaExpandCollapseProvider)
public:
    explicit QWindowsUiaExpandCollapseProvider(QAccessible::Id id);
    virtual ~QWindowsUiaExpandCollapseProvider() override;

    // IExpandCollapseProvider
    HRESULT STDMETHODCALLTYPE Expand() override;
    HRESULT STDMETHODCALLTYPE Collapse() override;
    HRESULT STDMETHODCALLTYPE get_ExpandCollapseState(__RPC__out ExpandCollapseState *pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIAEXPANDCOLLAPSEPROVIDER_H
