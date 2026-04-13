# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qudpsocket Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qudpsocket
    SOURCES
        tst_qudpsocket.cpp
    LIBRARIES
        BobUI::Network
        BobUI::Test
)
