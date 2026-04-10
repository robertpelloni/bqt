// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QITERABLE_IMPL_H
#define QITERABLE_IMPL_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIIterablePrivate {

template<typename Callback>
static QVariant retrieveElement(QMetaType type, Callback callback)
{
    QVariant v(type);
    void *dataPtr;
    if (type == QMetaType::fromType<QVariant>())
        dataPtr = &v;
    else
        dataPtr = v.data();
    callback(dataPtr);
    return v;
}

} // namespace BobUIIterablePrivate

BOBUI_END_NAMESPACE

#endif // QITERABLE_IMPL_H
