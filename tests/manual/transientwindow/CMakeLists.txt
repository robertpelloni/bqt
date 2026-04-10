# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## transientwindow Binary:
#####################################################################

bobui_internal_add_manual_test(transientwindow
    GUI
    SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
