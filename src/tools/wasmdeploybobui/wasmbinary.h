// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef WASMBINARY_H
#define WASMBINARY_H

#include <QString>
#include <QList>

class WasmBinary
{
public:
    enum class Type { INVALID, STATIC, SHARED };
    WasmBinary(QString filepath);
    Type type;
    QList<QString> dependencies;

private:
    bool parsePreambule(QByteArrayView data);
    size_t getLeb(QByteArrayView data, size_t &offset);
    QString getString(QByteArrayView data, size_t &offset);
};

#endif
