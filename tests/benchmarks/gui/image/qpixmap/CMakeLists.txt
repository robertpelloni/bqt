# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qpixmap Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qpixmap
    SOURCES
        tst_qpixmap.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Test
)
