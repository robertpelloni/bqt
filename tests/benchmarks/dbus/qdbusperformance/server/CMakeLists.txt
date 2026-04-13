# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## server Binary:
#####################################################################

bobui_internal_add_executable(server
    GUI
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        ../serverobject.h
        server.cpp
    LIBRARIES
        BobUI::DBus
)
