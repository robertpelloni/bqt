# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qurl Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qurl
    SOURCES
        tst_bench_qurl.cpp
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bench_qurl CONDITION WIN32
    DEFINES
        _CRT_SECURE_NO_WARNINGS
)
