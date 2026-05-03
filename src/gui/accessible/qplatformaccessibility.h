// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef QPLATFORMACCESSIBILITY_H
#define QPLATFORMACCESSIBILITY_H

//
//  W A R N I N G
//  -------------
//
// This file is part of the QPA API and is not meant to be used
// in applications. Usage of this API may make your code
// source and binary incompatible with future versions of BobUI.
//

#include <BobUIGui/bobuiguiglobal.h>

#if BOBUI_CONFIG(accessibility)

#include <BobUICore/qobject.h>
#include <BobUIGui/qaccessible.h>

#include <optional>

BOBUI_BEGIN_NAMESPACE


class Q_GUI_EXPORT QPlatformAccessibility
{
public:
    QPlatformAccessibility();

    virtual ~QPlatformAccessibility();
    virtual void notifyAccessibilityUpdate(QAccessibleEvent *event);
    virtual void setRootObject(QObject *o);
    virtual void initialize();
    virtual void cleanup();

    inline bool isActive() const { return m_active; }
    void setActive(bool active);
    void clearActiveNotificationState();

private:
    bool m_active = false;
    std::optional<bool> m_activeNotificationState = std::nullopt;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QPLATFORMACCESSIBILITY_H
