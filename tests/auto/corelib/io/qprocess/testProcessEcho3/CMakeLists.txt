# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## testProcessEcho3 Binary:
#####################################################################

bobui_internal_add_executable(testProcessEcho3
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    CORE_LIBRARY None
    SOURCES
        main.cpp
)
