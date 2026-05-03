# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


# The module finds its dependencies
find_package(BobUI6
    COMPONENTS Core DoesNotExist
)

if (BobUI6_FOUND)
    message(SEND_ERROR "BobUI6 umbrella package found, though it should not be!")
endif()

if (NOT BobUI6Core_FOUND)
    message(SEND_ERROR "BobUI6Core package not found!")
endif()

if (BobUI6DoesNotExist_FOUND)
    message(SEND_ERROR "Non-existent package found!")
endif()
