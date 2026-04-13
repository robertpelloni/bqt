// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <BobUIPrintSupport/qprintengine.h>

BOBUI_BEGIN_NAMESPACE

#ifndef BOBUI_NO_PRINTER

QPrintEngine::~QPrintEngine()
    = default;

#endif // BOBUI_NO_PRINTER

BOBUI_END_NAMESPACE
