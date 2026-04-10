# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qmetaobject Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qmetaobject
    SOURCES
        tst_bench_qmetaobject.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
)
