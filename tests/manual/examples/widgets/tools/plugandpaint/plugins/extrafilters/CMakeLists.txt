# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_add_plugin(pnp_extrafilters
    CLASS_NAME ExtraFiltersPlugin
    extrafiltersplugin.cpp extrafiltersplugin.h
)

set_target_properties(pnp_extrafilters PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/plugins"
)

target_include_directories(pnp_extrafilters PRIVATE
    ../../app
)

target_link_libraries(pnp_extrafilters PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

install(TARGETS pnp_extrafilters
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/plugins"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/plugins"
)
