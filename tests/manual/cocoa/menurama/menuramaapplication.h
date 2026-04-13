// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef MENURAMAAPPLICATION_H
#define MENURAMAAPPLICATION_H

#include <BobUIWidgets>

#define menuApp (static_cast<MenuramaApplication *>(QCoreApplication::instance()))

class MenuramaApplication : public QApplication
{
public:
    MenuramaApplication(int &argc, char **argv);
    void addDynMenu(QLatin1String title, QMenu *parentMenu);
    QAction *findAction(QLatin1String title, QMenu *parentMenu);

public slots:
    void populateMenu(QMenu *menu, bool clear);
};

#endif // MENURAMAAPPLICATION_H
