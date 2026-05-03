// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTBLACKLIST_P_H
#define BOBUIESTBLACKLIST_P_H

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

#include <BobUITest/bobuitestglobal.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIestPrivate {
    // Export functions so they can also be used by QQuickTest
    Q_TESTLIB_EXPORT void parseBlackList();
    Q_TESTLIB_EXPORT bool checkBlackLists(const char *slot, const char *data,
                                          const char *global = nullptr);
    Q_TESTLIB_EXPORT QSet<QByteArray> blacklistKeywords();
}

BOBUI_END_NAMESPACE

#endif
