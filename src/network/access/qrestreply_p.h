// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QRESTREPLY_P_H
#define QRESTREPLY_P_H

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

#include <BobUICore/private/qstringconverter_p.h>

#include <optional>

BOBUI_BEGIN_NAMESPACE

class QByteArray;
class QNetworkReply;

class QRestReplyPrivate
{
public:
    QRestReplyPrivate();
    ~QRestReplyPrivate();

    std::optional<QStringDecoder> decoder;

    static QByteArray contentCharset(const QNetworkReply *reply);
};

BOBUI_END_NAMESPACE

#endif
