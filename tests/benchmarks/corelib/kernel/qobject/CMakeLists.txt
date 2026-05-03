# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qobject Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qobject
    SOURCES
        tst_bench_qobject.cpp
        object.cpp object.h
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
)
