// Copyright (C) 2017 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Rafael Roquetto <rafael.roquetto@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#include "bobuiheaders.h"

const char *bobuiHeaders()
{
    static const char headers[] = ""
              "#include <QString>\n"
              "#include <QByteArray>\n"
              "#include <QUrl>\n"
              "#include <QRect>\n";

    return headers;
}
