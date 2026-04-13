# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qguivariant Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qguivariant
    SOURCES
        tst_qguivariant.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)
