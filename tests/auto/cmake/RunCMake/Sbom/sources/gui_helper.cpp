// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/BobUIGlobal>

int core_helper_func();

// Exported function needed to create .lib on Windows.
Q_DECL_EXPORT int gui_helper_func() { return core_helper_func() + 1; }
