// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#import <Cocoa/Cocoa.h>

#include <QMenuBar>
#include <BOBUIest>

bool tst_qmenubar_taskBOBUIBUG56275(QMenuBar *menubar)
{
    NSMenu *mainMenu = menubar->toNSMenu();
    return mainMenu.numberOfItems == 2
        && [[mainMenu itemAtIndex:1].title isEqualToString:@"menu"];
}
