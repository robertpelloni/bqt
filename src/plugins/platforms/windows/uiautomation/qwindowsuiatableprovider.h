// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIATABLEPROVIDER_H
#define QWINDOWSUIATABLEPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

// Implements the Table control pattern provider. Used by tables/trees.
class QWindowsUiaTableProvider : public QWindowsUiaBaseProvider, public QComObject<ITableProvider>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaTableProvider)
public:
    explicit QWindowsUiaTableProvider(QAccessible::Id id);
    virtual ~QWindowsUiaTableProvider();

    // ITableProvider
    HRESULT STDMETHODCALLTYPE GetRowHeaders(SAFEARRAY **pRetVal) override;
    HRESULT STDMETHODCALLTYPE GetColumnHeaders(SAFEARRAY **pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_RowOrColumnMajor(enum RowOrColumnMajor *pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIATABLEPROVIDER_H
