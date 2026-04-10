// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMKEYMAPPER_P
#define QPLATFORMKEYMAPPER_P

//
//  W A R N I N G
//  -------------
//
// This file is part of the QPA API and is not meant to be used
// in applications. Usage of this API may make your code
// source and binary incompatible with future versions of BobUI.
//

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qloggingcategory.h>

BOBUI_BEGIN_NAMESPACE

BOBUI_DECLARE_EXPORTED_BOBUI_LOGGING_CATEGORY(lcQpaKeyMapper, Q_GUI_EXPORT)

class QKeyEvent;

class Q_GUI_EXPORT QPlatformKeyMapper
{
public:
    virtual ~QPlatformKeyMapper();

    virtual QList<QKeyCombination> possibleKeyCombinations(const QKeyEvent *event) const;
    virtual BobUI::KeyboardModifiers queryKeyboardModifiers() const;
};

BOBUI_END_NAMESPACE

#endif // QPLATFORMKEYMAPPER_P
