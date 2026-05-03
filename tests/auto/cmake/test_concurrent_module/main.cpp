// Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Stephen Kelly <stephen.kelly@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qbytearray.h>
#include <BobUIConcurrent/bobuiconcurrentrun.h>

int main(int argc, char **argv)
{
    QByteArray bytearray = "hello world";
    auto result = BobUIConcurrent::run(&QByteArray::split, bytearray, ',');
    Q_UNUSED(result);

    return 0;
}
