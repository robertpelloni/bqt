# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qmap Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qmap
    SOURCES
        tst_bench_qmap.cpp
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Test
)
