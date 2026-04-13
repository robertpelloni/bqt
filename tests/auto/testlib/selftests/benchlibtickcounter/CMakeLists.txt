# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## benchlibtickcounter Binary:
#####################################################################

bobui_internal_add_executable(benchlibtickcounter
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        tst_benchlibtickcounter.cpp
    LIBRARIES
        BobUI::TestPrivate
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(benchlibtickcounter)
