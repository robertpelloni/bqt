# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qcolor Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qcolor
    SOURCES
        tst_qcolor.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Test
)
