// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIATOGGLEPROVIDER_H
#define QWINDOWSUIATOGGLEPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

// Implements the Toggle control pattern provider. Used for checkboxes.
class QWindowsUiaToggleProvider : public QWindowsUiaBaseProvider, public QComObject<IToggleProvider>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaToggleProvider)
public:
    explicit QWindowsUiaToggleProvider(QAccessible::Id id);
    virtual ~QWindowsUiaToggleProvider();

    // IToggleProvider
    HRESULT STDMETHODCALLTYPE Toggle() override;
    HRESULT STDMETHODCALLTYPE get_ToggleState(ToggleState *pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIATOGGLEPROVIDER_H
