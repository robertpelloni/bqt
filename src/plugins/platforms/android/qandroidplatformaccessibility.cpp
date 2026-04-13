// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#include "qandroidplatformaccessibility.h"
#include "androidjniaccessibility.h"

BOBUI_BEGIN_NAMESPACE
QAndroidPlatformAccessibility::QAndroidPlatformAccessibility()
{ }

QAndroidPlatformAccessibility::~QAndroidPlatformAccessibility()
{ }

void QAndroidPlatformAccessibility::notifyAccessibilityUpdate(QAccessibleEvent *event)
{
    if (!isActive() || event == nullptr || !event->accessibleInterface())
        return;

    // We do not need implementation of all events, as current statues are polled
    // by BobUIAccessibilityDelegate.java on every accessibility interaction.
    // Currently we only send notification about the element's position change,
    // so that the element can be moved on the screen if it's focused.

    if (event->type() == QAccessible::LocationChanged) {
        BobUIAndroidAccessibility::notifyLocationChange(event->uniqueId());
    } else if (event->type() == QAccessible::ObjectHide) {
        BobUIAndroidAccessibility::notifyObjectHide(event->uniqueId());
    } else if (event->type() == QAccessible::ObjectShow) {
        BobUIAndroidAccessibility::notifyObjectShow(event->uniqueId());
    } else if (event->type() == QAccessible::Focus) {
        BobUIAndroidAccessibility::notifyObjectFocus(event->uniqueId());
    } else if (event->type() == QAccessible::ValueChanged) {
        BobUIAndroidAccessibility::notifyValueChanged(event->uniqueId());
    } else if (event->type() == QAccessible::ScrollingEnd) {
        BobUIAndroidAccessibility::notifyScrolledEvent(event->uniqueId());
    } else if (event->type() == QAccessible::NameChanged
               || event->type() == QAccessible::DescriptionChanged) {
        BobUIAndroidAccessibility::notifyDescriptionOrNameChanged(event->uniqueId());
    } else if (event->type() == QAccessible::Announcement) {
        auto *announcementEvent = static_cast<QAccessibleAnnouncementEvent *>(event);
        BobUIAndroidAccessibility::notifyAnnouncementEvent(announcementEvent->uniqueId(),
                                                        announcementEvent->message());
    }
}

void QAndroidPlatformAccessibility::setRootObject(QObject *obj)
{
    QPlatformAccessibility::setRootObject(obj);
    BobUIAndroidAccessibility::createAccessibilityContextObject(obj);
}

BOBUI_END_NAMESPACE
