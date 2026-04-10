# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qsqlrecord Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qsqlrecord
    SOURCES
        tst_bench_qsqlrecord.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Sql
        BobUI::SqlPrivate
        BobUI::Test
)
