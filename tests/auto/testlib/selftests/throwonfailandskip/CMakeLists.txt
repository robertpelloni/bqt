# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## exceptionthrow Binary:
#####################################################################

bobui_internal_add_executable(throwonfailandskip
    EXCEPTIONS
    NO_INSTALL
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEFINES
        BOBUIEST_THROW_ON_FAIL
        BOBUIEST_THROW_ON_SKIP
    SOURCES
        tst_throwonfailandskip.cpp
    LIBRARIES
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(throwonfailandskip)
