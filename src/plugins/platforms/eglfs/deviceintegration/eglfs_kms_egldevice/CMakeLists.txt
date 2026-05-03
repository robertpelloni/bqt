# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## QEglFSKmsEglDeviceIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEglFSKmsEglDeviceIntegrationPlugin
    OUTPUT_NAME qeglfs-kms-egldevice-integration
    PLUGIN_TYPE egldeviceintegrations
    SOURCES
        qeglfskmsegldevice.cpp qeglfskmsegldevice.h
        qeglfskmsegldeviceintegration.cpp qeglfskmsegldeviceintegration.h
        qeglfskmsegldevicemain.cpp
        qeglfskmsegldevicescreen.cpp qeglfskmsegldevicescreen.h
    DEFINES
        BOBUI_EGL_NO_X11
    LIBRARIES
        Libdrm::Libdrm
        BobUI::Core
        BobUI::CorePrivate
        BobUI::EglFSDeviceIntegrationPrivate
        BobUI::EglFsKmsSupportPrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::KmsSupportPrivate
)
