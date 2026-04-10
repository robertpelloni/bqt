// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QObject>
#include <BOBUIextObjectInterface>

//! [0]
class SvgTextObject : public QObject, public BOBUIextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(BOBUIextObjectInterface)
//! [0]
};
