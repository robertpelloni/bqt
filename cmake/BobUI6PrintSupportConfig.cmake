include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6printsupport_args BobUI6PrintSupport)
find_package(Qt6PrintSupport ${_bobui_find_qt6printsupport_args})

if(Qt6PrintSupport_FOUND)
    set(BobUI6PrintSupport_FOUND TRUE)
    set(_bobui_module_version "${Qt6PrintSupport_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6PrintSupport_VERSION "${_bobui_module_version}")
    set(BobUI6PrintSupport_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::PrintSupport" "Qt6::PrintSupport")
    _bobui_create_target_forwarder("BobUI::PrintSupport" "Qt::PrintSupport")
    if(NOT TARGET "BobUI::PrintSupport")
        _bobui_create_target_forwarder("BobUI::PrintSupport" "Qt6::PrintSupport")
    endif()
else()
    set(BobUI6PrintSupport_FOUND FALSE)
endif()
