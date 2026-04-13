# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_network_remote_stresstest Binary:
#####################################################################

bobui_internal_add_manual_test(tst_network_remote_stresstest
    SOURCES
        tst_network_remote_stresstest.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Network
        BobUI::NetworkPrivate
        BobUI::Test
)

# Resources:
set(url-list_resource_files
    "url-list.txt"
)

bobui_internal_add_resource(tst_network_remote_stresstest "url-list"
    PREFIX
        "/"
    FILES
        ${url-list_resource_files}
)

