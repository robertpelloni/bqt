// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIGui>

int core_helper_func();
int gui_helper_func();

int main(int argc, char **argv) {
    QWindow w;
    w.show();
    return core_helper_func() + gui_helper_func();
}
