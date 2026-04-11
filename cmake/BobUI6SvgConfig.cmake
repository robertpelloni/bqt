include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6svg_args BobUI6Svg)
find_package(Qt6Svg ${_bobui_find_qt6svg_args})

if(Qt6Svg_FOUND)
    set(BobUI6Svg_FOUND TRUE)
    set(_bobui_module_version "${Qt6Svg_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Svg_VERSION "${_bobui_module_version}")
    set(BobUI6Svg_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Svg" "Qt6::Svg")
    _bobui_create_target_forwarder("BobUI::Svg" "Qt::Svg")
    if(NOT TARGET "BobUI::Svg")
        _bobui_create_target_forwarder("BobUI::Svg" "Qt6::Svg")
    endif()
else()
    set(BobUI6Svg_FOUND FALSE)
endif()
