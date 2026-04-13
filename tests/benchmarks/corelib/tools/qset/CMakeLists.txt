# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qset Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qset
    SOURCES
        tst_bench_qset.cpp
    LIBRARIES
        BobUI::Test
)
