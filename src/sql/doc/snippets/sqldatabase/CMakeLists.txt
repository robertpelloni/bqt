# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

add_library(sqldatabase_snippets OBJECT
    sqldatabase.cpp
)

target_link_libraries(sqldatabase_snippets PRIVATE
    BobUI::Core
    BobUI::Sql
)
