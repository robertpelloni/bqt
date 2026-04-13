# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobuicpserver Test:
#####################################################################

bobui_internal_add_test(tst_bobuicpserver
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_bobuicpserver.cpp
    LIBRARIES
        BobUI::Network
    BOBUI_TEST_SERVER_LIST "danted" "cyrus" "squid" "ftp-proxy"
)

add_dependencies(tst_bobuicpserver
    crashingServer
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bobuicpserver CONDITION WIN32
    LIBRARIES
        ws2_32
)
