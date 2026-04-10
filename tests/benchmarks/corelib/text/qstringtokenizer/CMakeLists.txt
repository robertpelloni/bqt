# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qstringtokenizer Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qstringtokenizer
    SOURCES
        tst_bench_qstringtokenizer.cpp
    LIBRARIES
        BobUI::Test
)

bobui_add_resources(tst_bench_qstringtokenizer "qstringtokenizer_bench_data"
    PREFIX
        "/"
    FILES
        "./data/lorem.txt"
)
