// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
package org.bobuiproject.bobui.android;

@UsedFromNativeCode
interface BobUIAccessibilityInterface {
    default void notifyLocationChange(int viewId) { }
    default void notifyObjectHide(int viewId, int parentId) { }
    default void notifyObjectFocus(int viewId) { }
    default void notifyScrolledEvent(int viewId) { }
    default void notifyValueChanged(int viewId, String value) { }
    default void notifyDescriptionOrNameChanged(int viewId, String value) { }
    default void notifyObjectShow(int parentId) { }
    default void notifyAnnouncementEvent(int viewId, String message) { }
}
