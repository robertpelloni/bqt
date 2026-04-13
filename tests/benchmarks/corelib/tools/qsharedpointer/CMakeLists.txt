# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_benchmark(tst_bench_shared_ptr
    SOURCES
        tst_bench_shared_ptr.cpp
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Core
        BobUI::Test
)
