// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef _WIDGETDUMP_
#define _WIDGETDUMP_

#include "qwindowdump.h"

BOBUI_FORWARD_DECLARE_CLASS(QWidget)

namespace BobUIDiag {

void dumpAllWidgets(FormatWindowOptions options = {}, const QWidget *root = nullptr);

} // namespace BobUIDiag

#endif
