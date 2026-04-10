# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## assert Binary:
#####################################################################

bobui_internal_add_executable(assert
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_assert.cpp
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(assert)
