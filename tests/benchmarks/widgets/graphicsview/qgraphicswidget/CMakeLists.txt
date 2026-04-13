# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qgraphicswidget Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qgraphicswidget
    SOURCES
        tst_qgraphicswidget.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
)
