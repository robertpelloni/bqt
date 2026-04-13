# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QEglFSX11IntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEglFSX11IntegrationPlugin
    OUTPUT_NAME qeglfs-x11-integration
    PLUGIN_TYPE egldeviceintegrations
    SOURCES
        qeglfsx11integration.cpp qeglfsx11integration.h
        qeglfsx11main.cpp
    DEFINES
        BOBUI_EGL_NO_X11
    INCLUDE_DIRECTORIES
        ../../api
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::EglFSDeviceIntegrationPrivate
        BobUI::Gui
        BobUI::GuiPrivate
        X11::X11
        X11::XCB
        XCB::XCB
    NO_UNITY_BUILD # X11 define clashes
)
