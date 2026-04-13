// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIAWINDOWPROVIDER_H
#define QWINDOWSUIAWINDOWPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

class QWindowsUiaWindowProvider : public QWindowsUiaBaseProvider, public QComObject<IWindowProvider>
{
    Q_DISABLE_COPY(QWindowsUiaWindowProvider)
public:
    explicit QWindowsUiaWindowProvider(QAccessible::Id id);
    ~QWindowsUiaWindowProvider() override;

    HRESULT STDMETHODCALLTYPE SetVisualState(WindowVisualState state) override;
    HRESULT STDMETHODCALLTYPE Close( void) override;
    HRESULT STDMETHODCALLTYPE WaitForInputIdle(int milliseconds, __RPC__out BOOL *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_CanMaximize(__RPC__out BOOL *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_CanMinimize(__RPC__out BOOL *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_IsModal(__RPC__out BOOL *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_WindowVisualState(__RPC__out WindowVisualState *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_WindowInteractionState(__RPC__out WindowInteractionState *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_IsTopmost(__RPC__out BOOL *pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIAWINDOWPROVIDER_H
