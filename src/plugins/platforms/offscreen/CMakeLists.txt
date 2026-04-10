# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QOffscreenIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QOffscreenIntegrationPlugin
    OUTPUT_NAME qoffscreen
    PLUGIN_TYPE platforms
    DEFAULT_IF "offscreen" IN_LIST BOBUI_QPA_PLATFORMS
    SOURCES
        main.cpp
        qoffscreencommon.cpp qoffscreencommon.h
        qoffscreenintegration.cpp qoffscreenintegration.h
        qoffscreenwindow.cpp qoffscreenwindow.h
    DEFINES
        BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QOffscreenIntegrationPlugin CONDITION BOBUI_FEATURE_opengl AND BOBUI_FEATURE_xlib AND NOT BOBUI_FEATURE_opengles2
    SOURCES
        qoffscreenintegration_x11.cpp qoffscreenintegration_x11.h
    LIBRARIES
        X11::X11
)
