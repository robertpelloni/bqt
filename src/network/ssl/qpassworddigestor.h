// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPASSWORDDIGESTOR_H
#define QPASSWORDDIGESTOR_H

#if 0
#pragma bobui_class(QPasswordDigestor)
#endif

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/QByteArray>
#include <BobUICore/QCryptographicHash>

BOBUI_BEGIN_NAMESPACE

namespace QPasswordDigestor {
Q_NETWORK_EXPORT QByteArray deriveKeyPbkdf1(QCryptographicHash::Algorithm algorithm,
                                   const QByteArray &password, const QByteArray &salt,
                                   int iterations, quint64 dkLen);
Q_NETWORK_EXPORT QByteArray deriveKeyPbkdf2(QCryptographicHash::Algorithm algorithm,
                                   const QByteArray &password, const QByteArray &salt,
                                   int iterations, quint64 dkLen);
} // namespace QPasswordDigestor

BOBUI_END_NAMESPACE

#endif // QPASSWORDDIGESTOR_H
