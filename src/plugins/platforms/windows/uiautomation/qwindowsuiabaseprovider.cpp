// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"
#include "qwindowsuiautils.h"
#include "qwindowscontext.h"

#include <BobUIGui/qaccessible.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE

using namespace QWindowsUiAutomation;


QWindowsUiaBaseProvider::QWindowsUiaBaseProvider(QAccessible::Id id) :
    m_id(id)
{
}

QWindowsUiaBaseProvider::~QWindowsUiaBaseProvider()
{
}

QAccessibleInterface *QWindowsUiaBaseProvider::accessibleInterface() const
{
    QAccessibleInterface *accessible = QAccessible::accessibleInterface(m_id);
    if (accessible && accessible->isValid())
        return accessible;
    return nullptr;
}

QAccessible::Id QWindowsUiaBaseProvider::id() const
{
    return m_id;
}

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)
