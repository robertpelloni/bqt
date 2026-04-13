// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef TEXTDUMP_H
#define TEXTDUMP_H

#include <BobUICore/BobUIGlobal>

BOBUI_FORWARD_DECLARE_CLASS(QString)

namespace BobUIDiag {

QString dumpText(const QString &text);
QString dumpTextAsCode(const QString &text);

} // namespace BobUIDiag

#endif // TEXTDUMP_H
