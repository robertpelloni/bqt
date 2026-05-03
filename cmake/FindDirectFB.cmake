# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

find_package(PkgConfig QUIET)

pkg_check_modules(DirectFB IMPORTED_TARGET "directfb")

if (NOT TARGET PkgConfig::DirectFB)
    set(DirectFB_FOUND 0)
endif()
