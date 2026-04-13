# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QXcbEglIntegrationPlugin Plugin:
#####################################################################

bobui_find_package(EGL MODULE)

bobui_internal_add_plugin(QXcbEglIntegrationPlugin
    OUTPUT_NAME qxcb-egl-integration
    PLUGIN_TYPE xcbglintegrations
    SOURCES
        qxcbeglcontext.h
        qxcbeglintegration.cpp qxcbeglintegration.h
        qxcbeglmain.cpp
        qxcbeglnativeinterfacehandler.cpp qxcbeglnativeinterfacehandler.h
        qxcbeglwindow.cpp qxcbeglwindow.h
    DEFINES
        BOBUI_NO_FOREACH
    INCLUDE_DIRECTORIES
        ..
        ../..
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::XcbQpaPrivate
        EGL::EGL
    NO_UNITY_BUILD # X11 define clashes
)
