# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QEglFSEmulatorIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEglFSEmulatorIntegrationPlugin
    OUTPUT_NAME qeglfs-emu-integration
    PLUGIN_TYPE egldeviceintegrations
    SOURCES
        qeglfsemulatorintegration.cpp qeglfsemulatorintegration.h
        qeglfsemulatorscreen.cpp qeglfsemulatorscreen.h
        qeglfsemumain.cpp
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
