# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_benchmark(tst_bench_qcborvalue
    SOURCES
        tst_bench_qcborvalue.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Test
)
