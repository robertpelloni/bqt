// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACCESSIBLEBRIDGEUTILS_H
#define QACCESSIBLEBRIDGEUTILS_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>

#include <BobUICore/qstring.h>
#include <BobUICore/qstringlist.h>
#include <BobUIGui/qaccessible.h>

BOBUI_REQUIRE_CONFIG(accessibility);

BOBUI_BEGIN_NAMESPACE

namespace QAccessibleBridgeUtils {
    Q_GUI_EXPORT QStringList effectiveActionNames(QAccessibleInterface *iface);
    Q_GUI_EXPORT bool performEffectiveAction(QAccessibleInterface *iface, const QString &actionName);
    Q_GUI_EXPORT QString accessibleId(QAccessibleInterface *accessible);
}

BOBUI_END_NAMESPACE

#endif //QACCESSIBLEBRIDGEUTILS_H
