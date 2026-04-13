// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <stdio.h>

int main()
{
    char buffer[1024];
    for (;;) {
        size_t num = fread(buffer, 1, sizeof(buffer), stdin);
        if (num <= 0)
            break;
        fwrite(buffer, num, 1, stdout);
        fflush(stdout);
    }

    return 0;
}
