# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QEglFSMaliIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEglFSMaliIntegrationPlugin
    OUTPUT_NAME qeglfs-mali-integration
    PLUGIN_TYPE egldeviceintegrations
    SOURCES
        qeglfsmaliintegration.cpp qeglfsmaliintegration.h
        qeglfsmalimain.cpp
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
)
