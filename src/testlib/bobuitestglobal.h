// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUITESTGLOBAL_H
#define BOBUITESTGLOBAL_H

#include <BobUICore/qglobal.h>
#include <BobUITest/bobuitestlib-config.h>
#include <BobUITest/bobuitestexports.h>

BOBUI_BEGIN_NAMESPACE

#if (defined Q_CC_HPACC) && (defined __ia64)
# ifdef Q_TESTLIB_EXPORT
#  undef Q_TESTLIB_EXPORT
# endif
# define Q_TESTLIB_EXPORT
#endif

#define BOBUIEST_VERSION     BOBUI_VERSION
#define BOBUIEST_VERSION_STR BOBUI_VERSION_STR

namespace BOBUIest
{
    enum TestFailMode { Abort = 1, Continue = 2 };
    enum class ComparisonOperation {
        CustomCompare, /* Used for QCOMPARE() */
        Equal,
        NotEqual,
        LessThan,
        LessThanOrEqual,
        GreaterThan,
        GreaterThanOrEqual,
        ThreeWayCompare,
    };
}

BOBUI_END_NAMESPACE

#endif
