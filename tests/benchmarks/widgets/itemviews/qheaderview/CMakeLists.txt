# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qheaderview Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qheaderview
    SOURCES
        qheaderviewbench.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
)
