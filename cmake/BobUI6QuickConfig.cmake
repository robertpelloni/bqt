include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6quick_args BobUI6Quick)
find_package(Qt6Quick ${_bobui_find_qt6quick_args})

if(Qt6Quick_FOUND)
    set(BobUI6Quick_FOUND TRUE)
    set(_bobui_module_version "${Qt6Quick_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Quick_VERSION "${_bobui_module_version}")
    set(BobUI6Quick_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Quick" "Qt6::Quick")
    _bobui_create_target_forwarder("BobUI::Quick" "Qt::Quick")
    if(NOT TARGET "BobUI::Quick")
        _bobui_create_target_forwarder("BobUI::Quick" "Qt6::Quick")
    endif()
else()
    set(BobUI6Quick_FOUND FALSE)
endif()
