# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qsqlquery Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qsqlquery
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Sql
        BobUI::SqlPrivate
        BobUI::Test
)
