// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwindowsgdinativeinterface.h"
#include "qwindowsbackingstore.h"

#include <BobUIGui/qbackingstore.h>

BOBUI_BEGIN_NAMESPACE

void *QWindowsGdiNativeInterface::nativeResourceForBackingStore(const QByteArray &resource, QBackingStore *bs)
{
    if (!bs || !bs->handle()) {
        qWarning("%s: '%s' requested for null backingstore or backingstore without handle.", __FUNCTION__, resource.constData());
        return nullptr;
    }
    auto *wbs = static_cast<QWindowsBackingStore *>(bs->handle());
    if (resource == "getDC")
        return wbs->getDC();
    qWarning("%s: Invalid key '%s' requested.", __FUNCTION__, resource.constData());
    return nullptr;
}

BOBUI_END_NAMESPACE
