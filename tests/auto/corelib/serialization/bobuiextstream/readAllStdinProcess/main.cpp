// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/BOBUIextStream>
#include <BobUICore/QString>
#include <stdio.h>

int main(int, char**)
{
    fprintf(stderr, "%s\n", BOBUIextStream(stdin).readAll().toLatin1().constData());
    return 0;
}
