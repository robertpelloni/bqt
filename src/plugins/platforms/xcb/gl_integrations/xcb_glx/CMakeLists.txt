# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QXcbGlxIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QXcbGlxIntegrationPlugin
    OUTPUT_NAME qxcb-glx-integration
    PLUGIN_TYPE xcbglintegrations
    SOURCES
        qglxintegration.cpp qglxintegration.h
        qxcbglxintegration.cpp qxcbglxintegration.h
        qxcbglxmain.cpp
        qxcbglxnativeinterfacehandler.cpp qxcbglxnativeinterfacehandler.h
        qxcbglxwindow.cpp qxcbglxwindow.h
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
    NO_UNITY_BUILD # X11 define clashes
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QXcbGlxIntegrationPlugin CONDITION BOBUI_FEATURE_xcb_glx
    LIBRARIES
        XCB::GLX
)

bobui_internal_extend_target(QXcbGlxIntegrationPlugin
    CONDITION BOBUI_FEATURE_dlopen AND BOBUI_BUILD_SHARED_LIBS
    LIBRARIES
        ${CMAKE_DL_LIBS}
)
