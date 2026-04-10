# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(DirectFB MODULE)
bobui_find_package(EGL MODULE)

#####################################################################
## QDirectFbIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QDirectFbIntegrationPlugin
    OUTPUT_NAME qdirectfb
    PLUGIN_TYPE platforms
    SOURCES
        main.cpp
        qdirectfbbackingstore.cpp qdirectfbbackingstore.h
        qdirectfbblitter.cpp qdirectfbblitter.h
        qdirectfbconvenience.cpp qdirectfbconvenience.h
        qdirectfbcursor.cpp qdirectfbcursor.h
        qdirectfbeglhooks.h
        qdirectfbinput.cpp qdirectfbinput.h
        qdirectfbintegration.cpp qdirectfbintegration.h
        qdirectfbscreen.cpp qdirectfbscreen.h
        qdirectfbwindow.cpp qdirectfbwindow.h
    LIBRARIES
        PkgConfig::DirectFB
        EGL::EGL
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(QDirectFbIntegrationPlugin CONDITION BOBUI_CONFIG___contains___directfb_egl
    SOURCES
        qdirectfb_egl.cpp qdirectfb_egl.h
    DEFINES
        DIRECTFB_GL_EGL
)

bobui_internal_extend_target(QDirectFbIntegrationPlugin CONDITION NOT DIRECTFB_PLATFORM_HOOKS_SOURCES_ISEMPTY
    DEFINES
        DIRECTFB_PLATFORM_HOOKS
)

bobui_internal_extend_target(QDirectFbIntegrationPlugin CONDITION DIRECTFB_PLATFORM_HOOKS_SOURCES_ISEMPTY
    SOURCES
        qdirectfbeglhooks_stub.cpp
)
