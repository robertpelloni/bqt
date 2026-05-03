# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_BOBUIext Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_BOBUIext
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Test
)
