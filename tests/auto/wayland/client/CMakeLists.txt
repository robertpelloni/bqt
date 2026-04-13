# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from client.pro.

#####################################################################
## tst_client Test:
#####################################################################

bobui_internal_add_test(tst_client
    SOURCES
        tst_client.cpp
    LIBRARIES
        SharedClientTest
)

#### Keys ignored in scope 1:.:.:client.pro:<TRUE>:
# check.commands = "$(TESTRUNNER)" "$${PWD}/run-with-all-shells.sh" "$(TESTARGS)"
