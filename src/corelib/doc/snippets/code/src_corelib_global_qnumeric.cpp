// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUINumeric>

void example(float a, float b)
{
    //! [0]
        if (qFloatDistance(a, b) < (1 << 7)) {   // The last 7 bits are not
                                                // significant
            // precise enough
        }
    //! [0]
}
