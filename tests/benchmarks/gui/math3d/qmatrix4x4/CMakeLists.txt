# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qmatrix4x4 Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qmatrix4x4
    SOURCES
        tst_qmatrix4x4.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)
