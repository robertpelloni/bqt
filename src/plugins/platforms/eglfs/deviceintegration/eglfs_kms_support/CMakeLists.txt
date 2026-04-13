# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## EglFsKmsSupportPrivate Module:
#####################################################################

bobui_internal_add_module(EglFsKmsSupportPrivate
    CONFIG_MODULE_NAME eglfs_kms_support
    INTERNAL_MODULE
    SOURCES
        qeglfskmsdevice.cpp qeglfskmsdevice_p.h
        qeglfskmseventreader.cpp qeglfskmseventreader_p.h
        qeglfskmshelpers_p.h
        qeglfskmsintegration.cpp qeglfskmsintegration_p.h
        qeglfskmsscreen.cpp qeglfskmsscreen_p.h
    DEFINES
        BOBUI_EGL_NO_X11
    PUBLIC_LIBRARIES
        Libdrm::Libdrm
        BobUI::Core
        BobUI::CorePrivate
        BobUI::EglFSDeviceIntegrationPrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::KmsSupportPrivate
    NO_GENERATE_CPP_EXPORTS
)
