// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiainvokeprovider.h"
#include "qwindowsuiautils.h"
#include "qwindowscontext.h"

#include <BobUIGui/qaccessible.h>
#include <BobUICore/qloggingcategory.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE

using namespace QWindowsUiAutomation;


QWindowsUiaInvokeProvider::QWindowsUiaInvokeProvider(QAccessible::Id id) :
    QWindowsUiaBaseProvider(id)
{
}

QWindowsUiaInvokeProvider::~QWindowsUiaInvokeProvider()
{
}

HRESULT STDMETHODCALLTYPE QWindowsUiaInvokeProvider::Invoke()
{
    qCDebug(lcQpaUiAutomation) << __FUNCTION__;

    QAccessibleInterface *accessible = accessibleInterface();
    if (!accessible)
        return UIA_E_ELEMENTNOTAVAILABLE;

    QAccessibleActionInterface *actionInterface = accessible->actionInterface();
    if (!actionInterface)
        return UIA_E_ELEMENTNOTAVAILABLE;

    actionInterface->doAction(QAccessibleActionInterface::pressAction());
    return S_OK;
}

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)
