# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qreadwritelock Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qreadwritelock
    SOURCES
        tst_bench_qreadwritelock.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Test
)
