# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## multiscreen-menus Binary:
#####################################################################

bobui_internal_add_manual_test(multiscreen-menus
    GUI
    SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
