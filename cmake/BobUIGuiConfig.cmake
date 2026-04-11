include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6gui_args BobUIGui)
find_package(BobUI6Gui ${_bobui_find_bobui6gui_args})

if(BobUI6Gui_FOUND)
    set(BobUIGui_FOUND TRUE)
    set(BobUIGui_VERSION "${BobUI6Gui_VERSION}")
    set(BobUIGui_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Gui" "BobUI6::Gui")
else()
    set(BobUIGui_FOUND FALSE)
endif()
