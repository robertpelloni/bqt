include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6widgets_args BobUIWidgets)
find_package(BobUI6Widgets ${_bobui_find_bobui6widgets_args})

if(BobUI6Widgets_FOUND)
    set(BobUIWidgets_FOUND TRUE)
    set(BobUIWidgets_VERSION "${BobUI6Widgets_VERSION}")
    set(BobUIWidgets_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Widgets" "BobUI6::Widgets")
else()
    set(BobUIWidgets_FOUND FALSE)
endif()
