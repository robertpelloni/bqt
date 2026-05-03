# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qstring Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qstring
    SOURCES
        tst_bench_qstring.cpp
    LIBRARIES
        BobUI::Test
)
