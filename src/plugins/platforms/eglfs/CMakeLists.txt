# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_find_package(EGL MODULE)

if(BOBUI_FEATURE_eglfs_gbm)
    set(_device_integration "eglfs_kms")
elseif(BOBUI_FEATURE_eglfs_egldevice)
    set(_device_integration "eglfs_kms_egldevice")
elseif(BOBUI_FEATURE_eglfs_viv)
    set(_device_integration "eglfs_viv")
elseif(BOBUI_FEATURE_eglfs_brcm)
    set(_device_integration "eglfs_brcm")
elseif(BOBUI_FEATURE_eglfs_openwfd)
    set(_device_integration "eglfs_openwfd")
endif()

set(BOBUI_QPA_DEFAULT_EGLFS_INTEGRATION "${_device_integration}" CACHE STRING "Default EGLFS device integration plugin")

#####################################################################
## EglFSDeviceIntegrationPrivate Module:
#####################################################################

bobui_internal_add_module(EglFSDeviceIntegrationPrivate
    INTERNAL_MODULE
    SOURCES
        api/qeglfsdeviceintegration.cpp api/qeglfsdeviceintegration_p.h
        api/qeglfsglobal_p.h
        api/qeglfshooks.cpp api/qeglfshooks_p.h
        api/qeglfsintegration.cpp api/qeglfsintegration_p.h
        api/qeglfsoffscreenwindow.cpp api/qeglfsoffscreenwindow_p.h
        api/qeglfsscreen.cpp api/qeglfsscreen_p.h
        api/qeglfswindow.cpp api/qeglfswindow_p.h
    DEFINES
        BOBUI_BUILD_EGL_DEVICE_LIB
        BOBUI_EGL_NO_X11
        BOBUI_NO_FOREACH
        EGLFS_PREFERRED_PLUGIN=${BOBUI_QPA_DEFAULT_EGLFS_INTEGRATION}
    INCLUDE_DIRECTORIES
        api
    PUBLIC_LIBRARIES
        BobUI::CorePrivate
        BobUI::DeviceDiscoverySupportPrivate
        BobUI::FbSupportPrivate
        BobUI::GuiPrivate
        EGL::EGL
    HEADER_SYNC_SOURCE_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/api"
    NO_GENERATE_CPP_EXPORTS
)

## Scopes:
#####################################################################

bobui_internal_extend_target(EglFSDeviceIntegrationPrivate CONDITION TARGET BobUI::InputSupportPrivate
    PUBLIC_LIBRARIES
        BobUI::InputSupportPrivate
)

bobui_internal_extend_target(EglFSDeviceIntegrationPrivate
    CONDITION TARGET PlatformGraphics::PlatformGraphics
    PUBLIC_LIBRARIES
        PlatformGraphics::PlatformGraphics
)

bobui_internal_extend_target(EglFSDeviceIntegrationPrivate CONDITION BOBUI_FEATURE_opengl
    SOURCES
        api/qeglfscontext.cpp api/qeglfscontext_p.h
        api/qeglfscursor.cpp api/qeglfscursor_p.h
    PUBLIC_LIBRARIES
        BobUI::OpenGLPrivate
)

if(BOBUI_FEATURE_cursor)
    # Resources:
    set(cursor_resource_files
        "cursor-atlas.png"
        "cursor.json"
    )

    bobui_internal_add_resource(EglFSDeviceIntegrationPrivate "cursor"
        PREFIX
            "/"
        FILES
            ${cursor_resource_files}
    )
endif()

#####################################################################
## QEglFSIntegrationPlugin Plugin:
#####################################################################

bobui_internal_add_plugin(QEglFSIntegrationPlugin
    OUTPUT_NAME qeglfs
    PLUGIN_TYPE platforms
    DEFAULT_IF "eglfs" IN_LIST BOBUI_QPA_PLATFORMS
    SOURCES
        qeglfsmain.cpp
    DEFINES
        BOBUI_EGL_NO_X11
    LIBRARIES
        BobUI::CorePrivate
        BobUI::EglFSDeviceIntegrationPrivate
        EGL::EGL
)

add_subdirectory(deviceintegration)
