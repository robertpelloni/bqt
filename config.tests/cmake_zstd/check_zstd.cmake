# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

file(ARCHIVE_CREATE
    OUTPUT cmake_zstd.zstd
    PATHS "${CMAKE_CURRENT_LIST_FILE}"
    FORMAT raw
    COMPRESSION Zstd)
