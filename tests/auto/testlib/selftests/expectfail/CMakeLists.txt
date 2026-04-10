# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## expectfail Binary:
#####################################################################

bobui_internal_add_executable(expectfail
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_expectfail.cpp
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(expectfail)
