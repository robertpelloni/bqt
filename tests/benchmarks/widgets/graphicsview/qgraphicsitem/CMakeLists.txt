# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qgraphicsitem Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qgraphicsitem
    SOURCES
        tst_qgraphicsitem.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
)
