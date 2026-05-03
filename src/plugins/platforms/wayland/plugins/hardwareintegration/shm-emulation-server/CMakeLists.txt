# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from shm-emulation-server.pro.

#####################################################################
## ShmServerBufferPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(ShmServerBufferPlugin
    OUTPUT_NAME shm-emulation-server
    PLUGIN_TYPE wayland-graphics-integration-client
    SOURCES
        shmserverbufferintegration.cpp shmserverbufferintegration.h
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::OpenGL
        BobUI::WaylandClientPrivate
        Wayland::Client
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_LGPL3
)

bobui6_generate_wayland_protocol_client_sources(ShmServerBufferPlugin
    PRIVATE_CODE
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/extensions/server-buffer-extension.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/extensions/shm-emulation-server-buffer.xml
)

#### Keys ignored in scope 1:.:.:shm-emulation-server.pro:<TRUE>:
# OTHER_FILES = "shm-emulation-server.json"
# QMAKE_CXXFLAGS_WARN_ON = "--Wcast-qual"
