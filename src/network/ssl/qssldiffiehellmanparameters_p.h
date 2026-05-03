// Copyright (C) 2015 Mikkel Krautz <mikkel@krautz.dk>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default


#ifndef QSSLDIFFIEHELLMANPARAMETERS_P_H
#define QSSLDIFFIEHELLMANPARAMETERS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qssldiffiehellmanparameters.cpp.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include "qssldiffiehellmanparameters.h"

#include <QSharedData>

BOBUI_BEGIN_NAMESPACE

class QSslDiffieHellmanParametersPrivate : public QSharedData
{
public:
    void initFromDer(const QByteArray &der);
    void initFromPem(const QByteArray &pem);

    QSslDiffieHellmanParameters::Error error = QSslDiffieHellmanParameters::NoError;
    QByteArray derData;
};

BOBUI_END_NAMESPACE

#endif // QSSLDIFFIEHELLMANPARAMETERS_P_H
