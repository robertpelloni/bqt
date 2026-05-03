include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6shadertools_args BobUI6ShaderTools)
find_package(Qt6ShaderTools ${_bobui_find_qt6shadertools_args})

if(Qt6ShaderTools_FOUND)
    set(BobUI6ShaderTools_FOUND TRUE)
    set(_bobui_module_version "${Qt6ShaderTools_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6ShaderTools_VERSION "${_bobui_module_version}")
    set(BobUI6ShaderTools_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::ShaderTools" "Qt6::ShaderTools")
    _bobui_create_target_forwarder("BobUI::ShaderTools" "Qt::ShaderTools")
    if(NOT TARGET "BobUI::ShaderTools")
        _bobui_create_target_forwarder("BobUI::ShaderTools" "Qt6::ShaderTools")
    endif()
else()
    set(BobUI6ShaderTools_FOUND FALSE)
endif()
