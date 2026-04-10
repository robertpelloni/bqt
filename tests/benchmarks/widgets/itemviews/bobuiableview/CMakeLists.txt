# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_bobuiableview Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_bobuiableview
    SOURCES
        tst_bobuiableview.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
)
