# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_tostring Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_tostring
    SOURCES
        tst_tostring.cpp
    LIBRARIES
        BobUI::Test
)
