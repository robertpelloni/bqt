// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QRegularExpression>

extern "C" int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {
    QRegularExpression qre(QByteArray::fromRawData(Data, Size));
    qre.optimize();
    return 0;
}
