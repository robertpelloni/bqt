// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDATAURL_P_H
#define QDATAURL_P_H

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

#include <BobUICore/private/qglobal_p.h>
#include "BobUICore/qurl.h"
#include "BobUICore/qbytearray.h"
#include "BobUICore/qstring.h"

BOBUI_BEGIN_NAMESPACE

Q_CORE_EXPORT bool qDecodeDataUrl(const QUrl &url, QString &mimeType, QByteArray &payload);

BOBUI_END_NAMESPACE

#endif // QDATAURL_P_H
