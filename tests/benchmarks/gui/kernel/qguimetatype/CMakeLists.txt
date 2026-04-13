# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qguimetatype Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qguimetatype
    SOURCES
        tst_qguimetatype.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)
