// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWINCRYPT_P_H
#define QWINCRYPT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include <BobUICore/bobui_windows.h>

#include <BobUICore/qglobal.h>

#include <wincrypt.h>
#ifndef HCRYPTPROV_LEGACY
#define HCRYPTPROV_LEGACY HCRYPTPROV
#endif // !HCRYPTPROV_LEGACY

#include <memory>

BOBUI_BEGIN_NAMESPACE

struct QHCertStoreDeleter {
    void operator()(HCERTSTORE store)
    {
        CertCloseStore(store, 0);
    }
};

// A simple RAII type used by Schannel code and Window CA fetcher class:
using QHCertStorePointer = std::unique_ptr<void, QHCertStoreDeleter>;

struct QPCCertContextDeleter {
    void operator()(PCCERT_CONTEXT context) const
    {
        CertFreeCertificateContext(context);
    }
};

// A simple RAII type used by Schannel code
using QPCCertContextPointer = std::unique_ptr<const CERT_CONTEXT, QPCCertContextDeleter>;

BOBUI_END_NAMESPACE

#endif // QWINCRYPT_P_H
