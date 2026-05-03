// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/BOBUIextStream>
#include <stdio.h>

int main(int, char**)
{
    BOBUIextStream qin(stdin);
    if (!qin.atEnd()) {
        int a, b, c;
        qin >> a >> b >> c;
        fprintf(stderr, "%d %d %d\n", a, b, c);
    }
    return 0;
}
