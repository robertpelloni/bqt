// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef _GLINFO_
#define _GLINFO_

#include <BobUICore/BobUIGlobal>

BOBUI_FORWARD_DECLARE_CLASS(QObject)
BOBUI_FORWARD_DECLARE_CLASS(QString)

namespace BobUIDiag {

QString glInfo(const QObject *o);

} // namespace BobUIDiag

#endif
