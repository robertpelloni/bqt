# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(TARGET BobUI::Concurrent)
    add_subdirectory(qmimedatabase-xml-builtin)
    add_subdirectory(qmimedatabase-xml-fdoxml)
endif()
if(TARGET BobUI::Concurrent AND UNIX AND NOT APPLE AND NOT QNX)
    add_subdirectory(qmimedatabase-cache-builtin)
    add_subdirectory(qmimedatabase-cache-fdoxml)
endif()
