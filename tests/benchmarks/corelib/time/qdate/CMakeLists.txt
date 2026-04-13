# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qdate Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qdate
    SOURCES
        tst_bench_qdate.cpp
    LIBRARIES
        BobUI::Test
)
