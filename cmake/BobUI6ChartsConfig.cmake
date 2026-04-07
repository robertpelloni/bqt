include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6charts_args BobUI6Charts)
find_package(Qt6Charts ${_bobui_find_qt6charts_args})

if(Qt6Charts_FOUND)
    set(BobUI6Charts_FOUND TRUE)
    set(_bobui_module_version "${Qt6Charts_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Charts_VERSION "${_bobui_module_version}")
    set(BobUI6Charts_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Charts" "Qt6::Charts")
    _bobui_create_target_forwarder("BobUI::Charts" "Qt::Charts")
    if(NOT TARGET "BobUI::Charts")
        _bobui_create_target_forwarder("BobUI::Charts" "Qt6::Charts")
    endif()
else()
    set(BobUI6Charts_FOUND FALSE)
endif()
