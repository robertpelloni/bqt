include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6openglwidgets_args BobUI6OpenGLWidgets)
find_package(Qt6OpenGLWidgets ${_bobui_find_qt6openglwidgets_args})

if(Qt6OpenGLWidgets_FOUND)
    set(BobUI6OpenGLWidgets_FOUND TRUE)
    set(_bobui_module_version "${Qt6OpenGLWidgets_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6OpenGLWidgets_VERSION "${_bobui_module_version}")
    set(BobUI6OpenGLWidgets_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::OpenGLWidgets" "Qt6::OpenGLWidgets")
    _bobui_create_target_forwarder("BobUI::OpenGLWidgets" "Qt::OpenGLWidgets")
    if(NOT TARGET "BobUI::OpenGLWidgets")
        _bobui_create_target_forwarder("BobUI::OpenGLWidgets" "Qt6::OpenGLWidgets")
    endif()
else()
    set(BobUI6OpenGLWidgets_FOUND FALSE)
endif()
