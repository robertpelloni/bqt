// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTCASE_P_H
#define BOBUIESTCASE_P_H

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

#include <BobUITest/bobuiestcase.h>
#include <BobUITest/bobuitestglobal.h>

#include <BobUICore/qstring.h>
#include <BobUICore/qnamespace.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest {
#if BOBUI_CONFIG(batch_test_support)
    Q_TESTLIB_EXPORT QList<QString> qGetTestCaseNames();
    Q_TESTLIB_EXPORT TestEntryFunction qGetTestCaseEntryFunction(const QString &name);
#endif  // BOBUI_CONFIG(batch_test_support)
} // namespace BOBUIest

BOBUI_END_NAMESPACE

#endif  // BOBUIESTCASE_P_H
