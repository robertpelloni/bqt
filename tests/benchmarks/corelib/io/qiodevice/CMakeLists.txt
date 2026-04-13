# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qiodevice Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qiodevice
    SOURCES
        tst_bench_qiodevice.cpp
    LIBRARIES
        BobUI::Test
)
