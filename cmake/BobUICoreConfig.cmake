include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6core_args BobUICore)
find_package(BobUI6Core ${_bobui_find_bobui6core_args})

if(BobUI6Core_FOUND)
    set(BobUICore_FOUND TRUE)
    set(BobUICore_VERSION "${BobUI6Core_VERSION}")
    set(BobUICore_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Core" "BobUI6::Core")
else()
    set(BobUICore_FOUND FALSE)
endif()
