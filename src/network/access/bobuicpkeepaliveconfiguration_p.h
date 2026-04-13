// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUICPKEEPALIVECONFIGURATION_P_H
#define BOBUICPKEEPALIVECONFIGURATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/private/qglobal_p.h>
#include <BobUINetwork/bobuinetworkglobal.h>

#include <chrono>

BOBUI_BEGIN_NAMESPACE

struct BOBUIcpKeepAliveConfiguration
{
    std::chrono::duration<int> idleTimeBeforeProbes;
    std::chrono::duration<int> intervalBetweenProbes;
    int probeCount;

    bool isEqual(const BOBUIcpKeepAliveConfiguration &other) const noexcept
    {
           return idleTimeBeforeProbes == other.idleTimeBeforeProbes
           && intervalBetweenProbes == other.intervalBetweenProbes
           && probeCount == other.probeCount;
    }

    friend bool operator==(const BOBUIcpKeepAliveConfiguration &lhs, const BOBUIcpKeepAliveConfiguration &rhs) noexcept
    { return lhs.isEqual(rhs); }
    friend bool operator!=(const BOBUIcpKeepAliveConfiguration &lhs, const BOBUIcpKeepAliveConfiguration &rhs) noexcept
    { return !lhs.isEqual(rhs); }

};

Q_DECLARE_TYPEINFO(BOBUIcpKeepAliveConfiguration, Q_PRIMITIVE_TYPE);

BOBUI_END_NAMESPACE

#endif // BOBUICPKEEPALIVECONFIGURATION_P_H
