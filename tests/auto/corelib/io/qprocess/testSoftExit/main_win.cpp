// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <windows.h>
#include <stdio.h>

int main()
{
    printf("Ready\n");
    fflush(stdout);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_CLOSE)
            PostQuitMessage(0);
    }

    return int(msg.wParam);
}
