// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIABASEPROVIDER_H
#define QWINDOWSUIABASEPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include <BobUIGui/qaccessible.h>
#include <BobUICore/qpointer.h>

#include "qwindowsuiautomation.h"
#include <BobUICore/private/qcomobject_p.h>

BOBUI_BEGIN_NAMESPACE

class QAccessibleInterface;

// Base class for UI Automation providers.
class QWindowsUiaBaseProvider : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QWindowsUiaBaseProvider)
public:
    explicit QWindowsUiaBaseProvider(QAccessible::Id id);
    virtual ~QWindowsUiaBaseProvider();

    QAccessibleInterface *accessibleInterface() const;
    QAccessible::Id id() const;

private:
    QAccessible::Id m_id;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIABASEPROVIDER_H
