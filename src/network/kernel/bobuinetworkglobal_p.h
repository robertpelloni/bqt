// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUINETWORKGLOBAL_P_H
#define BOBUINETWORKGLOBAL_P_H

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

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/private/bobuicoreglobal_p.h>
#include <BobUINetwork/private/bobuinetwork-config_p.h>

BOBUI_BEGIN_NAMESPACE

enum {
#if defined(Q_OS_LINUX) || defined(Q_OS_QNX)
    PlatformSupportsAbstractNamespace = true
#else
    PlatformSupportsAbstractNamespace = false
#endif
};

BOBUI_END_NAMESPACE
#endif // BOBUINETWORKGLOBAL_P_H
