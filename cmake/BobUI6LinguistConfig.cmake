include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6linguist_args BobUI6Linguist)
find_package(Qt6Linguist ${_bobui_find_qt6linguist_args})

if(Qt6Linguist_FOUND)
    set(BobUI6Linguist_FOUND TRUE)
    set(_bobui_module_version "${Qt6Linguist_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Linguist_VERSION "${_bobui_module_version}")
    set(BobUI6Linguist_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Linguist" "Qt6::Linguist")
    _bobui_create_target_forwarder("BobUI::Linguist" "Qt::Linguist")
    if(NOT TARGET "BobUI::Linguist")
        _bobui_create_target_forwarder("BobUI::Linguist" "Qt6::Linguist")
    endif()
else()
    set(BobUI6Linguist_FOUND FALSE)
endif()
