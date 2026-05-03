include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6shadertools_args BobUIShaderTools)
find_package(BobUI6ShaderTools ${_bobui_find_bobui6shadertools_args})

if(BobUI6ShaderTools_FOUND)
    set(BobUIShaderTools_FOUND TRUE)
    set(BobUIShaderTools_VERSION "${BobUI6ShaderTools_VERSION}")
    set(BobUIShaderTools_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::ShaderTools" "BobUI6::ShaderTools")
else()
    set(BobUIShaderTools_FOUND FALSE)
endif()
