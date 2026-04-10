# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bench_qdbustype Binary:
#####################################################################

bobui_internal_add_benchmark(tst_bench_qdbustype
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::DBusPrivate
        BobUI::Test
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bench_qdbustype CONDITION BOBUI_FEATURE_dbus_linked
    DEFINES
        BOBUI_LINKED_LIBDBUS
    LIBRARIES
        dbus-1
)

bobui_internal_extend_target(tst_bench_qdbustype CONDITION NOT BOBUI_FEATURE_dbus_linked
    SOURCES
        ../../../../src/dbus/qdbus_symbols.cpp
)
