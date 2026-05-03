# Copyright (C) 2025 Vlad Zahorodnii <vlad.zahorodnii@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qkeysequence Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qkeysequence
    SOURCES
        tst_qkeysequence.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)
