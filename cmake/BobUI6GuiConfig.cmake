include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6gui_args BobUI6Gui)
find_package(Qt6Gui ${_bobui_find_qt6gui_args})

if(Qt6Gui_FOUND)
    set(BobUI6Gui_FOUND TRUE)
    set(_bobui_module_version "${Qt6Gui_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Gui_VERSION "${_bobui_module_version}")
    set(BobUI6Gui_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Gui" "Qt6::Gui")
    _bobui_create_target_forwarder("BobUI::Gui" "Qt::Gui")
    if(NOT TARGET "BobUI::Gui")
        _bobui_create_target_forwarder("BobUI::Gui" "Qt6::Gui")
    endif()
else()
    set(BobUI6Gui_FOUND FALSE)
endif()
