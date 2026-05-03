# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(test_dbus_module)

find_package(BobUI6DBus 6.0.0 REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(my_srcs mydbusobject.cpp)

bobui_wrap_cpp(moc_files mydbusobject.h)

bobui_generate_dbus_interface(
    mydbusobject.h
    ${CMAKE_BINARY_DIR}/org.bobuiProject.Tests.MyDBusObject.xml
)

bobui_add_dbus_adaptor(my_srcs
    ${CMAKE_BINARY_DIR}/org.bobuiProject.Tests.MyDBusObject.xml
    mydbusobject.h
    MyDBusObject
)

add_executable(myobject ${my_srcs} ${moc_files})
target_link_libraries(myobject PRIVATE BobUI6::DBus)
