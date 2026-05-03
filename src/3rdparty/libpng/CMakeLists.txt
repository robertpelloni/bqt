# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
#####################################################################
## BundledLibpng Generic Library:
#####################################################################

bobui_internal_add_3rdparty_library(BundledLibpng
    QMAKE_LIB_NAME libpng
    STATIC
    SKIP_AUTOMOC
    INSTALL
    SOURCES
        png.c
        pngerror.c
        pngget.c
        pngmem.c
        pngpread.c
        pngread.c
        pngrio.c
        pngrtran.c
        pngrutil.c
        pngset.c
        pngtrans.c
        pngwio.c
        pngwrite.c
        pngwtran.c
        pngwutil.c
    DEFINES
        PNG_ARM_NEON_OPT=0
        PNG_POWERPC_VSX_OPT=0
        PNG_IMPEXP=
        _CRT_SECURE_NO_DEPRECATE
    PUBLIC_INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

bobui_internal_add_3rdparty_header_module(PngPrivate
    EXTERNAL_HEADERS
        png.h
        pngconf.h
        pnglibconf.h
)

bobui_disable_warnings(BundledLibpng)
bobui_set_symbol_visibility_hidden(BundledLibpng)

bobui_internal_extend_target(BundledLibpng CONDITION BOBUI_FEATURE_system_zlib
    LIBRARIES
        WrapZLIB::WrapZLIB
)

bobui_internal_extend_target(BundledLibpng CONDITION NOT BOBUI_FEATURE_system_zlib
    INCLUDE_DIRECTORIES
        ../zlib/src
)

bobui_internal_extend_target(BundledLibpng CONDITION NOT BOBUI_FEATURE_system_zlib
    LIBRARIES
        BobUI::Core
)
