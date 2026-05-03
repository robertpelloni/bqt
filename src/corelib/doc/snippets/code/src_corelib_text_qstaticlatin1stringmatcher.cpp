// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QStaticLatin1StringMatcher>

//! [0]
static constexpr auto matcher = qMakeStaticCaseSensitiveLatin1StringMatcher("needle");
//! [0]

void repetition()
{
    //! [1]
    static constexpr auto matcher = qMakeStaticCaseInsensitiveLatin1StringMatcher("needle");
    //! [1]
}
