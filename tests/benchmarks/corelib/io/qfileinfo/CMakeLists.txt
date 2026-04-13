# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qfileinfo Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qfileinfo
    SOURCES
        tst_bench_qfileinfo.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Test
)
