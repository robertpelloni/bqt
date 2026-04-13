// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

/**
 * BobUIEmbeddedViewInterface is intended to encapsulate the needs of BobUIView, so that the Activity and
 * Service implementations of these functions may be split clearly, and the interface can be stored
 * and used conveniently in BobUIView.
**/
interface BobUIEmbeddedViewInterface {
    void startBobUIApplication(String appParams, String mainLib);
    void addView(BobUIView view);
    void removeView(BobUIView view);
}
