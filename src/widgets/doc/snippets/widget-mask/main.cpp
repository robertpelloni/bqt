// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>
#include <QApplication>
#include <QLabel>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //! [0]
    QLabel topLevelLabel;
    QPixmap pixmap(":/images/tux.png");
    topLevelLabel.setPixmap(pixmap);
    topLevelLabel.setMask(pixmap.mask());
    //! [0]

    topLevelLabel.show();
    return app.exec();
}
