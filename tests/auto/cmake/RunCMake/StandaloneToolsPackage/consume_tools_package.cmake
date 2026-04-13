# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

find_package(BobUI6 REQUIRED COMPONENTS GarageTools)

if(NOT BOBUI_GARAGE_TOOLS_CONFIG_EXTRAS_LOADED)
    message(FATAL_ERROR "BobUI6GarageToolsConfigExtras.cmake was not loaded.")
endif()

if(NOT BOBUI_GARAGE_TOOLS_EXTRA_INCLUDE_LOADED)
    message(FATAL_ERROR "BobUI6GarageToolsExtraInclude.cmake was not loaded.")
endif()

if(NOT BOBUI_SCREW_DRIVER_LOADED)
    message(FATAL_ERROR "FindWrapScrewdriver.cmake was not loaded.")
endif()
