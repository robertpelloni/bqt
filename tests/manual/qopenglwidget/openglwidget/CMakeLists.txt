# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## openglwidget Binary:
#####################################################################

bobui_internal_add_manual_test(openglwidget
    GUI
    SOURCES
        main.cpp
        openglwidget.cpp openglwidget.h
    NO_PCH_SOURCES
        main.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::OpenGL
        BobUI::OpenGLWidgets
        BobUI::Widgets
        BobUI::WidgetsPrivate
)
