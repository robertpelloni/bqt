# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_imageScale Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_imageScale
    SOURCES
        tst_qimagescale.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)
