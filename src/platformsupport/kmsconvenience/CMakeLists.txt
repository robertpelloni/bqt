# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(Libdrm MODULE)

#####################################################################
## KmsSupportPrivate Module:
#####################################################################

bobui_internal_add_module(KmsSupportPrivate
    CONFIG_MODULE_NAME kms_support
    STATIC
    INTERNAL_MODULE
    SOURCES
        qkmsdevice.cpp qkmsdevice_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
    PUBLIC_LIBRARIES
        Libdrm::Libdrm
        BobUI::CorePrivate
        BobUI::GuiPrivate
    NO_GENERATE_CPP_EXPORTS
)
