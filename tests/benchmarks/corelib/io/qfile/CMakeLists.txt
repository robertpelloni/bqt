# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qfile Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qfile
    SOURCES
        tst_bench_qfile.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bench_qfile CONDITION WIN32
    DEFINES
        _CRT_SECURE_NO_WARNINGS
)
