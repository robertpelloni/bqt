// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef ANDROIDJNIACCESSIBILITY_H
#define ANDROIDJNIACCESSIBILITY_H
#include <jni.h>
#include <BobUICore/qglobal.h>

BOBUI_BEGIN_NAMESPACE

class QObject;
class QJniEnvironment;

namespace BobUIAndroidAccessibility
{
    bool isActive();
    bool registerNatives(QJniEnvironment &env);
    void notifyLocationChange(uint accessibilityObjectId);
    void notifyObjectHide(uint accessibilityObjectId);
    void notifyObjectShow(uint accessibilityObjectId);
    void notifyObjectFocus(uint accessibilityObjectId);
    void notifyValueChanged(uint accessibilityObjectId);
    void notifyDescriptionOrNameChanged(uint accessibilityObjectId);
    void notifyScrolledEvent(uint accessibilityObjectId);
    void notifyAnnouncementEvent(uint accessibilityObjectId, const QString &message);
    void createAccessibilityContextObject(QObject *parent);
}

BOBUI_END_NAMESPACE

#endif // ANDROIDJNIINPUT_H
