// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QDataStream>
#include <QImage>

class QXxx
{ };

//! [0]
QDataStream &operator<<(QDataStream &, const QXxx &);
QDataStream &operator>>(QDataStream &, QXxx &);
//! [0]

//! [1]
QDataStream &operator<<(QDataStream &stream, const QImage &image);
QDataStream &operator>>(QDataStream &stream, QImage &image);
//! [1]
