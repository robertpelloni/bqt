# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qudpsocket Test:
#####################################################################

bobui_internal_add_test(tst_qudpsocket
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_qudpsocket.cpp
    LIBRARIES
        BobUI::Network
        BobUI::TestPrivate
    BOBUI_TEST_SERVER_LIST "danted" "echo"
)

if(BOBUI_FEATURE_process)
    add_dependencies(tst_qudpsocket clientserver)
endif()
