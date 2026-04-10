# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_windeploybobui Test:
#####################################################################

bobui_internal_add_test(tst_windeploybobui
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_windeploybobui.cpp
)
