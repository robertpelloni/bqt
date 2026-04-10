# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qnetworkreply_from_cache Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qnetworkreply_from_cache
    SOURCES
        tst_qnetworkreply_from_cache.cpp
    LIBRARIES
        BobUI::Network
        BobUI::Test
)
