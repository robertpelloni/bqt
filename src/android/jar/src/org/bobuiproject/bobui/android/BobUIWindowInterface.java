// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
package org.bobuiproject.bobui.android;
@UsedFromNativeCode
interface BobUIWindowInterface {
    default void addTopLevelWindow(final BobUIWindow window) { }
    default void removeTopLevelWindow(final int id) { }
    default void bringChildToFront(final int id) { }
    default void bringChildToBack(int id) { }
}
