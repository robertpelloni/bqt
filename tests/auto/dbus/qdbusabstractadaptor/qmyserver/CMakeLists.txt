# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qmyserver Binary:
#####################################################################

bobui_internal_add_executable(qmyserver
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    NO_INSTALL
    SOURCES
        ../myobject.h
        qmyserver.cpp
    LIBRARIES
        BobUI::DBus
)
