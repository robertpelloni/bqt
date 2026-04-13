// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIAGRIDITEMPROVIDER_H
#define QWINDOWSUIAGRIDITEMPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

// Implements the Grid Item control pattern provider. Used by items within a table/tree.
class QWindowsUiaGridItemProvider : public QWindowsUiaBaseProvider,
                                    public QComObject<IGridItemProvider>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaGridItemProvider)
public:
    explicit QWindowsUiaGridItemProvider(QAccessible::Id id);
    virtual ~QWindowsUiaGridItemProvider();

    // IGridItemProvider
    HRESULT STDMETHODCALLTYPE get_Row(int *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_Column(int *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_RowSpan(int *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_ColumnSpan(int *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_ContainingGrid(IRawElementProviderSimple **pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIAGRIDITEMPROVIDER_H
