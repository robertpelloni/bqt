// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include "mylib.h"
#include <BobUIGui/QImageReader>

QList<QByteArray> getSupportedImageFormats()
{
    return QImageReader::supportedImageFormats();
}
