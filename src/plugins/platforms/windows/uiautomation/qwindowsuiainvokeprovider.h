// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIAINVOKEPROVIDER_H
#define QWINDOWSUIAINVOKEPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

// Implements the Invoke control pattern provider.
class QWindowsUiaInvokeProvider : public QWindowsUiaBaseProvider, public QComObject<IInvokeProvider>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaInvokeProvider)
public:
    explicit QWindowsUiaInvokeProvider(QAccessible::Id id);
    virtual ~QWindowsUiaInvokeProvider();

    // IInvokeProvider
    HRESULT STDMETHODCALLTYPE Invoke() override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIAINVOKEPROVIDER_H
