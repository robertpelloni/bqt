// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef GAME_H
#define GAME_H

#include "character.h"
#include "level.h"

#include <QJsonObject>
#include <QList>

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextStream)

//! [0]
class Game
{
public:
    enum SaveFormat { Json, Binary };

    Character player() const;
    QList<Level> levels() const;

    void newGame();
    bool loadGame(SaveFormat saveFormat);
    bool saveGame(SaveFormat saveFormat) const;

    void read(const QJsonObject &json);
    QJsonObject toJson() const;

    void print(BOBUIextStream &s, int indentation = 0) const;

private:
    Character mPlayer;
    QList<Level> mLevels;
};
//! [0]

#endif // GAME_H
