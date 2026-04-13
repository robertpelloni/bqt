# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
bobui_internal_add_3rdparty_library(BundledZLIB
    STATIC
    SKIP_AUTOMOC
    SOURCES
        src/adler32.c
        src/compress.c
        src/crc32.c
        src/crc32.h
        src/deflate.c
        src/deflate.h
        src/gzclose.c
        src/gzguts.h
        src/gzlib.c
        src/gzread.c
        src/gzwrite.c
        src/infback.c
        src/inffast.c
        src/inffast.h
        src/inffixed.h
        src/inflate.c
        src/inflate.h
        src/inftrees.c
        src/inftrees.h
        src/trees.c
        src/trees.h
        src/uncompr.c
        src/zconf.h
        src/zlib.h
        src/zutil.c
        src/zutil.h
    DEFINES
        BOBUI_BUILD_CORE_LIB
    INCLUDE_DIRECTORIES
        $<TARGET_PROPERTY:Core,INCLUDE_DIRECTORIES>
)

bobui_internal_add_sync_header_dependencies(BundledZLIB Core)

bobui_disable_warnings(BundledZLIB)

bobui_set_symbol_visibility_hidden(BundledZLIB)

bobui_internal_add_3rdparty_header_module(ZlibPrivate
    EXTERNAL_HEADERS
        src/zlib.h
        src/zconf.h
)
