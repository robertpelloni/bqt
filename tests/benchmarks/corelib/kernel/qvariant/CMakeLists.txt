# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qvariant Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qvariant
    SOURCES
        tst_bench_qvariant.cpp
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bench_qvariant CONDITION TARGET BobUI::Gui
    LIBRARIES
        BobUI::Gui
)
