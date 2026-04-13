// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSDROPDATAOBJECT_H
#define QWINDOWSDROPDATAOBJECT_H

#include "qwindowsole.h"

BOBUI_BEGIN_NAMESPACE

class QWindowsDropDataObject : public QWindowsOleDataObject
{
public:
    explicit QWindowsDropDataObject(QMimeData *mimeData);
    ~QWindowsDropDataObject() override;

    // overridden IDataObject methods
    STDMETHOD(GetData)(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium) noexcept override;
    STDMETHOD(QueryGetData)(LPFORMATETC pformatetc) noexcept override;

private:
    bool shouldIgnore(LPFORMATETC pformatetc) const;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSDROPDATAOBJECT_H
