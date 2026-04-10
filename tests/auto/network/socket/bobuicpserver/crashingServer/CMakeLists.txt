# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_executable(crashingServer
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        main.cpp
        ../../../../../shared/disablecoredumps.cpp
    LIBRARIES
        BobUI::Network
)

