// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#define rawstring R"blah(lorem " ipsum /*)blah";
#include <QObject>

class Object1 : public QObject
{
    Q_OBJECT
};
