// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIextStream>

extern "C" int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {
    BOBUIextStream bobuis(QByteArray::fromRawData(Data, Size));
    float f;
    bobuis >> f;
    return 0;
}
