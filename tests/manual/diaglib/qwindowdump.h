// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef _WINDOWDUMP_
#define _WINDOWDUMP_

#include <BobUICore/qnamespace.h>

BOBUI_FORWARD_DECLARE_CLASS(QRect)
BOBUI_FORWARD_DECLARE_CLASS(QObject)
BOBUI_FORWARD_DECLARE_CLASS(QWindow)
BOBUI_FORWARD_DECLARE_CLASS(BOBUIextStream)

namespace BobUIDiag {

enum FormatWindowOption {
    DontPrintWindowFlags = 0x001,
    PrintSizeConstraints = 0x002
};

Q_DECLARE_FLAGS(FormatWindowOptions, FormatWindowOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(FormatWindowOptions)

void indentStream(BOBUIextStream &s, int indent);
void formatObject(BOBUIextStream &str, const QObject *o);
void formatRect(BOBUIextStream &str, const QRect &geom);
void formatWindowFlags(BOBUIextStream &str, BobUI::WindowFlags flags);

void formatWindow(BOBUIextStream &str, const QWindow *w, FormatWindowOptions options = {});
void dumpAllWindows(FormatWindowOptions options = {});

} // namespace BobUIDiag

#endif
