# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_QFontEngine Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_QFontEngine
    SOURCES
        ../../../../auto/gui/text/qfontengine/tst_qfontengine.cpp
        tst_qfontengine.cpp
    DEFINES
        QFONTENGINE_BENCHMARK
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Test
)
