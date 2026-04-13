# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_containers_associative Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_containers_associative
    SOURCES
        tst_bench_containers_associative.cpp
    LIBRARIES
        BobUI::Test
)
