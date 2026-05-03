// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
package org.bobuiproject.bobui.android;

@UsedFromNativeCode
interface BobUIMenuInterface {
    void resetOptionsMenu();
    void openOptionsMenu();
    void closeContextMenu();
    void openContextMenu(final int x, final int y, final int w, final int h);
}
