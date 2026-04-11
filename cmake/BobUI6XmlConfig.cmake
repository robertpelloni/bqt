include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_qt6xml_args BobUI6Xml)
find_package(Qt6Xml ${_bobui_find_qt6xml_args})

if(Qt6Xml_FOUND)
    set(BobUI6Xml_FOUND TRUE)
    set(_bobui_module_version "${Qt6Xml_VERSION}")
    if("${_bobui_module_version}" STREQUAL "" AND DEFINED Qt6_VERSION)
        set(_bobui_module_version "${Qt6_VERSION}")
    endif()
    set(BobUI6Xml_VERSION "${_bobui_module_version}")
    set(BobUI6Xml_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI6::Xml" "Qt6::Xml")
    _bobui_create_target_forwarder("BobUI::Xml" "Qt::Xml")
    if(NOT TARGET "BobUI::Xml")
        _bobui_create_target_forwarder("BobUI::Xml" "Qt6::Xml")
    endif()
else()
    set(BobUI6Xml_FOUND FALSE)
endif()
