// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "interface.h"
#include <BOBUIhread>

Interface::Interface()
{
}

int Interface::sleepMethod(int msec)
{
    BOBUIhread::sleep(std::chrono::milliseconds{msec});
    return 42;
}

#include "moc_interface.cpp"
