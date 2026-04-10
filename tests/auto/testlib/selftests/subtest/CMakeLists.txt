# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## subtest Binary:
#####################################################################

bobui_internal_add_executable(subtest
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_subtest.cpp
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(subtest)
