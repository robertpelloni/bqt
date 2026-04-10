# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_containers-sequential Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_containers_sequential
    SOURCES
        tst_bench_containers_sequential.cpp
    LIBRARIES
        BobUI::Test
)
