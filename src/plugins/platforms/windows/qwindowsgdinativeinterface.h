// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSGDINATIVEINTERFACE_H
#define QWINDOWSGDINATIVEINTERFACE_H

#include "qwindowsnativeinterface.h"

BOBUI_BEGIN_NAMESPACE

class QWindowsGdiNativeInterface : public QWindowsNativeInterface
{
    Q_OBJECT
public:
    void *nativeResourceForBackingStore(const QByteArray &resource, QBackingStore *bs) override;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSGDINATIVEINTERFACE_H
