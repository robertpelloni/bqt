// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QStringList>
#include <QCollator>

//! [0]
QStringList sortedStrings(QStringList seq)
{
    QCollator order;
    std::sort(seq.begin(), seq.end(), order);
    return seq;
}
//! [0]
