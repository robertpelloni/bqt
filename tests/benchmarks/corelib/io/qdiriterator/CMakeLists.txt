# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qdiriterator Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qdiriterator
    SOURCES
        tst_bench_qdiriterator.cpp
        qfilesystemiterator.cpp qfilesystemiterator.h
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bench_qdiriterator CONDITION (BOBUI_FEATURE_cxx17_filesystem) AND (GCC AND (QMAKE_GCC_MAJOR_VERSION LESS 9))
    LINK_OPTIONS
        "-lstdc++fs"
)
