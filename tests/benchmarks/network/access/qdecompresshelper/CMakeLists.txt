# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qdecompresshelper Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qdecompresshelper
    SOURCES
        main.cpp
    DEFINES
        SRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}
    LIBRARIES
        BobUI::NetworkPrivate
        BobUI::Test
)
