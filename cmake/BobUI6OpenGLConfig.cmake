include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6opengl_args BobUI6OpenGL)
find_package(Qt6OpenGL ${_bobui_find_qt6opengl_args})

if(Qt6OpenGL_FOUND)
    set(BobUI6OpenGL_FOUND TRUE)
    set(_bobui_module_version "${Qt6OpenGL_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6OpenGL_VERSION "${_bobui_module_version}")
    set(BobUI6OpenGL_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::OpenGL" "Qt6::OpenGL")
    _bobui_create_target_forwarder("BobUI::OpenGL" "Qt::OpenGL")
    if(NOT TARGET "BobUI::OpenGL")
        _bobui_create_target_forwarder("BobUI::OpenGL" "Qt6::OpenGL")
    endif()
else()
    set(BobUI6OpenGL_FOUND FALSE)
endif()
