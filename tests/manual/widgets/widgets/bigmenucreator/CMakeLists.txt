# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## BigMenuCreator Binary:
#####################################################################

bobui_internal_add_manual_test(BigMenuCreator
    GUI
    SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
    DEFINES
        BOBUI_DEPRECATED_WARNINGS
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
