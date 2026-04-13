# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qfile_vs_qnetworkaccessmanager Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qfile_vs_qnetworkaccessmanager
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Network
        BobUI::Test
)
