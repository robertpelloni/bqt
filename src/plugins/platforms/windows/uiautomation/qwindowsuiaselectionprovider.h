// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIASELECTIONPROVIDER_H
#define QWINDOWSUIASELECTIONPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIPrivate {

template <>
struct QComObjectTraits<ISelectionProvider2>
{
    static constexpr bool isGuidOf(REFIID riid) noexcept
    {
        return QComObjectTraits<ISelectionProvider2, ISelectionProvider>::isGuidOf(riid);
    }
};

} // namespace BobUIPrivate

// Implements the Selection control pattern provider. Used for Lists.
class QWindowsUiaSelectionProvider : public QWindowsUiaBaseProvider,
                                     public QComObject<ISelectionProvider2>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaSelectionProvider)
public:
    explicit QWindowsUiaSelectionProvider(QAccessible::Id id);
    virtual ~QWindowsUiaSelectionProvider();

    // ISelectionProvider
    HRESULT STDMETHODCALLTYPE GetSelection(SAFEARRAY **pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_CanSelectMultiple(BOOL *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_IsSelectionRequired(BOOL *pRetVal) override;

    // ISelectionProvider2
    HRESULT STDMETHODCALLTYPE get_FirstSelectedItem(__RPC__deref_out_opt IRawElementProviderSimple **pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_LastSelectedItem(__RPC__deref_out_opt IRawElementProviderSimple **pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_CurrentSelectedItem(__RPC__deref_out_opt IRawElementProviderSimple **pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_ItemCount(__RPC__out int *pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIASELECTIONPROVIDER_H
