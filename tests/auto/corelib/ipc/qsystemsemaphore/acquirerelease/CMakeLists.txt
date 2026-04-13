# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## acquirerelease_helper Binary:
#####################################################################

bobui_internal_add_test_helper(acquirerelease_helper
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Test
)
