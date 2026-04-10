# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_bobuihreadpool Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_bobuihreadpool
    SOURCES
        tst_bench_bobuihreadpool.cpp
    LIBRARIES
        BobUI::Test
)
