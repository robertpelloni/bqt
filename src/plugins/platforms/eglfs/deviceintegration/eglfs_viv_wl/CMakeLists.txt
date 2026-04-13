# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QEglFSVivWaylandIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEglFSVivWaylandIntegrationPlugin
    OUTPUT_NAME qeglfs-viv-wl-integration
    PLUGIN_TYPE egldeviceintegrations
    SOURCES
        qeglfsvivwlintegration.cpp qeglfsvivwlintegration.h
        qeglfsvivwlmain.cpp
    DEFINES
        EGL_API_FB=1
        LINUX=1
    INCLUDE_DIRECTORIES
        ../../api
    LIBRARIES
        Wayland::Server
        BobUI::Core
        BobUI::CorePrivate
        BobUI::EglFSDeviceIntegrationPrivate
        BobUI::Gui
        BobUI::GuiPrivate
)
