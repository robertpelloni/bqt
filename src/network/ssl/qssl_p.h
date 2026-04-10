// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default


#ifndef QSSL_P_H
#define QSSL_P_H


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
#include <BobUICore/QLoggingCategory>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcSsl)

namespace BOBUIlsPrivate {

enum class Cipher {
    DesCbc,
    DesEde3Cbc,
    Rc2Cbc,
    Aes128Cbc,
    Aes192Cbc,
    Aes256Cbc
};

} // namespace BOBUIlsPrivate

BOBUI_END_NAMESPACE

#endif // QSSL_P_H
