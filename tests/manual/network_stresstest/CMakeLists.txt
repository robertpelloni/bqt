# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_network_stresstest Binary:
#####################################################################

bobui_internal_add_manual_test(tst_network_stresstest
    SOURCES
        minihttpserver.cpp minihttpserver.h
        tst_network_stresstest.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
        BobUI::Test
)

# Resources:
set(wwwfiles_resource_files
    "bobuiest"
)

bobui_internal_add_resource(tst_network_stresstest "wwwfiles"
    PREFIX
        "/"
    FILES
        ${wwwfiles_resource_files}
)
