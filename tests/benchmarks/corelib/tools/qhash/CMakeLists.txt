# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qhash Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qhash
    SOURCES
        tst_bench_qhash.cpp
        outofline.cpp
    NO_PCH_SOURCES
        tst_bench_qhash.cpp # undef BOBUI_NO_FOREACH
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Test
)
