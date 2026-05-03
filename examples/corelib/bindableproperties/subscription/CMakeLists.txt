# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_add_executable(subscription
    main.cpp
    subscription.cpp subscription.h
    user.cpp user.h
)

target_link_libraries(subscription PRIVATE
    bindableproperties_shared
)
