# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## windowgeometry Binary:
#####################################################################

bobui_internal_add_manual_test(windowgeometry
    GUI
    SOURCES
        ../windowflags/controls.cpp ../windowflags/controls.h
        controllerwidget.cpp controllerwidget.h
        main.cpp
    INCLUDE_DIRECTORIES
        ../windowflags
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
