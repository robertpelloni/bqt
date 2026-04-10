# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QEglFSOpenWFDIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEglFSOpenWFDIntegrationPlugin
    OUTPUT_NAME qeglfs-openwfd-integration
    PLUGIN_TYPE egldeviceintegrations
    SOURCES
        qeglfsopenwfdintegration.cpp qeglfsopenwfdintegration.h
        qeglfsopenwfdmain.cpp
    INCLUDE_DIRECTORIES
        ../../api
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::EglFSDeviceIntegrationPrivate
        BobUI::Gui
        BobUI::GuiPrivate
)
