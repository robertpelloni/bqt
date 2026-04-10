// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIARANGEVALUEPROVIDER_H
#define QWINDOWSUIARANGEVALUEPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

// Implements the Range Value control pattern provider.
class QWindowsUiaRangeValueProvider : public QWindowsUiaBaseProvider,
                                      public QComObject<IRangeValueProvider>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaRangeValueProvider)
public:
    explicit QWindowsUiaRangeValueProvider(QAccessible::Id id);
    virtual ~QWindowsUiaRangeValueProvider();

    // IRangeValueProvider
    HRESULT STDMETHODCALLTYPE SetValue(double val) override;
    HRESULT STDMETHODCALLTYPE get_Value(double *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_IsReadOnly(BOOL *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_Maximum(double *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_Minimum(double *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_LargeChange(double *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_SmallChange(double *pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIARANGEVALUEPROVIDER_H
