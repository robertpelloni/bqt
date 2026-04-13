# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## findtestdata Binary:
#####################################################################

bobui_internal_add_executable(findtestdata
    NO_INSTALL
    EXCEPTIONS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES
        findtestdata.cpp
    LIBRARIES
        BobUI::Test
)

# Resources:
set(findtestdata_resource_files
    "bobui.conf"
)

bobui_internal_add_resource(findtestdata "findtestdata"
    PREFIX
        "/bobui/etc/"
    FILES
        ${findtestdata_resource_files}
)


## Scopes:
#####################################################################

bobui_internal_apply_testlib_coverage_options(findtestdata)
