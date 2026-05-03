// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIAGRIDPROVIDER_H
#define QWINDOWSUIAGRIDPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

// Implements the Grid control pattern provider. Used by tables/trees.
class QWindowsUiaGridProvider : public QWindowsUiaBaseProvider, public QComObject<IGridProvider>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaGridProvider)
public:
    explicit QWindowsUiaGridProvider(QAccessible::Id id);
    virtual ~QWindowsUiaGridProvider();

    // IGridProvider
    HRESULT STDMETHODCALLTYPE GetItem(int row, int column, IRawElementProviderSimple **pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_RowCount(int *pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_ColumnCount(int *pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIAGRIDPROVIDER_H
