// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QCborValue>

extern "C" int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {
    QCborValue::fromCbor(QByteArray::fromRawData(Data, Size));
    return 0;
}
