include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6widgets_args BobUI6Widgets)
find_package(Qt6Widgets ${_bobui_find_qt6widgets_args})

if(Qt6Widgets_FOUND)
    set(BobUI6Widgets_FOUND TRUE)
    set(_bobui_module_version "${Qt6Widgets_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Widgets_VERSION "${_bobui_module_version}")
    set(BobUI6Widgets_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Widgets" "Qt6::Widgets")
    _bobui_create_target_forwarder("BobUI::Widgets" "Qt::Widgets")
    if(NOT TARGET "BobUI::Widgets")
        _bobui_create_target_forwarder("BobUI::Widgets" "Qt6::Widgets")
    endif()
else()
    set(BobUI6Widgets_FOUND FALSE)
endif()
