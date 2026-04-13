# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## stereographicsview Binary:
#####################################################################

set(CMAKE_AUTOUIC ON)

bobui_internal_add_manual_test(stereographicsview
    GUI
    SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        mygraphicsview.h
        mygraphicsview.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::OpenGL
        BobUI::OpenGLWidgets
        BobUI::Widgets
        BobUI::WidgetsPrivate
)
