# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_add_executable(bindablesubscription
    main.cpp
    bindablesubscription.cpp
    bindablesubscription.h
    bindableuser.cpp
    bindableuser.h
)

target_link_libraries(bindablesubscription PRIVATE
    bindableproperties_shared
)

