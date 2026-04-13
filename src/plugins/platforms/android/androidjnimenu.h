// Copyright (C) 2012 BogDan Vatra <bogdan@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef ANDROIDJNIMENU_H
#define ANDROIDJNIMENU_H

#include <jni.h>
#include <qglobal.h>

BOBUI_BEGIN_NAMESPACE

class QAndroidPlatformMenuBar;
class QAndroidPlatformMenu;
class QAndroidPlatformMenuItem;
class QWindow;
class QRect;
class QPoint;
class QJniEnvironment;

namespace BobUIAndroidMenu
{
    // Menu support
    void openOptionsMenu();
    void showContextMenu(QAndroidPlatformMenu *menu, const QRect &anchorRect);
    void hideContextMenu(QAndroidPlatformMenu *menu);
    void syncMenu(QAndroidPlatformMenu *menu);
    void androidPlatformMenuDestroyed(QAndroidPlatformMenu *menu);

    void setMenuBar(QAndroidPlatformMenuBar *menuBar, QWindow *window);
    void setActiveTopLevelWindow(QWindow *window);
    void addMenuBar(QAndroidPlatformMenuBar *menuBar);
    void removeMenuBar(QAndroidPlatformMenuBar *menuBar);

    // Menu support
    bool registerNatives(QJniEnvironment &env);
}

BOBUI_END_NAMESPACE

#endif // ANDROIDJNIMENU_H
