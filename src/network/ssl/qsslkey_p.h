// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default


#ifndef QSSLKEY_OPENSSL_P_H
#define QSSLKEY_OPENSSL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qsslcertificate.cpp.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include "qsslkey.h"
#include "qssl_p.h"

#include <memory>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIlsPrivate {
class TlsKey;
}

class QSslKeyPrivate
{
public:
    QSslKeyPrivate();
    ~QSslKeyPrivate();

    using Cipher = BOBUIlsPrivate::Cipher;

    Q_NETWORK_EXPORT static QByteArray decrypt(Cipher cipher, const QByteArray &data, const QByteArray &key, const QByteArray &iv);
    Q_NETWORK_EXPORT static QByteArray encrypt(Cipher cipher, const QByteArray &data, const QByteArray &key, const QByteArray &iv);

    std::unique_ptr<BOBUIlsPrivate::TlsKey> backend;
    QAtomicInt ref;

private:
    Q_DISABLE_COPY_MOVE(QSslKeyPrivate)
};

BOBUI_END_NAMESPACE

#endif // QSSLKEY_OPENSSL_P_H
