# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qdir_tree Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qdir_tree
    SOURCES
        tst_bench_qdir_tree.cpp
    LIBRARIES
        BobUI::Test
)

# Resources:
set(qdir_tree_resource_files
    "4.6.0-list.txt"
)

bobui_internal_add_resource(tst_bench_qdir_tree "tst_bench_qdir_tree"
    PREFIX
        "/"
    FILES
        ${qdir_tree_resource_files}
)
