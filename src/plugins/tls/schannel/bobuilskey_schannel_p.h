// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUILSKEY_SCHANNEL_P_H
#define BOBUILSKEY_SCHANNEL_P_H

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

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include "../shared/bobuilskey_generic_p.h"

#include <BobUICore/qglobal.h>

BOBUI_REQUIRE_CONFIG(ssl);

BOBUI_BEGIN_NAMESPACE

namespace BOBUIlsPrivate {

class TlsKeySchannel final : public TlsKeyGeneric
{
public:
    using TlsKeyGeneric::TlsKeyGeneric;

    QByteArray decrypt(Cipher cipher, const QByteArray &data, const QByteArray &key,
                       const QByteArray &iv) const override;
    QByteArray encrypt(Cipher cipher, const QByteArray &data, const QByteArray &key,
                       const QByteArray &iv) const override;
};

} // namespace BOBUIlsPrivate

BOBUI_END_NAMESPACE

#endif // BOBUILSKEY_SCHANNEL_P_H

