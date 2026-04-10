# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(test_plugins)

find_package(BobUI6Network 6.0.0 REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT BobUI6Network_PLUGINS)
  message(SEND_ERROR "BobUI6 network plugins not known!")
endif()

foreach(plugin ${BobUI6Network_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  if (NOT EXISTS "${_loc}")
    message(SEND_ERROR "Plugin ${plugin} not found at ${_loc}")
  endif()
endforeach()
