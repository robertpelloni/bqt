# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_imageConversion Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_imageConversion
    SOURCES
        tst_qimageconversion.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bench_imageConversion CONDITION BOBUI_FEATURE_gif
    DEFINES
        BOBUIEST_HAVE_GIF
)

bobui_internal_extend_target(tst_bench_imageConversion CONDITION BOBUI_FEATURE_jpeg
    DEFINES
        BOBUIEST_HAVE_JPEG
)
