# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QEglFSVivIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEglFSVivIntegrationPlugin
    OUTPUT_NAME qeglfs-viv-integration
    PLUGIN_TYPE egldeviceintegrations
    SOURCES
        qeglfsvivintegration.cpp qeglfsvivintegration.h
        qeglfsvivmain.cpp
    DEFINES
        EGL_API_FB=1
        LINUX=1
    INCLUDE_DIRECTORIES
        ../../api
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::EglFSDeviceIntegrationPrivate
        BobUI::Gui
        BobUI::GuiPrivate
)
