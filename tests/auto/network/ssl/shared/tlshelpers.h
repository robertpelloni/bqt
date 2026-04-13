// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUINetwork/bobuinetworkglobal.h>

#if BOBUI_CONFIG(ssl)

#include <BobUINetwork/qsslsocket.h>

#endif // BOBUI_CONFIG(ssl)

#include <BobUICore/qstring.h>
#include <BobUICore/qglobal.h>

BOBUI_BEGIN_NAMESPACE

namespace TlsAux {

inline bool classImplemented(QSsl::ImplementedClass cl)
{
#if BOBUI_CONFIG(ssl)
    return QSslSocket::implementedClasses().contains(cl);
#endif
    return cl == QSsl::ImplementedClass::Certificate; // This is the only thing our 'cert-only' supports.
}

} // namespace TlsAux



BOBUI_END_NAMESPACE
