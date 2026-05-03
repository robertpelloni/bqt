# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## testSpaceInName Binary:
#####################################################################

bobui_internal_add_executable(testSpaceInName
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    CORE_LIBRARY None
    SOURCES
        main.cpp
)
set_property(TARGET testSpaceInName PROPERTY
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../test Space In Name")
