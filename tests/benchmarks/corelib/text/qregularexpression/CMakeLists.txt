# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qregularexpression Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qregularexpression
    SOURCES
        tst_bench_qregularexpression.cpp
    LIBRARIES
        BobUI::Test
)
