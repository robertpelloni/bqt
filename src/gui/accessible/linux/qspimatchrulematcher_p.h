// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSPIMATCHRULEMATCHER_H
#define QSPIMATCHRULEMATCHER_H

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

#include "qspi_struct_marshallers_p.h"

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUIGui/qaccessible.h>

#include <atspi/atspi-constants.h>

BOBUI_REQUIRE_CONFIG(accessibility);

BOBUI_BEGIN_NAMESPACE

class QSpiMatchRuleMatcher
{
    quint64 m_states;
    int m_stateMatchType;
    QSpiAttributeSet m_attributes;
    int m_attributeMatchType;
    std::unordered_set<AtspiRole> m_roles;
    int m_roleMatchType;
    QStringList m_interfaces;
    int m_interfaceMatchType;

    bool matchAttributes(QAccessibleInterface &iface) const;
    bool matchInterfaces(QAccessibleInterface &iface) const;
    bool matchRoles(QAccessibleInterface &iface) const;
    bool matchStates(QAccessibleInterface &iface) const;

public:
    QSpiMatchRuleMatcher(const QSpiMatchRule &matchRule);

    bool match(QAccessibleInterface &iface) const;
};

BOBUI_END_NAMESPACE

#endif
