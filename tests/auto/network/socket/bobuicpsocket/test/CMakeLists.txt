# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobuicpsocket Test:
#####################################################################

bobui_internal_add_test(tst_bobuicpsocket
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_bobuicpsocket.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
    BOBUI_TEST_SERVER_LIST "danted" "squid" "apache2" "ftp-proxy" "vsftpd" "iptables" "cyrus"
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bobuicpsocket CONDITION WIN32
    LIBRARIES
        ws2_32
)
