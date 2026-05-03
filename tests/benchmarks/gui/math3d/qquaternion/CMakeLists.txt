# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qquaternion Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qquaternion
    SOURCES
        tst_qquaternion.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)
