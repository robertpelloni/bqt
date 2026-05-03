# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qnetworkreply Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qnetworkreply
    SOURCES
        tst_qnetworkreply.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Network
        BobUI::NetworkPrivate
        BobUI::Test
)
