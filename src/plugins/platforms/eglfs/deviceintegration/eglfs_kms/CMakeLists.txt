# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## EglFsKmsGbmSupportPrivate Module:
#####################################################################

bobui_internal_add_module(EglFsKmsGbmSupportPrivate
    CONFIG_MODULE_NAME eglfs_kms_gbm_support
    INTERNAL_MODULE
    SOURCES
        qeglfskmsgbmcursor.cpp qeglfskmsgbmcursor_p.h
        qeglfskmsgbmdevice.cpp qeglfskmsgbmdevice_p.h
        qeglfskmsgbmintegration.cpp qeglfskmsgbmintegration_p.h
        qeglfskmsgbmscreen.cpp qeglfskmsgbmscreen_p.h
        qeglfskmsgbmwindow.cpp qeglfskmsgbmwindow_p.h
    DEFINES
        BOBUI_EGL_NO_X11
    PUBLIC_LIBRARIES
        Libdrm::Libdrm
        BobUI::CorePrivate
        BobUI::EglFSDeviceIntegrationPrivate
        BobUI::EglFsKmsSupportPrivate
        BobUI::GuiPrivate
        BobUI::KmsSupportPrivate
        gbm::gbm
    NO_GENERATE_CPP_EXPORTS
)
#####################################################################
## QEglFSKmsGbmIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEglFSKmsGbmIntegrationPlugin
    OUTPUT_NAME qeglfs-kms-integration
    PLUGIN_TYPE egldeviceintegrations
    SOURCES
        qeglfskmsgbmmain.cpp
    DEFINES
        BOBUI_EGL_NO_X11
    LIBRARIES
        Libdrm::Libdrm
        BobUI::CorePrivate
        BobUI::EglFSDeviceIntegrationPrivate
        BobUI::EglFsKmsGbmSupportPrivate
        BobUI::EglFsKmsSupportPrivate
        BobUI::GuiPrivate
        BobUI::KmsSupportPrivate
        gbm::gbm
)
