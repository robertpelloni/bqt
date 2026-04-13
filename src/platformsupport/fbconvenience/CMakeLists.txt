# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## FbSupportPrivate Module:
#####################################################################

bobui_internal_add_module(FbSupportPrivate
    CONFIG_MODULE_NAME fb_support
    STATIC
    INTERNAL_MODULE
    SOURCES
        qfbbackingstore.cpp qfbbackingstore_p.h
        qfbcursor.cpp qfbcursor_p.h
        qfbscreen.cpp qfbscreen_p.h
        qfbvthandler.cpp qfbvthandler_p.h
        qfbwindow.cpp qfbwindow_p.h
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
    PUBLIC_LIBRARIES
        BobUI::CorePrivate
        BobUI::GuiPrivate
    PRECOMPILED_HEADER
        "../../corelib/global/bobui_pch.h"
    NO_GENERATE_CPP_EXPORTS
)
