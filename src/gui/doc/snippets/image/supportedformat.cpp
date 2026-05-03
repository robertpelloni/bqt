// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>

void wrapper() {
//! [0]
    QImageWriter writer;
    writer.setFormat("png");
    if (writer.supportsOption(QImageIOHandler::Description))
        qDebug() << "Png supports embedded text";
//! [0]

} // wrapper
