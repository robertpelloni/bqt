// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QCborStreamReader>

extern "C" int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {
    QCborStreamReader reader(QByteArray::fromRawData(Data, Size));
    while (reader.isValid())
        reader.next(1024);
    return 0;
}
