# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

find_package(PkgConfig QUIET)

pkg_check_modules(Libproxy IMPORTED_TARGET "libproxy-1.0")

if (NOT TARGET PkgConfig::Libproxy)
    set(Libproxy_FOUND 0)
endif()
