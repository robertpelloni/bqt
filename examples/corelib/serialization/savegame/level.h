// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef LEVEL_H
#define LEVEL_H

#include "character.h"

#include <QJsonObject>
#include <QList>

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextStream)

//! [0]
class Level
{
public:
    Level() = default;
    explicit Level(const QString &name);

    QString name() const;

    QList<Character> npcs() const;
    void setNpcs(const QList<Character> &npcs);

    static Level fromJson(const QJsonObject &json);
    QJsonObject toJson() const;

    void print(BOBUIextStream &s, int indentation = 0) const;

private:
    QString mName;
    QList<Character> mNpcs;
};
//! [0]

#endif // LEVEL_H
