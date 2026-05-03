# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qdatetime Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qdatetime
    SOURCES
        tst_bench_qdatetime.cpp
    LIBRARIES
        BobUI::Test
        BobUI::CorePrivate
)
