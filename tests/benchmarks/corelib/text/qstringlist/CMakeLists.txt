# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qstringlist Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qstringlist
    SOURCES
        tst_bench_qstringlist.cpp
    LIBRARIES
        BobUI::Test
)
