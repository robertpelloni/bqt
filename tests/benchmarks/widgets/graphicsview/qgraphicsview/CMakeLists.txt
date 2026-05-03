# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qgraphicsview Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qgraphicsview
    SOURCES
        chiptester/chip.cpp chiptester/chip.h
        chiptester/chiptester.cpp chiptester/chiptester.h
        tst_qgraphicsview.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
)

# Resources:
set(qgraphicsview_resource_files
    "images/designer.png"
    "images/wine-big.jpeg"
    "images/wine.jpeg"
    "random.data"
)

bobui_internal_add_resource(tst_bench_qgraphicsview "qgraphicsview"
    PREFIX
        "/"
    FILES
        ${qgraphicsview_resource_files}
)
set(images_resource_files
    "chiptester/bobui4logo.png"
)

bobui_internal_add_resource(tst_bench_qgraphicsview "images"
    PREFIX
        "/"
    BASE
        "chiptester"
    FILES
        ${images_resource_files}
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bench_qgraphicsview CONDITION TARGET BobUI::OpenGL
    LIBRARIES
        BobUI::OpenGL
)
