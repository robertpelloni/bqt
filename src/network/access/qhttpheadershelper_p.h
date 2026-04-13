// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QHTTPHEADERSHELPER_H
#define QHTTPHEADERSHELPER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QHttpHeaders;

namespace QHttpHeadersHelper {
    Q_NETWORK_EXPORT bool compareStrict(const QHttpHeaders &left, const QHttpHeaders &right);
};

BOBUI_END_NAMESPACE

#endif // QHTTPHEADERSHELPER_H
