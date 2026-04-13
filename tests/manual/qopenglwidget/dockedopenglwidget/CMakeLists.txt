# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(CMAKE_AUTOUIC ON)

bobui_internal_add_manual_test(dockedopenglwidget
    GUI
    SOURCES
        main.cpp
        geometryengine.cpp geometryengine.h
        mainwidget.cpp mainwidget.h
        mainwindow.cpp mainwindow.h
        mainwindow.ui
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::OpenGL
        BobUI::OpenGLWidgets
        BobUI::Widgets
        BobUI::WidgetsPrivate
)

bobui_add_resources(dockedopenglwidget "dockedopenglwidget"
    PREFIX
        "/"
    FILES
        vshader.glsl
        fshader.glsl
        cube.png
)
