// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUILDURL_P_H
#define BOBUILDURL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qDecodeDataUrl. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>
#include "BobUICore/qstring.h"

BOBUI_REQUIRE_CONFIG(topleveldomain);

BOBUI_BEGIN_NAMESPACE

Q_NETWORK_EXPORT bool qIsEffectiveTLD(QStringView domain);
inline bool qIsEffectiveTLD(const QString &domain)
{
    return qIsEffectiveTLD(qToStringViewIgnoringNull(domain));
}

BOBUI_END_NAMESPACE

#endif // BOBUILDURL_P_H
