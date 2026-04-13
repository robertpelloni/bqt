// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QObject>

class MyObject2 : public QObject {
    Q_OBJECT
public:
    MyObject2() = default;
};

#include "main.moc"

int main()
{
    return 0;
}
