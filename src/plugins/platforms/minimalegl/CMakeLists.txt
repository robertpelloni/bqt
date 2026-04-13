# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(EGL MODULE)

#####################################################################
## QMinimalEglIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QMinimalEglIntegrationPlugin
    OUTPUT_NAME qminimalegl
    PLUGIN_TYPE platforms
    DEFAULT_IF "minimalegl" IN_LIST BOBUI_QPA_PLATFORMS
    SOURCES
        main.cpp
        qminimaleglintegration.cpp qminimaleglintegration.h
        qminimaleglscreen.cpp qminimaleglscreen.h
        qminimaleglwindow.cpp qminimaleglwindow.h
    DEFINES
        BOBUI_EGL_NO_X11
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        EGL::EGL
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QMinimalEglIntegrationPlugin CONDITION BOBUI_FEATURE_opengl
    SOURCES
        qminimaleglbackingstore.cpp qminimaleglbackingstore.h
    LIBRARIES
        BobUI::OpenGL
)
