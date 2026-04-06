include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6network_args BobUI6Network)
find_package(Qt6Network ${_bobui_find_qt6network_args})

if(Qt6Network_FOUND)
    set(BobUI6Network_FOUND TRUE)
    set(_bobui_module_version "${Qt6Network_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Network_VERSION "${_bobui_module_version}")
    set(BobUI6Network_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Network" "Qt6::Network")
    _bobui_create_target_forwarder("BobUI::Network" "Qt::Network")
    if(NOT TARGET "BobUI::Network")
        _bobui_create_target_forwarder("BobUI::Network" "Qt6::Network")
    endif()
else()
    set(BobUI6Network_FOUND FALSE)
endif()
