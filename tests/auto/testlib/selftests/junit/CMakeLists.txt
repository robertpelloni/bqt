# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_executable(junit
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_junit.cpp
    LIBRARIES
        BobUI::Test
)

bobui_internal_apply_testlib_coverage_options(junit)
