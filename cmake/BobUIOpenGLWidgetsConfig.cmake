include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6openglwidgets_args BobUIOpenGLWidgets)
find_package(BobUI6OpenGLWidgets ${_bobui_find_bobui6openglwidgets_args})

if(BobUI6OpenGLWidgets_FOUND)
    set(BobUIOpenGLWidgets_FOUND TRUE)
    set(BobUIOpenGLWidgets_VERSION "${BobUI6OpenGLWidgets_VERSION}")
    set(BobUIOpenGLWidgets_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::OpenGLWidgets" "BobUI6::OpenGLWidgets")
else()
    set(BobUIOpenGLWidgets_FOUND FALSE)
endif()
