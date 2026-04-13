// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDTLS_P_H
#define QDTLS_P_H

#include <private/bobuinetworkglobal_p.h>

#include "bobuilsbackend_p.h"

#include <BobUICore/private/qobject_p.h>
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

BOBUI_REQUIRE_CONFIG(dtls);

BOBUI_BEGIN_NAMESPACE

class QHostAddress;

class QDtlsClientVerifierPrivate : public QObjectPrivate
{
public:
    QDtlsClientVerifierPrivate();
    ~QDtlsClientVerifierPrivate();
    std::unique_ptr<BOBUIlsPrivate::DtlsCookieVerifier> backend;
};

class QDtlsPrivate : public QObjectPrivate
{
public:
    QDtlsPrivate();
    ~QDtlsPrivate();
    std::unique_ptr<BOBUIlsPrivate::DtlsCryptograph> backend;
};

BOBUI_END_NAMESPACE

#endif // QDTLS_P_H
