// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#ifndef QANDROIDPLATFORMACCESSIBILITY_H
#define QANDROIDPLATFORMACCESSIBILITY_H

#include <qpa/qplatformaccessibility.h>

BOBUI_BEGIN_NAMESPACE

class QAndroidPlatformAccessibility: public QPlatformAccessibility
{
public:
    QAndroidPlatformAccessibility();
    ~QAndroidPlatformAccessibility();

    void notifyAccessibilityUpdate(QAccessibleEvent *event) override;
    void setRootObject(QObject *obj) override;
};

BOBUI_END_NAMESPACE

#endif
