# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## verbose1 Binary:
#####################################################################

bobui_internal_add_executable(verbose1
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        ../counting/tst_counting.cpp
    DEFINES
        TESTLIB_VERBOSITY_ARG=-v1
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(verbose1)
