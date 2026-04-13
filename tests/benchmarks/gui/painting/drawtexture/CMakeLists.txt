# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_drawtexture Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_drawtexture
    SOURCES
        tst_drawtexture.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Test
)
