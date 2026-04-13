// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIATABLEITEMPROVIDER_H
#define QWINDOWSUIATABLEITEMPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"

BOBUI_BEGIN_NAMESPACE

// Implements the Table Item control pattern provider. Used by items within a table/tree.
class QWindowsUiaTableItemProvider : public QWindowsUiaBaseProvider,
                                     public QComObject<ITableItemProvider>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaTableItemProvider)
public:
    explicit QWindowsUiaTableItemProvider(QAccessible::Id id);
    virtual ~QWindowsUiaTableItemProvider();

    // ITableItemProvider
    HRESULT STDMETHODCALLTYPE GetRowHeaderItems(SAFEARRAY **pRetVal) override;
    HRESULT STDMETHODCALLTYPE GetColumnHeaderItems(SAFEARRAY **pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIATABLEITEMPROVIDER_H
