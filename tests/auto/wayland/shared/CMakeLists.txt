# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
##Client test shared components:
#####################################################################

bobui_manual_moc(moc_files
    mockcompositor.h
    coreprotocol.h
    corecompositor.h
    datadevice.h
    fullscreenshellv1.h
    fractionalscalev1.h
    textinput.h
    bobuitextinput.h
    viewport.h
    xdgdialog.h
    xdgoutputv1.h
    xdgshell.h
)

add_library(SharedClientTest
    OBJECT
        corecompositor.cpp corecompositor.h
        coreprotocol.cpp coreprotocol.h
        datadevice.cpp datadevice.h
        fullscreenshellv1.cpp fullscreenshellv1.h
        fractionalscalev1.cpp fractionalscalev1.h
        mockcompositor.cpp mockcompositor.h
        textinput.cpp textinput.h
        bobuitextinput.cpp bobuitextinput.h
        xdgoutputv1.cpp xdgoutputv1.h
        xdgshell.cpp xdgshell.h
        xdgdialog.cpp xdgdialog.h
        viewport.cpp viewport.h
        ${moc_files}
)

bobui6_generate_wayland_protocol_server_sources(SharedClientTest
    FILES
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/cursor-shape/cursor-shape-v1.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/fullscreen-shell/fullscreen-shell-unstable-v1.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/wp-primary-selection/wp-primary-selection-unstable-v1.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/tablet/tablet-unstable-v2.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/text-input/v2/text-input-unstable-v2.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/extensions/bobui-text-input-method-unstable-v1.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/fractional-scale/fractional-scale-v1.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/viewporter/viewporter.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/wayland/wayland.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/xdg-decoration/xdg-decoration-unstable-v1.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/xdg-dialog/xdg-dialog-v1.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/xdg-output/xdg-output-unstable-v1.xml
        ${PROJECT_SOURCE_DIR}/src/3rdparty/wayland/protocols/xdg-shell/xdg-shell.xml
)

if(BOBUI_FEATURE_opengl)
    set(optional_libraries BobUI::OpenGL)
endif()

target_link_libraries(SharedClientTest
    PUBLIC
        BobUI::Gui
        ${optional_libraries}
        BobUI::WaylandClientPrivate
        Wayland::Server
        Threads::Threads # special case
)

target_include_directories(SharedClientTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
