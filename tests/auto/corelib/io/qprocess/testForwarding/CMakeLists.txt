# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## testForwarding Binary:
#####################################################################

bobui_internal_add_executable(testForwarding
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        main.cpp
)

add_dependencies(testForwarding testForwardingHelper)
