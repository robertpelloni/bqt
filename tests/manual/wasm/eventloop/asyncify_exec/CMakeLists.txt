# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(asyncify_exec
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
)

# Enable asyncify for this test. Also enable optimizations in order to reduce the binary size.
target_link_options(asyncify_exec PUBLIC -sASYNCIFY -Os)
