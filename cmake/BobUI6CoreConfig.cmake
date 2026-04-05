include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6core_args BobUI6Core)
find_package(Qt6Core ${_bobui_find_qt6core_args})

if(Qt6Core_FOUND)
    set(BobUI6Core_FOUND TRUE)
    set(_bobui_module_version "${Qt6Core_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Core_VERSION "${_bobui_module_version}")
    set(BobUI6Core_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Core" "Qt6::Core")
    _bobui_create_target_forwarder("BobUI::Core" "Qt::Core")
    if(NOT TARGET "BobUI::Core")
        _bobui_create_target_forwarder("BobUI::Core" "Qt6::Core")
    endif()
else()
    set(BobUI6Core_FOUND FALSE)
endif()
