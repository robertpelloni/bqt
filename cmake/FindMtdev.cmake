# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

find_package(PkgConfig QUIET)

pkg_check_modules(Mtdev IMPORTED_TARGET "mtdev")

if (NOT TARGET PkgConfig::Mtdev)
    set(Mtdev_FOUND 0)
endif()
