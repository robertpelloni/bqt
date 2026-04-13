# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#! [cmake_use]
find_package(BobUI6 REQUIRED COMPONENTS DBus)
target_link_libraries(mytarget PRIVATE BobUI6::DBus)
#! [cmake_use]
