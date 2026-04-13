# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_benchmark(tst_bench_qproperty
    SOURCES
       tst_bench_qproperty.cpp
       propertytester.h
    LIBRARIES
        BobUI::Core
        BobUI::Test
)
