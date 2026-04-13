# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(BOBUI_FEATURE_cups AND UNIX AND NOT APPLE)
    add_subdirectory(cups)
endif()
