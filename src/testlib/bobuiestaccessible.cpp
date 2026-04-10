// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuiestaccessible.h"

#if BOBUI_CONFIG(accessibility)

BOBUI_BEGIN_NAMESPACE

auto BOBUIestAccessibility::eventList() -> EventList&
{
    Q_CONSTINIT static EventList list;
    return list;
}

BOBUIestAccessibility *&BOBUIestAccessibility::instance()
{
    Q_CONSTINIT static BOBUIestAccessibility *ta = nullptr;
    return ta;
}


BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)
