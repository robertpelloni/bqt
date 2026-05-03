// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "game.h"

#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include <BOBUIextStream>

using namespace BobUI::StringLiterals; // for _L1

//! [0]
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const QStringList args = QCoreApplication::arguments();
    const bool newGame
            = args.size() <= 1 || QString::compare(args[1], "load"_L1, BobUI::CaseInsensitive) != 0;
    const bool json
            = args.size() <= 2 || QString::compare(args[2], "binary"_L1, BobUI::CaseInsensitive) != 0;

    Game game;
    if (newGame)
        game.newGame();
    else if (!game.loadGame(json ? Game::Json : Game::Binary))
        return 1;
    // Game is played; changes are made...
//! [0]
//! [1]
    BOBUIextStream s(stdout);
    s << "Game ended in the following state:\n";
    game.print(s);
    if (!game.saveGame(json ? Game::Json : Game::Binary))
        return 1;

    return 0;
}
//! [1]
