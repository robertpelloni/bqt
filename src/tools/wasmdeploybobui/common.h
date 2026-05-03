// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef COMMON_H
#define COMMON_H

#include <QHash>
#include <QString>

struct PreloadEntry
{
    QString source;
    QString destination;

    bool operator==(const PreloadEntry &other) const
    {
        return source == other.source && destination == other.destination;
    }
};

inline size_t qHash(const PreloadEntry &key, size_t seed = 0)
{
    return qHash(key.source, seed) ^ qHash(key.destination, seed);
}

#endif
