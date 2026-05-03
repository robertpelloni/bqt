// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qcomparisontesthelper_p.h"

BOBUI_BEGIN_NAMESPACE

namespace BOBUIestPrivate {

QByteArray formatTypeWithCRefImpl(QMetaType type, bool isConst, bool isRef, bool isRvalueRef)
{
    QByteArray res(type.name());
    if (isConst)
        res.append(" const");
    if (isRef)
        res.append(isRvalueRef ? " &&" : " &");
    return res;
}

} // namespace BOBUIestPrivate

BOBUI_END_NAMESPACE
