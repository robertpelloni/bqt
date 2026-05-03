# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qcoreapplication Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qcoreapplication
    SOURCES
        tst_bench_qcoreapplication.cpp
    LIBRARIES
        BobUI::Test
)
