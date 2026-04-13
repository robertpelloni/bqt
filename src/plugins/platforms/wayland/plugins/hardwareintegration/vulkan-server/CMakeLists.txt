# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from vulkan-server.pro.

#####################################################################
## VulkanServerBufferPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(VulkanServerBufferPlugin
    OUTPUT_NAME vulkan-server
    PLUGIN_TYPE wayland-graphics-integration-client
    SOURCES
        vulkanserverbufferintegration.cpp vulkanserverbufferintegration.h
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::OpenGL
        BobUI::WaylandClientPrivate
        Wayland::Client
    BOBUI_LICENSE_ID BOBUI_COMMERCIAL_OR_LGPL3
)

bobui6_generate_wayland_protocol_client_sources(VulkanServerBufferPlugin
    PRIVATE_CODE
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/extensions/bobui-vulkan-server-buffer-unstable-v1.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../3rdparty/wayland/extensions/server-buffer-extension.xml

)

#### Keys ignored in scope 1:.:.:vulkan-server.pro:<TRUE>:
# OTHER_FILES = "vulkan-server.json"
# QMAKE_CXXFLAGS_WARN_ON = "--Wcast-qual"
