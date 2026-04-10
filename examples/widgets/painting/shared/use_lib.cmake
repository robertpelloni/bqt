# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

# Include this file in your example project to use the library defined in this directory.
# This avoids find_package calls in a directory scope different from the directory scope of the
# consuming target.

if(NOT TARGET BobUI::Widgets)
    find_package(BobUI6 REQUIRED COMPONENTS Widgets)
endif()

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}" painting_shared)
