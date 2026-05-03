// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QKTXHANDLER_H
#define QKTXHANDLER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "bobuiexturefilehandler_p.h"

#include <optional>

BOBUI_BEGIN_NAMESPACE

struct KTXHeader;

class QKtxHandler : public BOBUIextureFileHandler
{
public:
    using BOBUIextureFileHandler::BOBUIextureFileHandler;
    ~QKtxHandler() override;

    static bool canRead(const QByteArray &suffix, const QByteArray &block);

    BOBUIextureFileData read() override;

private:
    bool checkHeader(const KTXHeader &header);
    std::optional<QMap<QByteArray, QByteArray>> decodeKeyValues(QByteArrayView view) const;
    quint32 decode(quint32 val) const;

    bool inverseEndian = false;
};

BOBUI_END_NAMESPACE

#endif // QKTXHANDLER_H
