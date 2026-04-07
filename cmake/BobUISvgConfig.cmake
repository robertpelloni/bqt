include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6svg_args BobUISvg)
find_package(BobUI6Svg ${_bobui_find_bobui6svg_args})

if(BobUI6Svg_FOUND)
    set(BobUISvg_FOUND TRUE)
    set(BobUISvg_VERSION "${BobUI6Svg_VERSION}")
    set(BobUISvg_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Svg" "BobUI6::Svg")
else()
    set(BobUISvg_FOUND FALSE)
endif()
