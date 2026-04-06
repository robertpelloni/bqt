include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6opengl_args BobUIOpenGL)
find_package(BobUI6OpenGL ${_bobui_find_bobui6opengl_args})

if(BobUI6OpenGL_FOUND)
    set(BobUIOpenGL_FOUND TRUE)
    set(BobUIOpenGL_VERSION "${BobUI6OpenGL_VERSION}")
    set(BobUIOpenGL_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::OpenGL" "BobUI6::OpenGL")
else()
    set(BobUIOpenGL_FOUND FALSE)
endif()
