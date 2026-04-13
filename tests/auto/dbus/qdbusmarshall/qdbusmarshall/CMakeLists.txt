# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdbusmarshall Test:
#####################################################################

bobui_internal_add_test(tst_qdbusmarshall
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../"
    SOURCES
        ../tst_qdbusmarshall.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::DBusPrivate
)

## Scopes:
#####################################################################

# This test case interacts with dbus code directly via the private
# headers. The include path is also not exposed via BobUI:DBus.
bobui_internal_extend_target(tst_qdbusmarshall CONDITION BOBUI_FEATURE_dbus_linked
    DEFINES
        BOBUI_LINKED_LIBDBUS
    LIBRARIES
        dbus-1
)

bobui_internal_extend_target(tst_qdbusmarshall CONDITION NOT BOBUI_FEATURE_dbus_linked
    SOURCES
        ../../../../../src/dbus/qdbus_symbols.cpp
)
