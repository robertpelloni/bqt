# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qutf8stringview Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qutf8stringview
    SOURCES
        tst_bench_qutf8stringview.cpp
    LIBRARIES
        BobUI::Test
        BobUI::CorePrivate
)
